#include "ring.hpp"
#include <cassert>
#include <ctime>


extern int RING_DEBUG_TRACE_COROUTINE_SCHED;


//
static std::unordered_map<CO_ID, RingCoroutine*> coroutine_map;

//
// static RingCoroutine* root_coroutine = nullptr;

//
static CO_ID global_coroutine_id = 0;

//
static CO_ID get_next_coroutine_id() {
    return global_coroutine_id++;
}


/*
 * launch_root_coroutine
 *
 * TODO:
 * 1. 这里写的不太好，rvm->executer 有可能还是 nullptr，需要确定初始化时机
 * 2. 这里应该 拆分成 create 和 launch 两个函数，类似于  launch 和 resume 函数
 */
RingCoroutine* launch_root_coroutine(Ring_VirtualMachine* rvm) {
    RingCoroutine* co = (RingCoroutine*)mem_alloc(rvm->meta_pool, sizeof(RingCoroutine));
    co->co_id         = get_next_coroutine_id();
    co->p_co_id       = -1;
    co->last_run_time = -1;
    co->status        = CO_STAT_INIT;
    co->runtime_stack = new_runtime_stack();
    co->call_info     = nullptr;

    // step-1: new and stroe callinfo
    RVM_CallInfo* callinfo         = (RVM_CallInfo*)mem_alloc(rvm->meta_pool, sizeof(RVM_CallInfo));
    callinfo->caller_object        = nullptr;
    callinfo->caller_function      = nullptr;
    callinfo->caller_closure       = nullptr;
    callinfo->caller_stack_base    = 0;
    callinfo->callee_object        = nullptr;
    callinfo->callee_function      = nullptr;
    callinfo->callee_closure       = nullptr;
    callinfo->callee_argument_size = 0;
    callinfo->code_list            = rvm->executer->bootloader_code_list;
    callinfo->code_size            = rvm->executer->bootloader_code_size;
    callinfo->pc                   = 0;
    callinfo->caller_resume_pc     = 0;
    callinfo->coroutine_resume_pc  = 0;
    callinfo->prev                 = nullptr;
    callinfo->next                 = nullptr;

    co->call_info                  = store_callinfo(co->call_info, callinfo);


    coroutine_map.insert(std::make_pair(co->co_id, co));

    return co;
}

/*
 * launch_coroutine
 *
 * 创建一个新的协程但不运行
 *
 * runtime_stack: 独占
 * call_info: 独占
 *
 * constant_pool: 共享
 * runtime_static: 共享
 * runtime_heap: 共享
 *
 * meta_pool: 共享
 */
RingCoroutine* launch_coroutine(Ring_VirtualMachine* rvm,
                                RVM_ClassObject** caller_object, RVM_Function** caller_function, RVM_Closure** caller_closure,
                                RVM_ClassObject* callee_object, RVM_Function* callee_function, RVM_Closure* callee_closure,
                                unsigned int argument_list_size) {

    RingCoroutine* co = (RingCoroutine*)mem_alloc(rvm->meta_pool, sizeof(RingCoroutine));
    co->co_id         = get_next_coroutine_id();
    co->p_co_id       = VM_CUR_CO->co_id;
    co->last_run_time = -1;
    co->status        = CO_STAT_INIT;
    co->runtime_stack = new_runtime_stack();
    co->call_info     = nullptr;


    // step-1: new and stroe callinfo
    RVM_CallInfo* callinfo         = (RVM_CallInfo*)mem_alloc(rvm->meta_pool, sizeof(RVM_CallInfo));
    callinfo->caller_object        = *caller_object;
    callinfo->caller_function      = *caller_function;
    callinfo->caller_closure       = *caller_closure;
    callinfo->caller_stack_base    = 0;
    callinfo->caller_is_defer      = false;
    callinfo->callee_object        = callee_object;
    callinfo->callee_function      = callee_function;
    callinfo->callee_closure       = callee_closure;
    callinfo->callee_argument_size = 0;
    callinfo->curr_closure         = callee_closure;
    callinfo->code_list            = callee_function->u.derive_func->code_list;
    callinfo->code_size            = callee_function->u.derive_func->code_size;
    callinfo->pc                   = 0;
    callinfo->caller_resume_pc     = 0;
    callinfo->coroutine_resume_pc  = 0;
    callinfo->prev                 = nullptr;
    callinfo->next                 = nullptr;

    //
    co->call_info = store_callinfo(co->call_info, callinfo);

    coroutine_map.insert(std::make_pair(co->co_id, co));

    init_coroutine_entry_func_local_variable(rvm,
                                             co,
                                             callee_object,
                                             callee_function,
                                             argument_list_size);
    // callinfo->caller_stack_base = co->runtime_stack->top_index;

    if (RING_DEBUG_TRACE_COROUTINE_SCHED == 1) {
        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [create::] co_id:%llu\n", co->co_id);
    }


    // step-3: 初始化匿名函数的局部变量

    return co;
}

/*
 * init_coroutine_entry_func_local_variable 初始化协程入口函数的局部变量
 *
 * Usage:
 * e.g.  launch func_(func_arg1, func_arg2, func_arg3);
 * 需要将 func_arg1/func_arg2/func_arg3 copy到目标协程栈中
 * 用于初始化目标协程入口函数中的参数
 *
 * FIXME: 这里的实现和 init_derive_function_local_variable 实现重复了，考虑如何合并
 *
 */
void init_coroutine_entry_func_local_variable(Ring_VirtualMachine* rvm,
                                              RingCoroutine*       co,
                                              RVM_ClassObject*     callee_object,
                                              RVM_Function*        callee_function,
                                              unsigned int         argument_list_size) {

    RVM_TypeSpecifier*   type_specifier          = nullptr;
    RVM_ClassDefinition* rvm_class_definition    = nullptr;
    RVM_ClassObject*     class_ob                = nullptr;
    RVM_String*          string                  = nullptr;
    RVM_Array*           array                   = nullptr;

    unsigned int         argument_stack_index    = 0; // argument's abs-index in runtime_stack.
    unsigned int         local_vari_stack_offset = 0; // callee_function's local variable offset in runtime_stack.


    // Step-0: relocate argument's abs-index in runtime_stack.
    argument_stack_index = VM_CUR_CO_STACK_TOP_INDEX - argument_list_size;


    // Step-1: init `self` variable when callee_function is method.
    if (callee_object != nullptr) {
        co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].type             = RVM_VALUE_TYPE_CLASS_OB;
        co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].u.class_ob_value = callee_object;
        local_vari_stack_offset++;
    }


    // Step-2: init parameters by parameters.
    // if a parameter is variadic, convert arguments to array.
    // access arguments by array.
    unsigned int argument_stack_offset = 0;
    for (unsigned int i = 0;
         i < callee_function->parameter_size && argument_stack_offset < argument_list_size;
         i++, argument_stack_offset++) {

        RVM_Parameter* parameter = &callee_function->parameter_list[i];

        if (parameter->is_variadic) {
            /*
             * 1)
             * 可变参数只能是函数的最后一个参数, 直接一次性全部获取完成
             * 2)
             * 如果 parameter 是：var bool... array
             *  则变更为 bool[]
             * 如果 parameter 是：var bool[]... array
             *  则变更为 bool[!2]
             * 也就是增加一个维度
             */
            RVM_Array*   array_value = nullptr;
            unsigned int size        = argument_list_size - argument_stack_offset;

            //
            array_value = init_derive_function_variadic_argument(rvm,
                                                                 parameter,
                                                                 size);

            // 将stack中的参数放入array中
            for (unsigned array_index = 0;
                 argument_stack_offset < argument_list_size;
                 array_index++, argument_stack_offset++) {

                assert_throw_range(array_index, array_value->length);

                switch (parameter->type_specifier->kind) {
                case RING_BASIC_TYPE_BOOL: {
                    RVM_Bool tmp        = VM_CUR_CO->runtime_stack->data[argument_stack_index + argument_stack_offset].u.bool_value;
                    bool     bool_value = tmp;
                    rvm_array_set_bool(rvm, array_value, array_index, &bool_value);
                } break;
                case RING_BASIC_TYPE_INT: {
                    int int_value = VM_CUR_CO->runtime_stack->data[argument_stack_index + argument_stack_offset].u.int_value;
                    rvm_array_set_int(rvm, array_value, array_index, &int_value);
                } break;
                case RING_BASIC_TYPE_INT64: {
                    long long int64_value = VM_CUR_CO->runtime_stack->data[argument_stack_index + argument_stack_offset].u.int64_value;
                    rvm_array_set_int64(rvm, array_value, array_index, &int64_value);
                } break;
                case RING_BASIC_TYPE_DOUBLE: {
                    double double_value = VM_CUR_CO->runtime_stack->data[argument_stack_index + argument_stack_offset].u.double_value;
                    rvm_array_set_double(rvm, array_value, array_index, &double_value);
                } break;
                case RING_BASIC_TYPE_STRING: {
                    RVM_String* string_value = VM_CUR_CO->runtime_stack->data[argument_stack_index + argument_stack_offset].u.string_value;
                    rvm_array_set_string(rvm, array_value, array_index, &string_value);
                } break;
                case RING_BASIC_TYPE_CLASS: {
                    RVM_ClassObject* class_ob = VM_CUR_CO->runtime_stack->data[argument_stack_index + argument_stack_offset].u.class_ob_value;
                    rvm_array_set_class_object(rvm, array_value, array_index, &class_ob);
                } break;
                case RING_BASIC_TYPE_ARRAY: {
                    RVM_Array* array_value_item = VM_CUR_CO->runtime_stack->data[argument_stack_index + argument_stack_offset].u.array_value;
                    rvm_array_set_array(rvm, array_value, array_index, &array_value_item);
                } break;
                case RING_BASIC_TYPE_FUNC: {
                    RVM_Closure* closure = VM_CUR_CO->runtime_stack->data[argument_stack_index + argument_stack_offset].u.closure_value;
                    rvm_array_set_closure(rvm, array_value, array_index, &closure);
                } break;

                default:
                    ring_error_report("not support `any` as variadic parameter");
                    break;
                }
            }

            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].type          = RVM_VALUE_TYPE_ARRAY;
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].u.array_value = array_value;
            local_vari_stack_offset++;
            // 可变参数只能是函数的最后一个参数
            break;
        } else {
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset] =
                VM_CUR_CO->runtime_stack->data[argument_stack_index + argument_stack_offset];
            local_vari_stack_offset++;
        }
    }


    // Step-3: init local variables which defined in callee_function.
    for (unsigned int i = 0;
         i < callee_function->local_variable_size;
         i++) {

        type_specifier = callee_function->local_variable_list[i].type_specifier;

        // 初始化 self 变量
        if (callee_object != nullptr && i == 0) {
            // 如果 callee_object != nullptr
            // function->local_variable_list[0] 是self 变量
            // 将 callee_object 初始化给 self
            // 这个操作 已经在 Step-1 完成了, 该变量需要忽略.
            continue;
        }

        // local_variable_list 其实是包含 parameter_list , 避免重复初始化
        // 看看 是否已经在 Step-2初始化了
        // TODO: 后续优化一下, 这个试下方式不太好
        bool already_init = false;
        for (unsigned int p = 0; p < callee_function->parameter_size; p++) {
            RVM_Parameter* parameter = &callee_function->parameter_list[p];
            if (str_eq(callee_function->local_variable_list[i].identifier, parameter->identifier)) {
                already_init = true;
                break;
            }
        }
        if (already_init) {
            continue;
        }

        // TODO: 后续抽象成宏
        switch (type_specifier->kind) {
        case RING_BASIC_TYPE_BOOL:
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].type         = RVM_VALUE_TYPE_BOOL;
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].u.bool_value = RVM_FALSE;
            break;

        case RING_BASIC_TYPE_INT:
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].type        = RVM_VALUE_TYPE_INT;
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].u.int_value = 0;
            break;

        case RING_BASIC_TYPE_INT64:
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].type          = RVM_VALUE_TYPE_INT64;
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].u.int64_value = 0;
            break;

        case RING_BASIC_TYPE_DOUBLE:
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].type           = RVM_VALUE_TYPE_DOUBLE;
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].u.double_value = 0.0;
            break;

        case RING_BASIC_TYPE_STRING:
            string = rvm_gc_new_string_meta(rvm);
            rvm_fill_string(rvm, string, ROUND_UP8(1));
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].type           = RVM_VALUE_TYPE_STRING;
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].u.string_value = string;
            break;

        case RING_BASIC_TYPE_CLASS:
            rvm_class_definition = &(rvm->class_list[type_specifier->u.class_t->class_def_index]);
            class_ob             = rvm_gc_new_class_ob_meta(rvm);
            rvm_fill_class_ob(rvm, class_ob, rvm_class_definition);
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].type             = RVM_VALUE_TYPE_CLASS_OB;
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].u.class_ob_value = class_ob;
            break;
        case RING_BASIC_TYPE_ARRAY: {
            // 这里没有分配空间, 只分配了一下meta
            RVM_TypeSpecifier*   sub_type_specifier   = type_specifier->u.array_t->sub;
            RVM_Array_Type       array_type           = convert_rvm_array_type(type_specifier);
            RVM_ClassDefinition* sub_class_definition = nullptr;
            if (sub_type_specifier->kind == RING_BASIC_TYPE_CLASS) {
                sub_class_definition = &(rvm->class_list[sub_type_specifier->u.class_t->class_def_index]);
            }

            array = rvm_gc_new_array_meta(rvm,
                                          array_type,
                                          sub_type_specifier->kind,
                                          sub_class_definition,
                                          type_specifier->u.array_t->dimension);
            //
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].type          = RVM_VALUE_TYPE_ARRAY;
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].u.array_value = array;
        } break;
        case RING_BASIC_TYPE_FUNC: {
            RVM_Closure* closure                                                                            = rvm_gc_new_closure_meta(rvm);
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].type            = RVM_VALUE_TYPE_CLOSURE;
            co->runtime_stack->data[co->runtime_stack->top_index + local_vari_stack_offset].u.closure_value = closure;
        } break;
        default:
            break;
        }

        local_vari_stack_offset++;
    }

    assert(callee_function->local_variable_size == local_vari_stack_offset);


    // Step-End: increase top index of runtime_stack.
    co->runtime_stack->top_index += callee_function->local_variable_size;
}

/*
 * resume_coroutine
 * @co_id: Which coroutine to resume
 */
int resume_coroutine(Ring_VirtualMachine* rvm,
                     CO_ID                target_co_id,
                     RVM_ClassObject** caller_object, RVM_Function** caller_function,
                     RVM_ClassObject* callee_object, RVM_Function* callee_function) {

    RingCoroutine* curr_co   = rvm->current_coroutine;

    RingCoroutine* target_co = coroutine_map[target_co_id];
    if (target_co == nullptr) {
        // 为什么这里没有抛出异常
        // 这里的设计是，resume 一个不存在的/已经dead 协程不会报错
        return -1;
    }
    // status 为 CO_STAT_DEAD 不能 resume
    if (target_co->status == CO_STAT_DEAD) {
        return -1;
    }
    // 不能自己唤醒自己
    assert_throw_coroutine_resume_self(target_co->co_id == curr_co->co_id, curr_co->co_id);

    // 第一次被调度
    if (target_co->status == CO_STAT_INIT) {
        *caller_object   = target_co->call_info->callee_object;
        *caller_function = target_co->call_info->callee_function;
    }


    // 协程上下文切换
    // 1. 记录协程 切回时，对应的 pc
    curr_co->status                         = CO_STAT_SUSPENDED;
    curr_co->call_info->coroutine_resume_pc = curr_co->call_info->pc + 1;
    // 2. 目标协程 记录 pc
    target_co->p_co_id       = curr_co->co_id;
    target_co->status        = CO_STAT_RUNNING;
    target_co->call_info->pc = target_co->call_info->coroutine_resume_pc;
    // 3. 切换 coroutine
    rvm->current_coroutine = target_co;


    if (RING_DEBUG_TRACE_COROUTINE_SCHED == 1) {
        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [resume::] "
                            "co_id:%lld->%lld\n",
                            curr_co->co_id, target_co->co_id);

        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [resume::  store-old] "
                            "Coroutine{co_id:%lld, status:%d}\n",
                            curr_co->co_id, curr_co->status);
        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [resume::  store-old] "
                            "RuntimeStack{data:%p, top_index:%u, size:%u, capacity:%u}\n",
                            curr_co->runtime_stack->data, curr_co->runtime_stack->top_index, curr_co->runtime_stack->size, curr_co->runtime_stack->capacity);

        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [resume::restore-new] "
                            "Coroutine{co_id:%lld, status:%d}\n",
                            target_co->co_id, target_co->status);
        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [resume::restore-new] "
                            "RuntimeStack{data:%p, top_index:%u, size:%u, capacity:%u}\n",
                            target_co->runtime_stack->data, target_co->runtime_stack->top_index, target_co->runtime_stack->size, target_co->runtime_stack->capacity);
    }

    return 0;
}

/*
 * yield_coroutine
 * @co_id:
 * 协程让出CPU，交出控制权给他的parent
 * 如果 他没有parent，也就是 RootCoroutine，那么没有效果
 */
int yield_coroutine(Ring_VirtualMachine* rvm) {

    RingCoroutine* curr_co      = rvm->current_coroutine;

    CO_ID          target_co_id = curr_co->p_co_id;
    RingCoroutine* target_co    = coroutine_map[target_co_id];
    if (target_co == nullptr) {
        // 为什么这里没有抛出异常
        // 如果一个 root coroutine yield, 应该是没有效果的
        // 不应该退出
        return -1;
    }


    // get ns timestamp
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long long timestamp = (long long)(ts.tv_sec) * 1000000000 + ts.tv_nsec;

    // 协程上下文切换
    // 1. 记录协程 切回时，对应的 pc
    curr_co->status                         = CO_STAT_SUSPENDED;
    curr_co->call_info->coroutine_resume_pc = curr_co->call_info->pc + 1;
    // 2. 目标协程 记录 pc
    target_co->last_run_time = timestamp;
    target_co->status        = CO_STAT_RUNNING;
    target_co->call_info->pc = target_co->call_info->coroutine_resume_pc;
    // 3. 切换 coroutine
    rvm->current_coroutine = target_co;


    if (RING_DEBUG_TRACE_COROUTINE_SCHED == 1) {
        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [yield::] "
                            "co_id:%lld<-%lld\n",
                            target_co->co_id, curr_co->co_id);

        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [yield::  store-old] "
                            "Coroutine{co_id:%lld, status:%d}\n",
                            curr_co->co_id, curr_co->status);
        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [yield::  store-old] "
                            "RuntimeStack{data:%p, top_index:%u, size:%u, capacity:%u}\n",
                            curr_co->runtime_stack->data, curr_co->runtime_stack->top_index, curr_co->runtime_stack->size, curr_co->runtime_stack->capacity);

        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [yield::restore-new] "
                            "Coroutine{co_id:%lld, status:%d}\n",
                            target_co->co_id, target_co->status);
        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [yield::restore-new] "
                            "RuntimeStack{data:%p, top_index:%u, size:%u, capacity:%u}\n",
                            target_co->runtime_stack->data, target_co->runtime_stack->top_index, target_co->runtime_stack->size, target_co->runtime_stack->capacity);
    }


    return 0;
}


//
int finish_coroutine(Ring_VirtualMachine* rvm,
                     RVM_ClassObject** caller_object, RVM_Function** caller_function,
                     RVM_Function* callee_function) {

    CO_ID          curr_co_id = rvm->current_coroutine->co_id;
    RingCoroutine* curr_co    = rvm->current_coroutine;
    if (curr_co_id == 0) {
        // RootCoroutine finish
        // do nothing
        // TODO: 但是还得要销毁资源
        return 0;
    }

    CO_ID          target_co_id = curr_co->p_co_id;
    RingCoroutine* target_co    = coroutine_map[target_co_id];
    assert_throw_nil_coroutine(target_co_id, target_co);

    // get ns timestamp
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long long timestamp = (long long)(ts.tv_sec) * 1000000000 + ts.tv_nsec;

    // 协程上下文切换
    // 1. 记录协程 切回时，对应的 pc
    curr_co->status = CO_STAT_DEAD;
    // curr_co->call_info->coroutine_resume_pc = curr_co->call_info->pc + 1;
    // 2. 目标协程 记录 pc
    target_co->last_run_time = timestamp;
    target_co->status        = CO_STAT_RUNNING;
    target_co->call_info->pc = target_co->call_info->coroutine_resume_pc;
    // 3. 切换 coroutine
    rvm->current_coroutine = target_co;


    if (RING_DEBUG_TRACE_COROUTINE_SCHED == 1) {
        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [dead::] "
                            "co_id:%lld<-%lld\n",
                            target_co->co_id, curr_co->co_id);

        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [dead::    destory] "
                            "Coroutine{co_id:%lld, status:%d}\n",
                            curr_co->co_id, curr_co->status);
        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [dead::    destory] "
                            "RuntimeStack{data:%p, top_index:%u, size:%u, capacity:%u}\n",
                            curr_co->runtime_stack->data, curr_co->runtime_stack->top_index, curr_co->runtime_stack->size, curr_co->runtime_stack->capacity);


        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [dead::restore-new] "
                            "Coroutine{co_id:%lld, status:%d}\n",
                            target_co->co_id, target_co->status);
        printf_witch_yellow("[RING_DEBUG::trace_coroutine_sched] [dead::restore-new] "
                            "RuntimeStack{data:%p, top_index:%u, size:%u, capacity:%u}\n",
                            target_co->runtime_stack->data, target_co->runtime_stack->top_index, target_co->runtime_stack->size, target_co->runtime_stack->capacity);
    }

    coroutine_map.erase(curr_co_id);
    mem_free(rvm->meta_pool, curr_co, sizeof(RingCoroutine));
    // TODO: 复用 runtime_stack


    return 0;
}
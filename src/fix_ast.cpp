/*
 * fix_ast.cpp
 *
 * 主要对 AST进行修正, 同时做语义检查
 * 1. search global class definition.
 * 2. search global function definition.
 * 3. search global variable definition.
 * 4. search local variable definition.
 *
 * 1. Fix TypeSpecifier of every Expression.
 * 2. Update class definition index.
 * 3. Update global function index.
 */


#include "ring.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>


/*
 * bool_type_specifier int_type_specifier double_type_specifier string_type_specifier
 *
 * 直接使用 全局 TypeSpecifier, 不使用 mem_alloc
 * 因为对于基础类型的 TypeSpecifier, 不会对TypeSpecifier进行修改, 全局变量可满足要求
 */
TypeSpecifier bool_type_specifier = TypeSpecifier{
    .line_number = 0,
    .identifier  = nullptr,
    .kind        = RING_BASIC_TYPE_BOOL,
    .u           = {.array_t = nullptr},
};
TypeSpecifier int_type_specifier = TypeSpecifier{
    .line_number = 0,
    .identifier  = nullptr,
    .kind        = RING_BASIC_TYPE_INT,
    .u           = {.array_t = nullptr},
};
TypeSpecifier int64_type_specifier = TypeSpecifier{
    .line_number = 0,
    .identifier  = nullptr,
    .kind        = RING_BASIC_TYPE_INT64,
    .u           = {.array_t = nullptr},
};
TypeSpecifier double_type_specifier = TypeSpecifier{
    .line_number = 0,
    .identifier  = nullptr,
    .kind        = RING_BASIC_TYPE_DOUBLE,
    .u           = {.array_t = nullptr},
};
TypeSpecifier string_type_specifier = TypeSpecifier{
    .line_number = 0,
    .identifier  = nullptr,
    .kind        = RING_BASIC_TYPE_STRING,
    .u           = {.array_t = nullptr},
};
TypeSpecifier func_type_specifier = TypeSpecifier{
    .line_number = 0,
    .identifier  = nullptr,
    .kind        = RING_BASIC_TYPE_FUNC,
    .u           = {.array_t = nullptr},
};

extern Ring_Command_Arg global_ring_command_arg;


// 这里修正是有依赖关系的，需要重构，根据依赖图需要重新设计
void ring_compiler_fix_ast(Package* package) {

    // 一个package 创建一个 context
    RingContext ctx = RingContext{
        .package        = package,

        .optimize_level = global_ring_command_arg.optimize_level,
    };

    // step-0. fix enum
    for (TypeAlias* type_alias : package->type_alias_list) {
        fix_type_specfier(type_alias->type_specifier, package);

        if (type_alias->is_enum) {
            fix_enum_declaration(type_alias->enum_declaration, package, ctx);
        }
    }


    // step-1. fix class list
    unsigned int class_index = 0;
    for (ClassDefinition* class_def : package->class_definition_list) {
        class_def->class_index = class_index++;
        fix_class_definition(class_def);
    }

    // step-3. fix function list
    unsigned int func_index = 0;
    for (Function* func : package->function_list) {
        func->func_index = func_index++;
        if (str_eq(func->identifier, FUNC_GLOBAL_INIT)) {
            continue;
        }
        fix_function_definition(func);
    }


    for (auto tmp : package->global_block_statement_list) {
        for (Statement* statement = tmp.second; statement; statement = statement->next) {
            switch (statement->type) {
            case STATEMENT_TYPE_VAR_DECL: {

                for (VarDecl* pos = statement->u.var_decl_statement; pos != nullptr; pos = pos->next) {

                    fix_type_specfier(pos->type_specifier, package);


                    // 添加全局变量
                    pos->variable_index = package->global_var_decl_list.size();
                    pos->is_local       = 0;
                    package->global_var_decl_list.push_back(pos);
                }
            } break;
            case STATEMENT_TYPE_EXPRESSION: {
                Expression* expression = statement->u.expression;
                if (expression->type != EXPRESSION_TYPE_ASSIGN) {
                    ring_error_report("only support variable definition&init statement in global block. expression->type:%d\n", expression->type);
                }
                fix_assign_expression(expression->u.assign_expression, statement->block, nullptr);
            } break;
            default:
                ring_error_report("only support variable definition&init statement in global block. statement->type:%d\n", statement->type);
                break;
            }
        }
    }

    // step-2. fix global init statement
    // 添加一个 __global_init() 函数
    // 专门用来对全局变量的初始化
    // TODO:
    // 当前一个package中只能有一个文件, 所以 一个package只有 一个 global{} 块
    // 后续如果支持了多个文件，这里需要适配
    Function*  global_init_func = create_global_init_func(package);
    Statement* global_statement = nullptr;
    if (global_init_func != nullptr) {
        // global_init() 放在最开始的位置，重新开始修正 func_index
        package->function_list.insert(package->function_list.begin(), global_init_func);

        global_statement = global_init_func->block->statement_list;
    }
    // TODO: 这里还应该继续优化一下
    // 不能只看 有没有 statement_list
    // 对于这种情况，也不能生成 __global_init 函数
    // e.g.  global { var bool bool_value; }
    // 因为他没有对应的 initilizer, 所以不需要生成
    for (; global_statement; global_statement = global_statement->next) {
        switch (global_statement->type) {
        case STATEMENT_TYPE_VAR_DECL: {
            VarDecl* declaration = global_statement->u.var_decl_statement;
            for (; declaration != nullptr; declaration = declaration->next) {
                fix_type_specfier(declaration->type_specifier, package);
            }
        } break;
        case STATEMENT_TYPE_EXPRESSION: {
            Expression* expression = global_statement->u.expression;
            if (expression->type != EXPRESSION_TYPE_ASSIGN) {
                ring_error_report("only support variable definition&init statement in global block. expression->type:%d\n", expression->type);
            }
            fix_assign_expression(expression->u.assign_expression, global_statement->block, nullptr);
        } break;
        default:
            ring_error_report("only support variable definition&init statement in global block. statement->type:%d\n", global_statement->type);
            break;
        }
    }


    // 为什么这个放在后边
    // 需要先 注册所有的class的类型表之后，
    // 才能开始修正做语义检查
    func_index = 0;
    for (Function* func : package->function_list) {
        func->func_index = func_index++;
        if (str_eq(func->identifier, FUNC_GLOBAL_INIT)) {
            continue;
        }
        fix_function_block(func);
    }
}

Function* create_global_init_func(Package* package) {

    if (package->global_block_statement_list.empty()) {
        return nullptr;
    }

    Block* block                       = (Block*)mem_alloc(get_front_mem_pool(), sizeof(Block));
    block->start_line_number           = 0;
    block->end_line_number             = 0;
    block->type                        = BLOCK_TYPE_UNKNOW;
    block->var_decl_list_size          = 0;
    block->var_decl_list               = nullptr;
    block->statement_list_size         = 0;
    block->statement_list              = nullptr;
    block->parent_block                = nullptr;
    block->block_labels.break_label    = 0;
    block->block_labels.continue_label = 0;

    unsigned int all_statement_size    = 0;
    Statement*   all_statement         = nullptr;
    Statement*   prev                  = nullptr;
    // 处理每个源文件中的全局变量
    // 这里没有处理 命名顺序
    for (std::pair<unsigned int, Statement*> tuple : package->global_block_statement_list) {
        unsigned int statement_list_size = tuple.first;
        Statement*   statement_list      = tuple.second;

        all_statement_size += statement_list_size;
        for (; statement_list; statement_list = statement_list->next) {
            if (prev == nullptr) {
                all_statement = statement_list;
            } else {
                prev->next = statement_list;
            }

            prev = statement_list;
        }
    }

    block->statement_list         = all_statement;
    block->statement_list_size    = all_statement_size;


    Function* function            = (Function*)mem_alloc(get_front_mem_pool(), sizeof(Function));
    function->source_file         = package_unit_get_file_name();
    function->start_line_number   = 0;
    function->end_line_number     = 0;
    function->package             = package;
    function->ring_file_stat      = nullptr;
    function->identifier          = (char*)FUNC_GLOBAL_INIT;
    function->parameter_list_size = 0;
    function->parameter_list      = nullptr;
    function->return_list_size    = 0;
    function->return_list         = nullptr;
    function->block               = block;
    function->next                = nullptr;
    function->func_index          = 0;
    function->type                = FUNCTION_TYPE_DERIVE;

    return function;
}

void fix_function_definition(Function* func) {
    Parameter* pos = func->parameter_list;
    for (; pos != nullptr; pos = pos->next) {
        fix_type_specfier(pos->type_specifier, func->package);
    }

    FunctionReturnList* return_list = func->return_list;
    for (; return_list != nullptr; return_list = return_list->next) {
        fix_type_specfier(return_list->type_specifier, func->package);
    }
}


void fix_function_block(Function* func) {
    if (func->block) {
        add_parameter_to_declaration(func->parameter_list, func->block);
        fix_statement_list(func->block->statement_list, func->block, (FunctionTuple*)func);
    }
}

void fix_statement_list(Statement* statement_list, Block* block, FunctionTuple* func) {
    if (statement_list == nullptr) {
        return;
    }

    for (Statement* pos = statement_list; pos; pos = pos->next) {
        fix_statement(pos, block, func);
    }
}

void fix_statement(Statement* statement, Block* block, FunctionTuple* func) {
    switch (statement->type) {
    case STATEMENT_TYPE_EXPRESSION:
        fix_expression(statement->u.expression, block, func, RingContext{});
        break;
    case STATEMENT_TYPE_VAR_DECL:
        add_local_declaration(statement->u.var_decl_statement, block, func);
        break;

    case STATEMENT_TYPE_IF:
        fix_if_statement(statement->u.if_statement, block, func);
        break;

    case STATEMENT_TYPE_FOR:
        fix_for_statement(statement->u.for_statement, block, func);
        break;

    case STATEMENT_TYPE_DOFOR:
        fix_dofor_statement(statement->u.dofor_statement, block, func);
        break;

    case STATEMENT_TYPE_BREAK:
        fix_break_statement(statement->u.break_statement, block, func);
        break;

    case STATEMENT_TYPE_CONTINUE:
        fix_continue_statement(statement->u.continue_statement, block, func);
        break;

    case STATEMENT_TYPE_RETURN:
        fix_return_statement(statement->u.return_statement, block, func);
        break;

    case STATEMENT_TYPE_TAG_DEFINITION:
        break;

    case STATEMENT_TYPE_JUMP_TAG:
        break;

    case STATEMENT_TYPE_DEFER:
        fix_defer_statement(statement->u.defer_statement, block, func);
        break;

    default: break;
    }
}

// fix_expression 会根据 next 指针递归向下遍历
void fix_expression(Expression* expression,
                    Block* block, FunctionTuple* func,
                    RingContext ctx) {

BEGIN:
    if (expression == nullptr) {
        return;
    }
    switch (expression->type) {

    case EXPRESSION_TYPE_LITERAL_BOOL:
        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        EXPRESSION_ADD_CONVERT_TYPE(expression, &bool_type_specifier);
        break;
    case EXPRESSION_TYPE_LITERAL_INT:
        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        EXPRESSION_ADD_CONVERT_TYPE(expression, &int_type_specifier);
        break;
    case EXPRESSION_TYPE_LITERAL_INT64:
        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        EXPRESSION_ADD_CONVERT_TYPE(expression, &int64_type_specifier);
        break;
    case EXPRESSION_TYPE_LITERAL_DOUBLE:
        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        EXPRESSION_ADD_CONVERT_TYPE(expression, &double_type_specifier);
        break;
    case EXPRESSION_TYPE_LITERAL_STRING:
        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        EXPRESSION_ADD_CONVERT_TYPE(expression, &string_type_specifier);
        break;

    case EXPRESSION_TYPE_VARIABLE:
        break;

    case EXPRESSION_TYPE_IDENTIFIER:
        fix_identifier_expression(expression, expression->u.identifier_expression, block, ctx);
        break;

    case EXPRESSION_TYPE_FUNCTION_CALL:
        fix_function_call_expression(expression, expression->u.function_call_expression, block, func);
        break;

    case EXPRESSION_TYPE_MEMBER_CALL:
        fix_member_call_expression(expression, expression->u.member_call_expression, block, func);
        break;

    case EXPRESSION_TYPE_ASSIGN:
        fix_assign_expression(expression->u.assign_expression, block, func);
        break;

    case EXPRESSION_TYPE_TERNARY:
        fix_ternary_condition_expression(expression, expression->u.ternary_expression, block, func);
        break;

    case EXPRESSION_TYPE_LAUNCH:
        fix_launch_expression(expression, expression->u.launch_expression, block, func);
        break;

    case EXPRESSION_TYPE_ANONYMOUS_FUNC:
        fix_anonymous_func_expression(expression, expression->u.anonymous_func_expression, block, func);
        break;

    case EXPRESSION_TYPE_IIFE:
        fix_iife_expression(expression, expression->u.iife, block, func);
        break;


    case EXPRESSION_TYPE_ARITHMETIC_ADD:
    case EXPRESSION_TYPE_ARITHMETIC_SUB:
    case EXPRESSION_TYPE_ARITHMETIC_MUL:
    case EXPRESSION_TYPE_ARITHMETIC_DIV:
    case EXPRESSION_TYPE_ARITHMETIC_MOD:
        fix_binary_math_expression(expression, expression->type, expression->u.binary_expression, block, func, ctx);
        break;
    case EXPRESSION_TYPE_LOGICAL_AND:
    case EXPRESSION_TYPE_LOGICAL_OR:
        fix_binary_logical_expression(expression, expression->type, expression->u.binary_expression, block, func, ctx);
        break;
    case EXPRESSION_TYPE_RELATIONAL_EQ:
    case EXPRESSION_TYPE_RELATIONAL_NE:
    case EXPRESSION_TYPE_RELATIONAL_GT:
    case EXPRESSION_TYPE_RELATIONAL_GE:
    case EXPRESSION_TYPE_RELATIONAL_LT:
    case EXPRESSION_TYPE_RELATIONAL_LE:
        fix_binary_relational_expression(expression, expression->type, expression->u.binary_expression, block, func, ctx);
        break;

    case EXPRESSION_TYPE_BITWISE_UNITARY_NOT:
        fix_bitwise_unitary_not_expression(expression, expression->u.unitary_expression, block, func, ctx);
        break;
    case EXPRESSION_TYPE_BITWISE_AND:
    case EXPRESSION_TYPE_BITWISE_OR:
    case EXPRESSION_TYPE_BITWISE_XOR:
    case EXPRESSION_TYPE_BITWISE_LSH:
    case EXPRESSION_TYPE_BITWISE_RSH:
        fix_bitwise_binary_expression(expression, expression->type, expression->u.binary_expression, block, func, ctx);
        break;


    case EXPRESSION_TYPE_ARITHMETIC_UNITARY_MINUS:
        fix_unitary_minus_expression(expression, expression->u.unitary_expression, block, func, ctx);
        break;
    case EXPRESSION_TYPE_LOGICAL_UNITARY_NOT:
        fix_unitary_not_expression(expression, expression->u.unitary_expression, block, func, ctx);
        break;
    case EXPRESSION_TYPE_UNITARY_INCREASE:
    case EXPRESSION_TYPE_UNITARY_DECREASE:
        fix_unitary_increase_decrease_expression(expression, expression->u.unitary_expression, block, func);
        break;


    case EXPRESSION_TYPE_NEW_ARRAY:
        fix_new_array_expression(expression, expression->u.new_array_expression, block, func);
        break;
    case EXPRESSION_TYPE_ARRAY_LITERAL:
        fix_array_literal_expression(expression, expression->u.array_literal_expression, block, func);
        break;
    case EXPRESSION_TYPE_CLASS_OBJECT_LITERAL:
        fix_class_object_literal_expression(expression, expression->u.class_object_literal_expression, block, func);
        break;


    case EXPRESSION_TYPE_ARRAY_INDEX:
        fix_array_index_expression(expression, expression->u.array_index_expression, block, func);
        break;
    case EXPRESSION_TYPE_SLICE:
        fix_slice_expression(expression, expression->u.slice_expression, block, func);
        break;
    case EXPRESSION_TYPE_MEMBER:
        fix_field_member_expression(expression, expression->u.member_expression, block, func);
        break;

    case EXPRESSION_TYPE_ELEMENT_ACCESS:
        ring_error_report("not implemented");
        break;

    case EXPRESSION_TYPE_CAST:
        fix_expression(expression->u.cast_expression->operand, block, func, ctx);
        break;

    default: break;
    }

    expression = expression->next;
    goto BEGIN;
}

void add_local_declaration(VarDecl* declaration, Block* block, FunctionTuple* func) {
    assert(declaration != nullptr);
    assert(block != nullptr);

    VarDecl* decl_pos  = declaration;
    VarDecl* decl_next = decl_pos->next;
    for (; decl_pos != nullptr; decl_pos = decl_next) {
        decl_next      = decl_pos->next;
        decl_pos->next = nullptr;

        // fix type specifier
        fix_type_specfier(decl_pos->type_specifier, block->package_unit->parent_package);


        // 添加局部变量
        block->var_decl_list =
            declaration_list_add_item(block->var_decl_list, decl_pos);

        decl_pos->variable_index = block->var_decl_list_size++;
        decl_pos->is_local       = 1;

        // Ring-Compiler-Error-Report ERROR_TOO_MANY_LOCAL_VARIABLE
        if (block->var_decl_list_size > 255) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "the number of local variable is greater than 255 in this block; E:%d.",
                ERROR_TOO_MANY_LOCAL_VARIABLES);
            compile_adv_buf = sprintf_string(
                "delete useless local variable in this block.");

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(declaration->line_number),
                .line_number             = declaration->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }
    }
}

void fix_enum_declaration(EnumDeclaration* declaration, Package* curr_package, RingContext ctx) {
    ctx.optimize_level = OPTIMIZE_LEVEL_1; // 强制开启优化

    // 必须是基础类型
    // Ring-Compiler-Error-Report ERROR_NOT_SUPPORT_ENUM_VALUE
    if (!TYPE_IS_BASIC(declaration->type_specifier)) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "enum `%s` only support basic type bool/int/int64/double/string; E:%d.",
            declaration->identifier,
            ERROR_MISS_CLASS_DEFINITION);


        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(declaration->start_line_number),
            .line_number             = declaration->start_line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    EnumItemDeclaration* pos   = declaration->enum_item_list;
    unsigned int         index = 0;
    for (; pos != nullptr; pos = pos->next, index++) {
        pos->index_of_enum = index;

        fix_expression(pos->value_expr, nullptr, nullptr, ctx);

        // Ring-Compiler-Error-Report ERROR_NOT_SUPPORT_ENUM_VALUE
        if (!EXPR_IS_BASIC_LITERAL(pos->value_expr->type)) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "enum `%s` invalid:only basic literal is allowed as enum value; E:%d.",
                pos->identifier,
                ERROR_MISS_CLASS_DEFINITION);


            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(pos->line_number),
                .line_number             = pos->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }
    }
}

// curr_package 为定义/使用 TypeSpecifier 所在的Package
void fix_type_specfier(TypeSpecifier* type_specifier, Package* curr_package) {
    if (type_specifier == nullptr) {
        return;
    }


    TypeAlias* type_alias = nullptr;

    // e.g. var a tmp;
    // 1. a 是一个类-类型, 从全局类定义中搜索
    // 2. a 是一个匿名函数类型, 从全局类型定义中搜索
    if (type_specifier->kind == RING_BASIC_TYPE_UNKNOW) {

        assert(type_specifier->identifier != nullptr);

        // step-2. 是个函数别名
        Package* package = resolve_package(type_specifier->package_posit, type_specifier->line_number, nullptr, curr_package, RingContext{});
        type_alias       = search_type_alias(package, type_specifier->identifier);
        if (type_alias != nullptr) {
            // shallow copy
            type_specifier->kind = type_alias->type_specifier->kind;
            type_specifier->u    = type_alias->type_specifier->u;
            goto END;
        }

        // Ring-Compiler-Error-Report ERROR_MISS_CLASS_DEFINITION
        if (true) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "miss type `%s` definition; E:%d.",
                type_specifier->identifier,
                ERROR_MISS_CLASS_DEFINITION);
            compile_adv_buf = sprintf_string(
                "definite type `%s` before use it.",
                type_specifier->identifier);


            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(type_specifier->line_number),
                .line_number             = type_specifier->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }
    }


END:

    // 递归修正数组
    // 其实修正递归数组，只会对 class-object的数组生效
    // TODO: 这里递归修正数组，有可能浪费时间
    if (type_specifier->kind == RING_BASIC_TYPE_ARRAY) {
        fix_type_specfier(type_specifier->u.array_t->sub, curr_package);
    }

    if (type_specifier->kind == RING_BASIC_TYPE_FUNC) {
        Ring_DeriveType_Func* func_type = type_specifier->u.func_t;

        for (Parameter* param_pos = func_type->parameter_list;
             param_pos != nullptr;
             param_pos = param_pos->next) {
            fix_type_specfier(param_pos->type_specifier, curr_package);
        }


        for (FunctionReturnList* return_pos = func_type->return_list;
             return_pos != nullptr;
             return_pos = return_pos->next) {
            fix_type_specfier(return_pos->type_specifier, curr_package);
        }
    }
}

void fix_block(Block* block, FunctionTuple* func) {
    if (block == nullptr) {
        return;
    }
    fix_statement_list(block->statement_list, block, func);
}

void fix_if_statement(IfStatement* if_statement, Block* block, FunctionTuple* func) {

    fix_expression(if_statement->condition_expression, block, func, RingContext{});

    if (global_ring_command_arg.optimize_level > 0) {
        crop_if_statement(if_statement, block, func);
    }

    fix_block(if_statement->if_block, func);
    fix_block(if_statement->else_block, func);

    ElseIfStatement* pos = if_statement->elseif_list;
    for (; pos; pos = pos->next) {
        fix_expression(pos->condition_expression, block, func, RingContext{});
        fix_block(pos->elseif_block, func);
    }
}

void fix_for_statement(ForStatement* for_statement, Block* block, FunctionTuple* func) {
    if (for_statement == nullptr) {
        return;
    }

    if (for_statement->type == FOR_STATEMENT_TYPE_TERNARY) {
        ForTernaryStatement* ternary_statement = for_statement->u.ternary_statement;

        fix_expression(ternary_statement->init_expression, block, func, RingContext{});
        fix_expression(ternary_statement->condition_expression, block, func, RingContext{});
        fix_expression(ternary_statement->post_expression, block, func, RingContext{});
    } else if (for_statement->type == FOR_STATEMENT_TYPE_RANGE) {
        fix_for_range_statement(for_statement->u.range_statement, block, func);
    }

    if (global_ring_command_arg.optimize_level > 0) {
        crop_for_statement(for_statement, block, func);
    }


    fix_block(for_statement->block, func);
}

void fix_for_range_statement(ForRangeStatement* for_range_statement,
                             Block*             block,
                             FunctionTuple*     func) {

    Expression*      left       = for_range_statement->left;
    RangeExpression* range_expr = for_range_statement->range_expr;

    fix_expression(left, block, func, RingContext{});
    fix_range_expression(range_expr, block, func);

    std::vector<TypeSpecifier*> left_types  = get_type_specifier_from_expression_list(left);
    std::vector<TypeSpecifier*> right_types = get_type_specifier_from_range_expression(range_expr);

    // 赋值操作 left/operand 数量不匹配
    // Ring-Compiler-Error-Report ERROR_ASSIGNMENT_MISMATCH_NUM
    if (for_range_statement->range_expr->type == RANGE_EXPRESSION_TYPE_LINEAR
        && left_types.size() == 1 && right_types.size() == 2) {
        // TODO: 兼容逻辑，后续处理掉
        // 对于历史的语法做一下
        // for value = range array{}
        // 不然会报错
    } else if (left_types.size() != right_types.size()) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "assignment mismatch: %lu variables not match %lu operands; E:%d.",
            left_types.size(),
            right_types.size(),
            ERROR_ASSIGNMENT_MISMATCH_NUM);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(left->line_number),
            .line_number             = left->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    if (left_types.size() == right_types.size()) {
        // TODO: 兼容逻辑，后续处理掉

        for (unsigned int i = 0; i < left_types.size(); i++) {
            // 深度比较两个类型是否匹配
            // Ring-Compiler-Error-Report ERROR_RANGE_ASSIGN_MISSMATCH_TYPE
            if (!comp_type_specifier(left_types[i], right_types[i])) {
                DEFINE_ERROR_REPORT_STR;

                compile_err_buf = sprintf_string(
                    "range mismatch: range return (%s) but assign to (%s); E:%d.",
                    format_type_specifier(right_types).c_str(),
                    format_type_specifier(left_types).c_str(),
                    ERROR_RANGE_ASSIGN_MISSMATCH_TYPE);

                ErrorReportContext context = {
                    .package                 = nullptr,
                    .package_unit            = get_package_unit(),
                    .source_file_name        = get_package_unit()->current_file_name,
                    .line_content            = package_unit_get_line_content(left->line_number),
                    .line_number             = left->line_number,
                    .column_number           = package_unit_get_column_number(),
                    .error_message           = std::string(compile_err_buf),
                    .advice                  = std::string(compile_adv_buf),
                    .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                    .ring_compiler_file      = (char*)__FILE__,
                    .ring_compiler_file_line = __LINE__,
                };
                ring_compile_error_report(&context);
            }
        }
    }
}


void fix_range_expression(RangeExpression* range_expression,
                          Block*           block,
                          FunctionTuple*   func) {

    assert(range_expression != nullptr);


    // TODO: 需要继续拆分
    if (range_expression->type == RANGE_EXPRESSION_TYPE_STEP) {
        fix_expression(range_expression->u.sub_step_range_expr->start_expr, block, func, RingContext{});
        fix_expression(range_expression->u.sub_step_range_expr->end_expr, block, func, RingContext{});
        if (range_expression->u.sub_step_range_expr->step_expr != nullptr) {
            fix_expression(range_expression->u.sub_step_range_expr->step_expr, block, func, RingContext{});
        }

        // 判断类型
        // TODO: 类型必须是 int/int64/double
        // TODO: check convert_type size
        if (range_expression->u.sub_step_range_expr->start_expr->convert_type_size != 1) {
            // error report
        }
        if (range_expression->u.sub_step_range_expr->end_expr->convert_type_size != 1) {
            // error report
        }

        TypeSpecifier* start_expr_type = range_expression->u.sub_step_range_expr->start_expr->convert_type[0];
        TypeSpecifier* end_expr_type   = range_expression->u.sub_step_range_expr->end_expr->convert_type[0];

        // 适配不同的step int64 or double
        // TODO: 这里是否要考虑 step_expr 的类型
        EXPRESSION_CLEAR_CONVERT_TYPE(range_expression);
        if (TYPE_IS_DOUBLE(start_expr_type) || TYPE_IS_DOUBLE(end_expr_type)) {
            range_expression->u.sub_step_range_expr->value_type = RING_BASIC_TYPE_DOUBLE;
            EXPRESSION_ADD_CONVERT_TYPE(range_expression, &double_type_specifier);
        } else if (TYPE_IS_INT64(start_expr_type) || TYPE_IS_INT64(end_expr_type)) {
            range_expression->u.sub_step_range_expr->value_type = RING_BASIC_TYPE_INT64;
            EXPRESSION_ADD_CONVERT_TYPE(range_expression, &int64_type_specifier);
        } else {
            range_expression->u.sub_step_range_expr->value_type = RING_BASIC_TYPE_INT64;
            EXPRESSION_ADD_CONVERT_TYPE(range_expression, &int_type_specifier);
        }


        // TODO:
    } else if (range_expression->type == RANGE_EXPRESSION_TYPE_LINEAR) {
        fix_expression(range_expression->u.sub_linear_range_expr->collection_expr, block, func, RingContext{});

        // TODO: 还需要判断 size
        // range_expression->u.sub_linear_range_expr->collection_expr->convert_type_size

        TypeSpecifier* type_specifier = range_expression->u.sub_linear_range_expr->collection_expr->convert_type[0];
        // Ring-Compiler-Error-Report ERROR_RANGE_OPERAND_IS_NOT_ARRAY
        if (!TYPE_IS_ARRAY(type_specifier)) {
            DEFINE_ERROR_REPORT_STR;
            compile_err_buf = sprintf_string(
                "range operand is not an array; E:%d.",
                ERROR_RANGE_OPERAND_IS_NOT_ARRAY);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(range_expression->line_number),
                .line_number             = range_expression->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }

        TypeSpecifier* item_type = (TypeSpecifier*)mem_alloc(get_front_mem_pool(), sizeof(TypeSpecifier));
        item_type->line_number   = range_expression->line_number;

        // TODO: fix_array_index_expression 与 fix_range_expression 抽象出公共的方法
        if (type_specifier->u.array_t->dimension > 1) {
            item_type->kind                 = type_specifier->kind;
            item_type->u.array_t            = (Ring_DeriveType_Array*)mem_alloc(get_front_mem_pool(), sizeof(Ring_DeriveType_Array));
            item_type->u.array_t->dimension = type_specifier->u.array_t->dimension - 1;
            item_type->u.array_t->sub       = type_specifier->u.array_t->sub;
        } else {
            // item_type 是基础类型
            item_type->kind = type_specifier->u.array_t->sub->kind;
        }

        // shallow copy
        if (item_type->kind == RING_BASIC_TYPE_CLASS) {
            item_type->u.class_t = type_specifier->u.array_t->sub->u.class_t;
        } else if (item_type->kind == RING_BASIC_TYPE_FUNC) {
            item_type->u.func_t = type_specifier->u.array_t->sub->u.func_t;
        } else {
            // 上面已经处理过了
        }
        fix_type_specfier(item_type, block->package_unit->parent_package);

        EXPRESSION_CLEAR_CONVERT_TYPE(range_expression);
        EXPRESSION_ADD_CONVERT_TYPE(range_expression, &int64_type_specifier);
        EXPRESSION_ADD_CONVERT_TYPE(range_expression, item_type);
    }
}

void fix_dofor_statement(DoForStatement* dofor_statement, Block* block, FunctionTuple* func) {
    if (dofor_statement == nullptr) {
        return;
    }

    fix_expression(dofor_statement->init_expression, block, func, RingContext{});
    fix_block(dofor_statement->block, func);
    fix_expression(dofor_statement->condition_expression, block, func, RingContext{});
    fix_expression(dofor_statement->post_expression, block, func, RingContext{});
}

void fix_break_statement(BreakStatement* break_statement, Block* block, FunctionTuple* func) {
    if (break_statement == nullptr) {
        return;
    }

    unsigned int break_loop_num = break_statement->break_loop_num;

    // 一层一层的往上找
    // 直到找到 break_loop_num 层对应的 for循环
    Block*       pos      = block;
    unsigned int loop_num = 1;
    for (; pos; pos = pos->parent_block) {

        if (pos->type != BLOCK_TYPE_FOR && pos->type != BLOCK_TYPE_DOFOR) {
            continue;
        }

        if (break_loop_num == loop_num) {
            break;
        }
        loop_num++;
    }


    // 找不到对应的跳转block
    // Ring-Compiler-Error-Report ERROR_INVALID_BREAK_STATEMENT
    if (pos == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid break statement, can't find break jump block; E:%d.",
            ERROR_INVALID_BREAK_STATEMENT);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(break_statement->line_number),
            .line_number             = break_statement->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    break_statement->jump_to_block = pos;
}

void fix_continue_statement(ContinueStatement* continue_statement, Block* block, FunctionTuple* func) {
    if (continue_statement == nullptr) {
        return;
    }

    Block* pos = block;
    for (; pos; pos = pos->parent_block) {
        if (pos->type == BLOCK_TYPE_FOR || pos->type == BLOCK_TYPE_DOFOR) {
            break;
        }
    }


    // 找不到对应的跳转block
    // Ring-Compiler-Error-Report ERROR_INVALID_CONTINUE_STATEMENT
    if (pos == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid continue statement, can't find continue jump block; E:%d.",
            ERROR_INVALID_CONTINUE_STATEMENT);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(continue_statement->line_number),
            .line_number             = continue_statement->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    continue_statement->jump_to_block = pos;
}

void fix_return_statement(ReturnStatement* return_statement, Block* block, FunctionTuple* func) {
    if (return_statement == nullptr) {
        return;
    }


    bool                        has_call = false;

    std::vector<TypeSpecifier*> return_convert_type;
    unsigned                    return_exp_num = 0;

    for (Expression* pos = return_statement->return_list;
         pos != nullptr;
         pos = pos->next, return_exp_num++) {

        if (pos->type == EXPRESSION_TYPE_FUNCTION_CALL
            || pos->type == EXPRESSION_TYPE_MEMBER_CALL) {
            has_call = true;
        }

        fix_expression(pos, block, func, RingContext{});

        // 函数调用有可能有多个返回值
        for (unsigned int i = 0; i < pos->convert_type_size; i++) {
            return_convert_type.push_back(pos->convert_type[i]);
        }
    }


    // check return语句 中表达式的类型 和 function_definition 中的 返回值类型 是否一致

    // Ring-Compiler-Error-Report ERROR_FUNCTION_CALL_IN_MULTIPLE_OPERANDS
    // operand中有多个, 其中有 function_call, 这是不合法的
    if (return_exp_num > 1 && has_call) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "function-call/method-call is not allowed in multiple operands; E:%d.",
            ERROR_FUNCTION_CALL_IN_MULTIPLE_OPERANDS);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(return_statement->line_number),
            .line_number             = return_statement->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    std::string expect_type_str;
    // formate to string
    {
        std::vector<TypeSpecifier*> tmp;
        for (FunctionReturnList* pos = func->return_list;
             pos != nullptr;
             pos = pos->next) {

            tmp.push_back(pos->type_specifier);
        }
        expect_type_str = format_type_specifier(tmp);
    }

    std::string return_type_str;
    return_type_str = format_type_specifier(return_convert_type);


    // Ring-Compiler-Error-Report ERROR_FUNCTION_MISMATCH_RETURN_NUM
    if (func->return_list_size != return_convert_type.size()) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "return mismatch: function expect (%s) but return (%s); E:%d.",
            expect_type_str.c_str(),
            return_type_str.c_str(),
            ERROR_FUNCTION_MISMATCH_RETURN_NUM);


        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(return_statement->line_number),
            .line_number             = return_statement->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    // Ring-Compiler-Error-Report ERROR_FUNCTION_MISMATCH_RETURN_TYPE
    FunctionReturnList* return_value = func->return_list;
    unsigned int        i            = 0;
    for (; return_value != nullptr;
         return_value = return_value->next, i++) {

        TypeSpecifier* expect = return_value->type_specifier;
        TypeSpecifier* actual = return_convert_type[i];

        // return 语句是否和 函数定义中的返回值是否一致
        if (!comp_type_specifier(expect, actual)) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "return mismatch: function expect (%s) but return (%s); E:%d.",
                expect_type_str.c_str(),
                return_type_str.c_str(),
                ERROR_FUNCTION_MISMATCH_RETURN_TYPE);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(return_statement->line_number),
                .line_number             = return_statement->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }
    }
}

void fix_defer_statement(DeferStatement* defer_statement, Block* block, FunctionTuple* func) {

    fix_iife_expression(nullptr, defer_statement->iife, block, func);
}

void fix_identifier_expression(Expression*           expression,
                               IdentifierExpression* identifier_expression,
                               Block*                block,
                               RingContext           ctx) {

    assert(identifier_expression != nullptr);

    // 一个identifier 有可能是 一个变量，也有可能是函数
    Variable*        variable         = nullptr;
    Function*        function         = nullptr;
    TypeSpecifier*   type_specifier   = nullptr;

    EnumDeclaration* enum_declaration = nullptr;
    enum_declaration                  = resolve_enum_decl(identifier_expression->path_segment, block, nullptr);
    if (enum_declaration != nullptr) {
        EnumItemDeclaration* enum_item = resolve_enum_item(enum_declaration, identifier_expression->identifier);
        // Ring-Compiler-Error-Report ERROR_NOT_FOUND_ENUM
        if (enum_item == nullptr) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "not found `%s` in enum `%s`; E:%d.",
                identifier_expression->identifier,
                identifier_expression->path_segment,
                ERROR_NOT_FOUND_ENUM);


            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(identifier_expression->line_number),
                .line_number             = identifier_expression->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
            return;
        }

        assert(enum_item->value_expr->type == EXPRESSION_TYPE_LITERAL_BOOL
               || enum_item->value_expr->type == EXPRESSION_TYPE_LITERAL_INT
               || enum_item->value_expr->type == EXPRESSION_TYPE_LITERAL_INT64
               || enum_item->value_expr->type == EXPRESSION_TYPE_LITERAL_DOUBLE
               || enum_item->value_expr->type == EXPRESSION_TYPE_LITERAL_STRING);
        // TODO: error-report 必须在编译期间确定
        // TODO: -O1 优化 常量折叠

        identifier_expression->type        = IDENTIFIER_EXPRESSION_TYPE_ENUM_ITEM;
        identifier_expression->u.enum_item = enum_item;

        type_specifier                     = enum_declaration->type_specifier;

        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        EXPRESSION_ADD_CONVERT_TYPE(expression, type_specifier);

        return;
    }

    Package* package = nullptr;
    package          = resolve_package(identifier_expression->path_segment, identifier_expression->line_number, block, nullptr, ctx);
    //
    variable = resolve_variable(package, identifier_expression->identifier, block);
    if (variable != nullptr) {
        // is a variable
        identifier_expression->type       = IDENTIFIER_EXPRESSION_TYPE_VARIABLE;
        identifier_expression->u.variable = variable;
        type_specifier                    = variable->decl->type_specifier;

        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        EXPRESSION_ADD_CONVERT_TYPE(expression, type_specifier);
        return;
    }

    function = search_function(package, identifier_expression->identifier);
    if (function != nullptr) {
        // is a function
        identifier_expression->type       = IDENTIFIER_EXPRESSION_TYPE_FUNC;
        identifier_expression->u.function = function;
        type_specifier                    = &func_type_specifier;

        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        EXPRESSION_ADD_CONVERT_TYPE(expression, type_specifier);
        return;
    }

    // not a variable and not a function
    // Ring-Compiler-Error-Report ERROR_UNDEFINITE_VARIABLE
    {
        DEFINE_ERROR_REPORT_STR;
        compile_err_buf = sprintf_string(
            "use undeclared identifier `%s`; E:%d.",
            identifier_expression->identifier,
            ERROR_UNDEFINITE_VARIABLE);
        compile_adv_buf = sprintf_string(
            "definite variable `%s` like: `var bool|int|double|string %s;` before use it.",
            identifier_expression->identifier,
            identifier_expression->identifier);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = nullptr,
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(identifier_expression->line_number),
            .line_number             = identifier_expression->line_number,
            .column_number           = 0,
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_EXIT_NOW,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }
}

/*
 * fix_assign_expression 主要对 assignment 语句进行语义检查和AST的修正
 *
 * 1. 检查 left 和 right 的数量是否匹配
 * 2. 检查 left 和 right 的类型是否匹配
 * 3. assignment 是多项赋值语句的时候, 情况复杂了
 *    3.1 如果 right 是function-call, 并且这个函数有多个返回值, 那么: 在 right中 只能存在 这个function-call
 *        a,b,c = test(); // 合法的, 因为test()返回值作为一个整体, 可以继续展开成 三个表达式
 *        a,b,c,c = test(), 1; // 不合法的, 因为test()返回值作为一个整体, 是不能继续展开的, 只能是一个表达式
 */
void fix_assign_expression(AssignExpression* expression, Block* block, FunctionTuple* func) {
    if (expression == nullptr) {
        return;
    }

    unsigned int                left_expr_num = 0;
    std::vector<TypeSpecifier*> left_convert_type;
    // 对于 left, left_expr_num 和 left_convert_type.size() 是相同的
    // 因为没有 一个表达式代表两个返回值的情况（没有函数调用）
    // TODO: 后续修改为 get_type_specifier_from_expression_list
    for (Expression* pos = expression->left; pos; pos = pos->next, left_expr_num++) {
        // FIXME: 如果 pos->next != nullptr, 会重复修正，耗时增加
        fix_expression(pos, block, func, RingContext{});
        left_convert_type.push_back(pos->convert_type[0]);
    }
    assert(left_expr_num == left_convert_type.size());

    bool                        has_call        = false;
    Expression*                 call_expression = nullptr;

    unsigned int                right_expr_num  = 0;
    std::vector<TypeSpecifier*> right_convert_type;
    // 对于 left, 如果某个表达式是个 function-call, 并且有多个返回值, right_expr_num 和 right_convert_type.size() 是不相同的
    // TODO: 后续修改为 get_type_specifier_from_expression_list
    for (Expression* pos = expression->operand; pos; pos = pos->next, right_expr_num++) {

        if (pos->type == EXPRESSION_TYPE_FUNCTION_CALL
            || pos->type == EXPRESSION_TYPE_MEMBER_CALL) {
            call_expression = pos;
            has_call        = true;
        }

        // FIXME: 如果 pos->next != nullptr, 会重复修正，耗时增加
        fix_expression(pos, block, func, RingContext{});

        for (unsigned int i = 0; i < pos->convert_type_size; i++) {
            right_convert_type.push_back(pos->convert_type[i]);
        }
    }


    if (left_expr_num > 1 || right_expr_num > 1) {
        expression->type = ASSIGN_EXPRESSION_TYPE_MULTI_ASSIGN;
    }

    // operand中有多个, 其中有 function_call, 这是不合法的
    // Ring-Compiler-Error-Report ERROR_FUNCTION_CALL_IN_MULTIPLE_OPERANDS
    if (right_expr_num > 1 && has_call) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "function-call/method-call is not allowed in multiple operands; E:%d.",
            ERROR_FUNCTION_CALL_IN_MULTIPLE_OPERANDS);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(call_expression->line_number),
            .line_number             = call_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    // 赋值操作 left/operand 数量不匹配
    // Ring-Compiler-Error-Report ERROR_ASSIGNMENT_MISMATCH_NUM
    if (left_convert_type.size() != right_convert_type.size()) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "assignment mismatch: %lu variables not match %lu operands; E:%d.",
            left_convert_type.size(),
            right_convert_type.size(),
            ERROR_ASSIGNMENT_MISMATCH_NUM);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(expression->line_number),
            .line_number             = expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    std::string left_type_s;
    {
        // formate left to string
        // TODO: 转化成这个函数 std::string format_type_specifier(std::vector<TypeSpecifier*> convert_type)
        std::vector<std::string> tmp;
        for (TypeSpecifier* type : left_convert_type) {
            tmp.push_back(format_type_specifier(type));
        }
        left_type_s = "(" + strings_join(tmp, ",") + ")";
    }


    std::string right_type_s;
    {
        // formate right to string
        // TODO: 转化成这个函数 std::string format_type_specifier(std::vector<TypeSpecifier*> convert_type)
        std::vector<std::string> tmp;
        for (TypeSpecifier* type : right_convert_type) {
            tmp.push_back(format_type_specifier(type));
        }
        right_type_s = "(" + strings_join(tmp, ",") + ")";
    }


    // 赋值操作 left/operand 类型不匹配
    // 这里先不考虑可变类型参数，比较复杂
    // Ring-Compiler-Error-Report ERROR_ASSIGNMENT_MISMATCH_TYPE
    for (unsigned int i = 0; i < left_convert_type.size(); i++) {

        // 深度比较两个类型是否匹配
        if (!comp_type_specifier(left_convert_type[i], right_convert_type[i])) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "assignment mismatch: expect %s but provided %s; E:%d.",
                left_type_s.c_str(),
                right_type_s.c_str(),
                ERROR_ASSIGNMENT_MISMATCH_TYPE);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(expression->line_number),
                .line_number             = expression->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }
    }
}


/*
 * fix_binary_concat_expression
 * 修正二元操作符 `+` 的语义检查, 字符串的拼接
 */
void fix_binary_concat_expression(Expression*       expression,
                                  BinaryExpression* binary_expression,
                                  Block* block, FunctionTuple* func) {

    assert(expression != nullptr);
    assert(binary_expression != nullptr);

    Expression* left  = binary_expression->left_expression;
    Expression* right = binary_expression->right_expression;

    fix_expression(left, block, func, RingContext{});
    fix_expression(right, block, func, RingContext{});

    // 上一层函数一定判断数组了，这里不用判断
    TypeSpecifier* left_type  = left->convert_type[0];
    TypeSpecifier* right_type = right->convert_type[0];


    // Ring-Compiler-Error-Report ERROR_CONCAT_OPERATOR_MISMATCH_TYPE
    if (!TYPE_IS_STRING(left_type)
        || !TYPE_IS_STRING(right_type)) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "Type error: string concat operator `+` can only be applied to string; E:%d.",
            ERROR_CONCAT_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(left->line_number),
            .line_number             = left->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, &string_type_specifier);

    if (global_ring_command_arg.optimize_level > 0) {
        crop_binary_concat_expression(expression, binary_expression, block, func);
    }
}

void fix_binary_math_expression(Expression*       expression,
                                ExpressionType    expression_type,
                                BinaryExpression* binary_expression,
                                Block* block, FunctionTuple* func,
                                RingContext ctx) {

    assert(expression != nullptr);
    assert(binary_expression != nullptr);

    Expression* left  = binary_expression->left_expression;
    Expression* right = binary_expression->right_expression;

    std::string oper  = formate_operator(expression_type);

    fix_expression(left, block, func, RingContext{});
    fix_expression(right, block, func, RingContext{});

    // 检查两边操作数的合法性
    if (left->convert_type_size != 1
        || left->convert_type == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: oper `%s`, `%s` return no value; E:%d.",
            oper.c_str(),
            formate_expression(left).c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(left->line_number),
            .line_number             = left->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    if (right->convert_type_size != 1
        || right->convert_type == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: oper `%s`, `%s` return no value; E:%d.",
            oper.c_str(),
            formate_expression(right).c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(right->line_number),
            .line_number             = right->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    TypeSpecifier* left_type  = left->convert_type[0];
    TypeSpecifier* right_type = right->convert_type[0];

    // 是否为字符串拼接运算
    if (expression_type == EXPRESSION_TYPE_ARITHMETIC_ADD
        && (TYPE_IS_STRING(left_type) || TYPE_IS_STRING(right_type))) {
        fix_binary_concat_expression(expression, binary_expression, block, func);
        return;
    }


    if (!TYPE_IS_NUM(left_type)) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: oper `%s`, invalid operand `%s`,only apply int/int64/double to math operator; E:%d.",
            oper.c_str(),
            formate_expression(left).c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(left->line_number),
            .line_number             = left->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    if (!TYPE_IS_NUM(right_type)) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: oper `%s`, invalid `%s`,only apply int/int64/double to math operator; E:%d.",
            oper.c_str(),
            formate_expression(right).c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(right->line_number),
            .line_number             = right->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    if (left_type->kind != right_type->kind) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: oper `%s`, `%s` is %s, `%s` is %s, type mismatch; E:%d.",
            oper.c_str(),
            formate_expression(left).c_str(),
            format_type_specifier(left_type).c_str(),
            formate_expression(right).c_str(),
            format_type_specifier(right_type).c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(binary_expression->line_number),
            .line_number             = binary_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    // TODO: 这里浪费空间了
    if (expression->convert_type == nullptr) {
        TypeSpecifier* convert_type = (TypeSpecifier*)mem_alloc(get_front_mem_pool(), sizeof(TypeSpecifier));
        convert_type->kind          = RING_BASIC_TYPE_UNKNOW;
        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        EXPRESSION_ADD_CONVERT_TYPE(expression, convert_type);
    }

    if (left_type->kind == RING_BASIC_TYPE_INT) {
        expression->convert_type[0]->kind = RING_BASIC_TYPE_INT;
    } else if (left_type->kind == RING_BASIC_TYPE_INT64) {
        expression->convert_type[0]->kind = RING_BASIC_TYPE_INT64;
    } else if (left_type->kind == RING_BASIC_TYPE_DOUBLE) {
        expression->convert_type[0]->kind = RING_BASIC_TYPE_DOUBLE;
    }

    if (ctx.optimize_level > 0) {
        crop_binary_match_expression(expression, binary_expression, block, func);
    }
}

void fix_binary_logical_expression(Expression*       expression,
                                   ExpressionType    expression_type,
                                   BinaryExpression* binary_expression,
                                   Block* block, FunctionTuple* func,
                                   RingContext ctx) {

    assert(expression != nullptr);
    assert(binary_expression != nullptr);

    Expression* left  = binary_expression->left_expression;
    Expression* right = binary_expression->right_expression;

    std::string oper  = formate_operator(expression_type);

    fix_expression(left, block, func, RingContext{});
    fix_expression(right, block, func, RingContext{});

    // 检查两边操作数的合法性
    if (left->convert_type_size != 1
        || left->convert_type == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: oper `%s`, `%s` return no value; E:%d.",
            oper.c_str(),
            formate_expression(left).c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(left->line_number),
            .line_number             = left->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    if (right->convert_type_size != 1
        || right->convert_type == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: oper `%s`, `%s` return no value; E:%d.",
            oper.c_str(),
            formate_expression(right).c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(right->line_number),
            .line_number             = right->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    TypeSpecifier* left_type  = left->convert_type[0];
    TypeSpecifier* right_type = right->convert_type[0];

    if (left_type->kind != RING_BASIC_TYPE_BOOL) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: only apply bool to `%s` operator; E:%d.",
            oper.c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(left->line_number),
            .line_number             = left->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    if (right_type->kind != RING_BASIC_TYPE_BOOL) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: only apply bool to `%s` operator; E:%d.",
            oper.c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(right->line_number),
            .line_number             = right->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, &bool_type_specifier);

    if (ctx.optimize_level > 0) {
        crop_binary_logical_expression(expression, expression_type, binary_expression, block, func);
    }
}

void fix_binary_relational_expression(Expression*       expression,
                                      ExpressionType    expression_type,
                                      BinaryExpression* binary_expression,
                                      Block* block, FunctionTuple* func,
                                      RingContext ctx) {

    assert(expression != nullptr);
    assert(binary_expression != nullptr);
    assert(expression_type == EXPRESSION_TYPE_RELATIONAL_EQ
           || expression_type == EXPRESSION_TYPE_RELATIONAL_NE
           || expression_type == EXPRESSION_TYPE_RELATIONAL_GT
           || expression_type == EXPRESSION_TYPE_RELATIONAL_GE
           || expression_type == EXPRESSION_TYPE_RELATIONAL_LT
           || expression_type == EXPRESSION_TYPE_RELATIONAL_LE);

    Expression* left  = binary_expression->left_expression;
    Expression* right = binary_expression->right_expression;

    std::string oper  = formate_operator(expression_type);

    fix_expression(left, block, func, RingContext{});
    fix_expression(right, block, func, RingContext{});

    // 检查两边操作数的合法性
    if (left->convert_type_size != 1
        || left->convert_type == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: oper `%s`, `%s` return no value; E:%d.",
            oper.c_str(),
            formate_expression(left).c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(left->line_number),
            .line_number             = left->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    if (right->convert_type_size != 1
        || right->convert_type == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: oper `%s`, `%s` return no value; E:%d.",
            oper.c_str(),
            formate_expression(right).c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(right->line_number),
            .line_number             = right->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    TypeSpecifier* left_type  = left->convert_type[0];
    TypeSpecifier* right_type = right->convert_type[0];

    // 检查两遍的类型是否匹配
    if (expression_type == EXPRESSION_TYPE_RELATIONAL_EQ
        || expression_type == EXPRESSION_TYPE_RELATIONAL_NE) {
        if (!TYPE_IS_COMPARE_EQ(left_type)) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "invalid operation: only bool/int/int64/double apply to operator `%s`; E:%d.",
                oper.c_str(),
                ERROR_MATH_OPERATOR_MISMATCH_TYPE);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(left->line_number),
                .line_number             = left->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }

        if (!TYPE_IS_COMPARE_EQ(right_type)) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "invalid operation: only bool/int/int64/double apply to operator `%s`; E:%d.",
                oper.c_str(),
                ERROR_MATH_OPERATOR_MISMATCH_TYPE);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(right->line_number),
                .line_number             = right->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }
    } else {
        if (!TYPE_IS_COMPARE_REL(left_type)) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "invalid operation: only int/int64/double apply to operator `%s`; E:%d.",
                oper.c_str(),
                ERROR_MATH_OPERATOR_MISMATCH_TYPE);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(left->line_number),
                .line_number             = left->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }

        if (!TYPE_IS_COMPARE_REL(right_type)) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "invalid operation: only int/int64/double apply to operator `%s`; E:%d.",
                oper.c_str(),
                ERROR_MATH_OPERATOR_MISMATCH_TYPE);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(right->line_number),
                .line_number             = right->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }
    }

    if (left_type->kind != right_type->kind) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "invalid operation: oper `%s`, `%s` is %s, `%s` is %s, type mismatch; E:%d.",
            oper.c_str(),
            formate_expression(left).c_str(),
            format_type_specifier(left_type).c_str(),
            formate_expression(right).c_str(),
            format_type_specifier(right_type).c_str(),
            ERROR_MATH_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(binary_expression->line_number),
            .line_number             = binary_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, &bool_type_specifier);

    if (ctx.optimize_level > 0) {
        crop_binary_relational_expression(expression, expression_type, binary_expression, block, func);
    }
}

void fix_bitwise_binary_expression(Expression*       expression,
                                   ExpressionType    expression_type,
                                   BinaryExpression* binary_expression,
                                   Block* block, FunctionTuple* func,
                                   RingContext ctx) {

    assert(expression != nullptr);
    assert(binary_expression != nullptr);

    Expression* left  = binary_expression->left_expression;
    Expression* right = binary_expression->right_expression;

    std::string oper  = formate_operator(expression_type);

    fix_expression(left, block, func, RingContext{});
    fix_expression(right, block, func, RingContext{});

    // Ring-Compiler-Error-Report ERROR_OPER_INVALID_USE
    if (left->convert_type_size != 1
        || left->convert_type == nullptr
        || !TYPE_IS_INT32OR64(left->convert_type[0])) {

        std::string type = format_type_specifier(left->convert_type_size, left->convert_type);

        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "operator `%s` only be used in int/int64, but provided type(%s); E:%d.",
            oper.c_str(),
            type.c_str(),
            ERROR_OPER_INVALID_USE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(left->line_number),
            .line_number             = left->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    // Ring-Compiler-Error-Report ERROR_OPER_INVALID_USE
    if (right->convert_type_size != 1
        || right->convert_type == nullptr
        || !TYPE_IS_INT32OR64(right->convert_type[0])) {

        std::string type = format_type_specifier(right->convert_type_size, right->convert_type);

        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "operator `%s` only be used in int/int64, but provided type(%s); E:%d.",
            oper.c_str(),
            type.c_str(),
            ERROR_OPER_INVALID_USE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(right->line_number),
            .line_number             = right->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    if (expression_type == EXPRESSION_TYPE_BITWISE_AND
        || expression_type == EXPRESSION_TYPE_BITWISE_OR
        || expression_type == EXPRESSION_TYPE_BITWISE_XOR) {
        // 两个操作数类型必须相等
        // Ring-Compiler-Error-Report ERROR_OPER_INVALID_USE
        if (!comp_type_specifier(left->convert_type[0], right->convert_type[0])) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "operator `%s` only be used in same type, but provided type(%s, %s); E:%d.",
                oper.c_str(),
                format_type_specifier(left->convert_type_size, left->convert_type).c_str(),
                format_type_specifier(right->convert_type_size, right->convert_type).c_str(),
                ERROR_OPER_INVALID_USE);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(left->line_number),
                .line_number             = left->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }

    } else if (expression_type == EXPRESSION_TYPE_BITWISE_LSH
               || expression_type == EXPRESSION_TYPE_BITWISE_RSH) {

        // 被位移操作数必须为int/int64
        // 位移的数量也必须为int/int64
    }

    // 以left为准
    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, left->convert_type[0]);

    if (ctx.optimize_level > 0) {
        crop_binary_bitwise_expression(expression, binary_expression, block, func);
    }
}

void fix_bitwise_unitary_not_expression(Expression* expression,
                                        Expression* unitary_expression,
                                        Block* block, FunctionTuple* func,
                                        RingContext ctx) {


    fix_expression(unitary_expression, block, func, RingContext{});

    // Ring-Compiler-Error-Report ERROR_OPER_INVALID_USE
    if (unitary_expression->convert_type_size != 1
        || unitary_expression->convert_type == nullptr
        || !TYPE_IS_INT32OR64(unitary_expression->convert_type[0])) {

        std::string type = format_type_specifier(unitary_expression->convert_type_size, unitary_expression->convert_type);

        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "operator `~` only be used in int/int64, but provided type(%s); E:%d.",
            type.c_str(),
            ERROR_OPER_INVALID_USE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(unitary_expression->line_number),
            .line_number             = unitary_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    if (ctx.optimize_level > 0) {
        crop_unitary_expression(expression, unitary_expression, block, func);
    }

    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, unitary_expression->convert_type[0]);
}

void fix_unitary_minus_expression(Expression* expression,
                                  Expression* unitary_expression,
                                  Block* block, FunctionTuple* func,
                                  RingContext ctx) {

    fix_expression(unitary_expression, block, func, RingContext{});

    // Ring-Compiler-Error-Report ERROR_OPER_INVALID_USE
    if (unitary_expression->convert_type_size != 1
        || unitary_expression->convert_type == nullptr
        || !TYPE_IS_NUM(unitary_expression->convert_type[0])) {

        std::string type = format_type_specifier(unitary_expression->convert_type_size, unitary_expression->convert_type);

        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "operator `-` only be used in int/int64/double, but provided type(%s); E:%d.",
            type.c_str(),
            ERROR_OPER_INVALID_USE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(unitary_expression->line_number),
            .line_number             = unitary_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    if (ctx.optimize_level > 0) {
        crop_unitary_expression(expression, unitary_expression, block, func);
    }

    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, unitary_expression->convert_type[0]);
}

void fix_unitary_not_expression(Expression* expression,
                                Expression* unitary_expression,
                                Block* block, FunctionTuple* func,
                                RingContext ctx) {

    fix_expression(unitary_expression, block, func, RingContext{});

    // Ring-Compiler-Error-Report ERROR_OPER_INVALID_USE
    if (unitary_expression->convert_type_size != 1
        || unitary_expression->convert_type == nullptr
        || unitary_expression->convert_type[0]->kind != RING_BASIC_TYPE_BOOL) {

        std::string type = format_type_specifier(unitary_expression->convert_type_size, unitary_expression->convert_type);

        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "operator `not` only be used in bool, but provided type(%s); E:%d.",
            type.c_str(),
            ERROR_OPER_INVALID_USE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(unitary_expression->line_number),
            .line_number             = unitary_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    if (ctx.optimize_level > 0) {
        crop_unitary_expression(expression, unitary_expression, block, func);
    }

    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, unitary_expression->convert_type[0]);
}


void fix_unitary_increase_decrease_expression(Expression* expression,
                                              Expression* unitary_expression,
                                              Block* block, FunctionTuple* func) {

    fix_expression(unitary_expression, block, func, RingContext{});

    // Ring-Compiler-Error-Report ERROR_OPER_INVALID_USE
    if (unitary_expression->convert_type_size != 1
        || unitary_expression->convert_type == nullptr
        || !TYPE_IS_NUM(unitary_expression->convert_type[0])) {

        std::string type = format_type_specifier(unitary_expression->convert_type_size, unitary_expression->convert_type);

        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "operator `++`/`--` only be used in int/int64/double, but provided type(%s); E:%d.",
            type.c_str(),
            ERROR_OPER_INVALID_USE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(unitary_expression->line_number),
            .line_number             = unitary_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    if (global_ring_command_arg.optimize_level > 0) {
        crop_unitary_expression(expression, unitary_expression, block, func);
    }

    // TODO: 编译错误
    // 1++ 这种语法不支持
    // 1 不是左值
    // 但是目前这种分析还不太好做

    // 自增/自减 只能作为一个单独的 expression
    // 以下语法编译错误
    // a = b++ // 这种行为太复杂，不支持，因为复杂c/cpp 引入了前置/后置 自增/自减
    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
}

/*
 * 函数有几种可能, 判断优先顺序
 * ring closure
 * ring buildin 函数
 * ring native 函数
 * ring derive 函数
 *
 */
void fix_function_call_expression(Expression*             expression,
                                  FunctionCallExpression* function_call_expression,
                                  Block*                  block,
                                  FunctionTuple*          func) {

    if (function_call_expression == nullptr) {
        return;
    }

    // fix argument list
    for (ArgumentList* pos = function_call_expression->argument_list;
         pos != nullptr;
         pos = pos->next) {
        fix_expression(pos->expression, block, func, RingContext{});
    }

    Variable*   variable  = nullptr;
    Function*   function  = nullptr;

    Expression* func_expr = function_call_expression->func_expr;
    if (func_expr != nullptr && func_expr->type != EXPRESSION_TYPE_IDENTIFIER) {
        fix_expression(func_expr, block, func, RingContext{});

        // func_expr 的返回值必须是一个函数
        assert(func_expr->convert_type_size == 1);
        assert(func_expr->convert_type[0]->kind == RING_BASIC_TYPE_FUNC);

        // check 函数调用是否符合语义
        check_func_var_call(function_call_expression, func_expr->convert_type[0]);

        Ring_DeriveType_Func* func_type = func_expr->convert_type[0]->u.func_t;
        // func_expr 的返回值的返回值 是 expression 的类型
        for (FunctionReturnList* pos = func_type->return_list;
             pos != nullptr;
             pos = pos->next) {
            EXPRESSION_ADD_CONVERT_TYPE(expression, pos->type_specifier);
        }
        return;
    }


    // 1. 判断是否为closure
    Package* package = resolve_package(nullptr, 0, block, nullptr, RingContext{});
    variable         = resolve_variable(package,
                                        function_call_expression->func_identifier,
                                        block);
    if (variable != nullptr) {

        if (variable->decl->type_specifier->kind == RING_BASIC_TYPE_FUNC) {

            // check 函数调用是否符合语义
            check_func_var_call(function_call_expression, variable->decl->type_specifier);

            // 是一个变量，并且是一个函数变量，需要继续匹配
            function_call_expression->type              = FUNCTION_CALL_TYPE_CLOSURE;
            function_call_expression->u.cc.closure_decl = variable->decl;

            // function_call_expression 的类型取决于 function 返回值的类型
            EXPRESSION_CLEAR_CONVERT_TYPE(expression);
            for (FunctionReturnList* pos = variable->decl->type_specifier->u.func_t->return_list;
                 pos != nullptr;
                 pos = pos->next) {
                EXPRESSION_ADD_CONVERT_TYPE(expression, pos->type_specifier);
            }
            return;
        } else {
            // 只是一个普通变量，普通的变量 可以不覆盖 函数的可见性
            // continue search
        }
    }


    // 2. 判断是 ring buildin function
    if (is_buildin_function_identifier(function_call_expression->package_posit,
                                       function_call_expression->func_identifier)) {

        function_call_expression->type = FUNCTION_CALL_TYPE_FUNC;
        fix_buildin_func(expression, function_call_expression, block, func);

        return;
    }


    // 3. 搜素剩下的函数
    // 如果还不能搜索到，就需要报错了
    package  = resolve_package(function_call_expression->package_posit, function_call_expression->line_number, block, nullptr, RingContext{});
    function = search_function(package,
                               function_call_expression->func_identifier);
    // Ring-Compiler-Error-Report ERROR_UNDEFINITE_FUNCTION
    if (function == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf            = sprintf_string("use undeclared function `%s` in package `%s`; E:%d",
                                                    function_call_expression->func_identifier,
                                                    package->package_name,
                                                    ERROR_UNDEFINITE_FUNCTION);
        compile_adv_buf            = sprintf_string("definite function `%s` like: `function %s() {}` before use it.",
                                                    function_call_expression->func_identifier,
                                                    function_call_expression->func_identifier);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = nullptr,
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(function_call_expression->line_number),
            .line_number             = function_call_expression->line_number,
            .column_number           = 0,
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_EXIT_NOW,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    // check 函数调用是否符合语义
    check_function_call(function_call_expression, function);

    function_call_expression->type          = FUNCTION_CALL_TYPE_FUNC;
    function_call_expression->u.fc.function = function;


    // function_call_expression 的类型取决于 function 返回值的类型
    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    for (FunctionReturnList* pos = function->return_list;
         pos != nullptr;
         pos = pos->next) {
        EXPRESSION_ADD_CONVERT_TYPE(expression, pos->type_specifier);
    }
}

void fix_member_call_expression(Expression*           expression,
                                MemberCallExpression* member_call_expression,
                                Block*                block,
                                FunctionTuple*        func) {

    if (member_call_expression == nullptr) {
        return;
    }

    // fix argument list
    for (ArgumentList* pos = member_call_expression->argument_list;
         pos != nullptr;
         pos = pos->next) {
        fix_expression(pos->expression, block, func, RingContext{});
    }

    char*            member_identifier = member_call_expression->member_identifier;
    ClassDefinition* class_definition  = nullptr;
    MethodMember*    method            = nullptr;
    FieldMember*     field             = nullptr;
    Expression*      object_expression = member_call_expression->object_expression;

    // fix object expression
    fix_expression(object_expression, block, func, RingContext{});

    // find class definition by object.
    class_definition = object_expression->convert_type[0]->u.class_t->class_definition;
    if (class_definition == nullptr) {
        ring_error_report("fix_member_call_expression error\n");
    }

    // 2. is method or not ?
    method = search_class_method(class_definition, member_identifier);
    if (method != nullptr) {
        // check method 调用是否和method定义是否一致
        check_method_call(member_call_expression, method);
        member_call_expression->type               = MEMBER_CALL_TYPE_METHOD;
        member_call_expression->u.mc.method_member = method;

        // member_call_expression 的类型取决于 method返回值的类型
        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        for (FunctionReturnList* pos = method->return_list;
             pos != nullptr;
             pos = pos->next) {
            EXPRESSION_ADD_CONVERT_TYPE(expression, pos->type_specifier);
        }
        return;
    }

    // 3. 看看是不是一个 field，这个field的类型是匿名函数变量
    field = search_class_field(class_definition, member_identifier);
    if (field != nullptr) {
        // Ring-Compiler-Error-Report ERROR_INVOKE_FIELD_OF_CLASS_AS_METHOD
        if (field->type_specifier->kind != RING_BASIC_TYPE_FUNC) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "field `%s` not a anonymous-function in class `%s`; E:%d.",
                member_identifier,
                class_definition->identifier,
                ERROR_NOT_FOUND_CLASS_MEMBER);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(member_call_expression->line_number),
                .line_number             = member_call_expression->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }

        // check field 调用是否和field 匿名函数的类型定义是否一致
        check_field_call(member_call_expression, field);
        member_call_expression->type              = MEMBER_CALL_TYPE_FIELD;
        member_call_expression->u.fc.field_member = field;

        // member_call_expression 的类型取决于 method返回值的类型
        EXPRESSION_CLEAR_CONVERT_TYPE(expression);
        for (FunctionReturnList* pos = field->type_specifier->u.func_t->return_list;
             pos != nullptr;
             pos = pos->next) {
            EXPRESSION_ADD_CONVERT_TYPE(expression, pos->type_specifier);
        }
        return;
    }


    // Ring-Compiler-Error-Report ERROR_NOT_FOUND_CLASS_MEMBER
    if (true) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "not found method/field `%s` in class `%s`; E:%d.",
            member_identifier,
            class_definition->identifier,
            ERROR_NOT_FOUND_CLASS_MEMBER);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(member_call_expression->line_number),
            .line_number             = member_call_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }
}

void fix_class_definition(ClassDefinition* class_definition) {
    assert(class_definition != nullptr);

    unsigned int field_index  = 0;
    unsigned int method_index = 0;

    for (FieldMember* pos = class_definition->field_list;
         pos != nullptr;
         pos = pos->next, field_index++) {
        pos->index_of_class = field_index;
        fix_class_field(class_definition, pos);
    }
    for (MethodMember* pos = class_definition->method_list;
         pos != nullptr;
         pos = (MethodMember*)pos->next, method_index++) {
        pos->index_of_class = method_index;
        fix_class_method(class_definition, pos);
    }
}

void fix_class_field(ClassDefinition* class_definition, FieldMember* field) {
    fix_type_specfier(field->type_specifier, class_definition->package_unit->parent_package);
}

void fix_class_method(ClassDefinition* class_definition, MethodMember* method) {
    Block* block = method->block;

    // `self` variable
    Package*       package           = class_definition->package_unit->parent_package;
    TypeAlias*     type_alias        = search_type_alias(package, class_definition->identifier);
    TypeSpecifier* type_specifier    = type_alias->type_specifier;

    VarDecl*       self_declaration  = (VarDecl*)mem_alloc(get_front_mem_pool(), sizeof(VarDecl));
    self_declaration->line_number    = method->start_line_number;
    self_declaration->blong_block    = method->block;
    self_declaration->type_specifier = type_specifier;
    self_declaration->identifier     = (char*)"self";
    self_declaration->is_const       = false;
    self_declaration->is_local       = false;
    self_declaration->variable_index = -1;
    self_declaration->next           = nullptr;

    // add `self` variable to local variable list.
    add_local_declaration(self_declaration, block, nullptr);

    FunctionReturnList* return_list = method->return_list;
    for (; return_list != nullptr; return_list = return_list->next) {
        fix_type_specfier(return_list->type_specifier, class_definition->package_unit->parent_package);
    }

    if (block != nullptr) {
        add_parameter_to_declaration(method->parameter_list, block);
        fix_statement_list(block->statement_list, block, (FunctionTuple*)method);
    }
}

void fix_array_index_expression(Expression*           expression,
                                ArrayIndexExpression* array_index_expression,
                                Block*                block,
                                FunctionTuple*        func) {

    assert(array_index_expression != nullptr);

    char*     package_posit    = array_index_expression->array_expression->u.identifier_expression->path_segment;
    char*     array_identifier = array_index_expression->array_expression->u.identifier_expression->identifier;
    Variable* variable         = nullptr;

    Package*  package          = resolve_package(package_posit, array_index_expression->line_number, block, nullptr, RingContext{});
    variable                   = resolve_variable(package,
                                                  array_identifier,
                                                  block);

    // Ring-Compiler-Error-Report ERROR_UNDEFINITE_VARIABLE
    if (variable == nullptr) {
        DEFINE_ERROR_REPORT_STR;
        compile_err_buf = sprintf_string(
            "use undeclared identifier `%s`; E:%d.",
            array_identifier,
            ERROR_UNDEFINITE_VARIABLE);
        compile_adv_buf = sprintf_string(
            "definite variable `%s` like: `var bool[]|int[]|double[]|string[] %s;` before use it.",
            array_identifier,
            array_identifier);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(array_index_expression->line_number),
            .line_number             = array_index_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    TypeSpecifier* type_specifier = variable->decl->type_specifier;
    // Ring-Compiler-Error-Report ERROR_VAR_IS_NOT_ARRAY
    if (!TYPE_IS_ARRAY(type_specifier)) {
        DEFINE_ERROR_REPORT_STR;
        compile_err_buf = sprintf_string(
            "`%s` is not an array; E:%d.",
            array_identifier,
            ERROR_VAR_IS_NOT_ARRAY);
        compile_adv_buf = sprintf_string(
            "definite variable `%s` like: `var bool[]|int[]|double[]|string[] %s;` before use it.",
            array_identifier,
            array_identifier);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(array_index_expression->line_number),
            .line_number             = array_index_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }
    assert(type_specifier->u.array_t != nullptr);
    TypeSpecifier* sub_type_specifier = type_specifier->u.array_t->sub;
    assert(sub_type_specifier->kind != RING_BASIC_TYPE_UNKNOW
           && sub_type_specifier->kind != RING_BASIC_TYPE_ARRAY
           && sub_type_specifier->kind != RING_BASIC_TYPE_ANY);

    fix_expression(array_index_expression->array_expression, block, func, RingContext{});

    fix_dimension_expression(array_index_expression->index_expression, block, func);

    // array_index_expression->array_expression->u.identifier_expression->u.declaration = declaration;

    DimensionExpression* index_expression = array_index_expression->index_expression;
    // 修正最外层 expression 的 convert_type
    TypeSpecifier* type = (TypeSpecifier*)mem_alloc(get_front_mem_pool(), sizeof(TypeSpecifier));
    type->line_number   = expression->line_number;

    /*
     * e.g. var int[,,,] students;
     *
     * students is a four-dimension array.
     * students[0] is a three-dimension array.
     * students[0,0,0] is a int value.
     */
    // TODO: fix_array_index_expression 与 fix_range_expression 抽象出公共的方法
    if (index_expression->dimension < variable->decl->type_specifier->u.array_t->dimension) {
        type->kind                 = variable->decl->type_specifier->kind;
        type->u.array_t            = (Ring_DeriveType_Array*)mem_alloc(get_front_mem_pool(), sizeof(Ring_DeriveType_Array));
        type->u.array_t->dimension = variable->decl->type_specifier->u.array_t->dimension - index_expression->dimension;
        type->u.array_t->sub       = variable->decl->type_specifier->u.array_t->sub;
    } else if (index_expression->dimension == variable->decl->type_specifier->u.array_t->dimension) {
        type->kind = variable->decl->type_specifier->u.array_t->sub->kind;
    } else {
        // 数组为 int[], 但是a[1,1] 这样访问
        // Ring-Compiler-Error-Report ERROR_ARRAY_ACCESS_DIMENSION_INVALID
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "dimension of array is %d, but access dimension is %d; E:%d.",
            variable->decl->type_specifier->u.array_t->dimension,
            index_expression->dimension,
            ERROR_ARRAY_ACCESS_DIMENSION_INVALID);


        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(array_index_expression->line_number),
            .line_number             = array_index_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    // shallow copy
    if (type->kind == RING_BASIC_TYPE_CLASS) {
        type->u.class_t = variable->decl->type_specifier->u.array_t->sub->u.class_t;
    } else if (type->kind == RING_BASIC_TYPE_FUNC) {
        type->u.func_t = variable->decl->type_specifier->u.array_t->sub->u.func_t;
    } else {
        // 上面已经处理过了
    }
    fix_type_specfier(type, block->package_unit->parent_package);

    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, type);
}

void fix_slice_expression(Expression*      expression,
                          SliceExpression* slice_expression,
                          Block*           block,
                          FunctionTuple*   func) {

    assert(slice_expression != nullptr);

    char*     package_posit    = nullptr;
    char*     array_identifier = slice_expression->operand->u.identifier_expression->identifier;
    Variable* variable         = nullptr;

    Package*  package          = resolve_package(package_posit, slice_expression->line_number, block, nullptr, RingContext{});
    variable                   = resolve_variable(package,
                                                  array_identifier,
                                                  block);

    // Ring-Compiler-Error-Report ERROR_UNDEFINITE_VARIABLE
    if (variable == nullptr) {
        DEFINE_ERROR_REPORT_STR;
        compile_err_buf = sprintf_string(
            "use undeclared identifier `%s`; E:%d.",
            array_identifier,
            ERROR_UNDEFINITE_VARIABLE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(slice_expression->line_number),
            .line_number             = slice_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    TypeSpecifier* type_specifier = variable->decl->type_specifier;
    // Ring-Compiler-Error-Report ERROR_SLICE_OPERATOR_MISMATCH_TYPE
    if (type_specifier->kind != RING_BASIC_TYPE_ARRAY && type_specifier->kind != RING_BASIC_TYPE_STRING) {
        DEFINE_ERROR_REPORT_STR;
        compile_err_buf = sprintf_string(
            "`%s` is not an array/string; E:%d.",
            array_identifier,
            ERROR_SLICE_OPERATOR_MISMATCH_TYPE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(slice_expression->line_number),
            .line_number             = slice_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    fix_expression(slice_expression->operand, block, func, RingContext{});
    fix_expression(slice_expression->sub_slice->start_expr, block, func, RingContext{});
    fix_expression(slice_expression->sub_slice->end_expr, block, func, RingContext{});

    if (type_specifier->kind == RING_BASIC_TYPE_ARRAY) {
        slice_expression->slice_operand_type = SLICE_OPERAND_TYPE_ARRAY;
    } else if (type_specifier->kind == RING_BASIC_TYPE_STRING) {
        slice_expression->slice_operand_type = SLICE_OPERAND_TYPE_STRING;
    }

    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, type_specifier);
}

void fix_new_array_expression(Expression*         expression,
                              NewArrayExpression* new_array_expression,
                              Block*              block,
                              FunctionTuple*      func) {

    fix_dimension_expression(new_array_expression->dimension_expression, block, func);
    fix_type_specfier(new_array_expression->type_specifier, block->package_unit->parent_package);

    TypeSpecifier* sub_type_specifier = new_array_expression->type_specifier->u.array_t->sub;
    assert(sub_type_specifier != nullptr);
    assert(sub_type_specifier->kind != RING_BASIC_TYPE_UNKNOW
           && sub_type_specifier->kind != RING_BASIC_TYPE_ARRAY
           && sub_type_specifier->kind != RING_BASIC_TYPE_ANY);


    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, (new_array_expression->type_specifier));
}

void fix_dimension_expression(DimensionExpression* dimension_expression,
                              Block*               block,
                              FunctionTuple*       func) {

    SubDimensionExpression* pos = dimension_expression->dimension_list;
    for (; pos != nullptr; pos = pos->next) {
        fix_expression(pos->num_expression, block, func, RingContext{});
    }
}

void fix_array_literal_expression(Expression*             expression,
                                  ArrayLiteralExpression* array_literal_expression,
                                  Block*                  block,
                                  FunctionTuple*          func) {

    assert(array_literal_expression != nullptr);

    fix_type_specfier(array_literal_expression->type_specifier, block->package_unit->parent_package);
    TypeSpecifier* array_type = array_literal_expression->type_specifier;
    TypeSpecifier* item_type  = nullptr;
    assert(array_type->kind = RING_BASIC_TYPE_ARRAY);

    TypeSpecifier* sub_type_specifier = array_type->u.array_t->sub;
    assert(sub_type_specifier->kind != RING_BASIC_TYPE_UNKNOW
           && sub_type_specifier->kind != RING_BASIC_TYPE_ARRAY
           && sub_type_specifier->kind != RING_BASIC_TYPE_ANY);


    for (Expression* pos = array_literal_expression->expression_list;
         pos != nullptr;
         pos = pos->next) {

        fix_expression(pos, block, func, RingContext{});

        // Ring-Compiler-Error-Report ERROR_ARRAY_LITERAL_INVALID_ITEM
        if (pos->convert_type_size != 1
            || pos->convert_type == nullptr) {
            std::string pos_type_str = format_type_specifier(pos->convert_type_size, pos->convert_type);

            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "array literal: invalid item type(%s); E:%d.",
                pos_type_str.c_str(),
                ERROR_ARRAY_LITERAL_INVALID_ITEM);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(pos->line_number),
                .line_number             = pos->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }


        item_type = pos->convert_type[0];

        // 检查类型是否匹配
        // 如果item 类型为 bool , 那 数组是 bool[]
        // 如果item 类型为 bool[!1] , 那 数组是 bool[!2]
        // 需要差一个维度
        // Ring-Compiler-Error-Report ERROR_ARRAY_LITERAL_MISMATCH_TYPE
        if (!comp_type_specifier_dimension(array_type, item_type, 1)) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "array literal expression invalid: array type (%s) not match with literal item type (%s); E:%d.",
                format_type_specifier(array_type).c_str(),
                format_type_specifier(item_type).c_str(),
                ERROR_ARRAY_LITERAL_MISMATCH_TYPE);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(pos->line_number),
                .line_number             = pos->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }
    }

    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, array_literal_expression->type_specifier);
}

void fix_class_object_literal_expression(Expression*                   expression,
                                         ClassObjectLiteralExpression* literal_expression,
                                         Block*                        block,
                                         FunctionTuple*                func) {

    assert(literal_expression != nullptr);

    fix_type_specfier(literal_expression->type_specifier, block->package_unit->parent_package);

    ClassDefinition* class_definition = literal_expression->type_specifier->u.class_t->class_definition;

    /*
     * 1. field 要存在
     * 2. field 不能被初始化两次
     * 3. field 的类型要匹配
     */

    std::vector<std::string> field_init_map;

    FieldInitExpression*     pos = literal_expression->field_init_expression_list;
    for (; pos != nullptr; pos = pos->next) {

        char*        field_identifier = pos->field_identifier;
        FieldMember* field_member     = nullptr;
        for (FieldMember* pos = class_definition->field_list; pos != nullptr; pos = pos->next) {
            if (str_eq(field_identifier, pos->identifier)) {
                field_member = pos;
                break;
            }
        }

        // 1.
        // Ring-Compiler-Error-Report ERROR_NOT_FOUND_CLASS_FIELD
        if (field_member == nullptr) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "ths class `%s` has not field `%s`; E:%d.",
                class_definition->identifier,
                field_identifier,
                ERROR_NOT_FOUND_CLASS_FIELD);
            compile_adv_buf = sprintf_string(
                "the class `%s` definition in %s:%d.",
                class_definition->identifier,
                class_definition->source_file.c_str(), class_definition->start_line_number);


            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(pos->line_number),
                .line_number             = pos->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_EXIT_NOW,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }

        // 2. 是否重复初始化
        // Ring-Compiler-Error-Report ERROR_DUPLICATE_INIT_CLASS_FIELD
        for (auto field_tmp : field_init_map) {
            if (str_eq(field_tmp.c_str(), field_identifier)) {
                DEFINE_ERROR_REPORT_STR;

                compile_err_buf = sprintf_string(
                    "duplicate init field `%s`; E:%d.",
                    field_identifier,
                    ERROR_DUPLICATE_INIT_CLASS_FIELD);

                ErrorReportContext context = {
                    .package                 = nullptr,
                    .package_unit            = get_package_unit(),
                    .source_file_name        = get_package_unit()->current_file_name,
                    .line_content            = package_unit_get_line_content(pos->line_number),
                    .line_number             = pos->line_number,
                    .column_number           = package_unit_get_column_number(),
                    .error_message           = std::string(compile_err_buf),
                    .advice                  = std::string(compile_adv_buf),
                    .report_type             = ERROR_REPORT_TYPE_EXIT_NOW,
                    .ring_compiler_file      = (char*)__FILE__,
                    .ring_compiler_file_line = __LINE__,
                };
                ring_compile_error_report(&context);
            }
        }
        field_init_map.push_back(std::string(field_identifier));

        pos->field_member = field_member;


        // fix init_expression
        fix_expression(pos->init_expression, block, func, RingContext{});


        // 3. 查看类型是否匹配，有点类似于 assign的类型检查
        TypeSpecifier* left_type = field_member->type_specifier;
        // Ring-Compiler-Error-Report ERROR_ARRAY_LITERAL_INVALID_FILED_INIT_EXPR
        if (pos->init_expression->convert_type_size != 1
            || pos->init_expression->convert_type == nullptr) {
            std::string pos_type_str = format_type_specifier(pos->init_expression->convert_type_size, pos->init_expression->convert_type);

            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "array literal field expression: expect (%s) but provided (%s); E:%d.",
                format_type_specifier(left_type).c_str(),
                pos_type_str.c_str(),
                ERROR_ARRAY_LITERAL_INVALID_FILED_INIT_EXPR);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(pos->line_number),
                .line_number             = pos->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }


        TypeSpecifier* right_type = pos->init_expression->convert_type[0];
        // Ring-Compiler-Error-Report ERROR_ASSIGNMENT_MISMATCH_TYPE
        if (!comp_type_specifier(left_type, right_type)) {
            DEFINE_ERROR_REPORT_STR;

            compile_err_buf = sprintf_string(
                "assignment mismatch: expect (%s) but provided (%s); E:%d.",
                format_type_specifier(left_type).c_str(),
                format_type_specifier(right_type).c_str(),
                ERROR_ASSIGNMENT_MISMATCH_TYPE);

            ErrorReportContext context = {
                .package                 = nullptr,
                .package_unit            = get_package_unit(),
                .source_file_name        = get_package_unit()->current_file_name,
                .line_content            = package_unit_get_line_content(pos->line_number),
                .line_number             = pos->line_number,
                .column_number           = package_unit_get_column_number(),
                .error_message           = std::string(compile_err_buf),
                .advice                  = std::string(compile_adv_buf),
                .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                .ring_compiler_file      = (char*)__FILE__,
                .ring_compiler_file_line = __LINE__,
            };
            ring_compile_error_report(&context);
        }
    }

    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, literal_expression->type_specifier);
}

/*
 * fix_field_member_expression 只是 修正 field
 */
void fix_field_member_expression(Expression*       expression,
                                 MemberExpression* member_expression,
                                 Block*            block,
                                 FunctionTuple*    func) {

    assert(member_expression != nullptr);

    Expression*      object_expression = member_expression->object_expression;
    char*            member_identifier = member_expression->field_member_identifier;
    ClassDefinition* class_definition  = nullptr;
    FieldMember*     field             = nullptr;


    // 0. fix object expression
    fix_expression(object_expression, block, func, RingContext{});

    // 1. find class definition by object.
    class_definition = object_expression->convert_type[0]->u.class_t->class_definition;
    if (class_definition == nullptr) {
        ring_error_report("fix_field_member_expression error, class_definition is null\n");
    }


    // 2. find member declaration by member identifier.
    field = search_class_field(class_definition, member_identifier);

    // Ring-Compiler-Error-Report ERROR_INVALID_NOT_FOUND_CLASS_FIELD
    if (field == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "not found field `%s`; E:%d.",
            member_identifier,
            ERROR_NOT_FOUND_CLASS_FIELD);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(member_expression->line_number),
            .line_number             = member_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    member_expression->field_member = field;


    // expression 最终的类型取决于field-member 的类型
    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, field->type_specifier);
    fix_class_member_expression(member_expression, object_expression, member_identifier);
}


void fix_class_member_expression(MemberExpression* member_expression,
                                 Expression*       object_expression,
                                 char*             member_identifier) {

    // member_expression->member_declaration = ;
}

ClassDefinition* search_class_definition(char* class_identifier) {
    assert(class_identifier != nullptr);

    for (ClassDefinition* pos : get_package_unit()->class_definition_list) {
        if (str_eq(pos->identifier, class_identifier)) {
            return pos;
        }
    }

    return nullptr;
}

TypeAlias* search_type_alias(Package* package, char* identifier) {
    assert(identifier != nullptr);

    for (TypeAlias* pos : package->type_alias_list) {
        if (str_eq(pos->identifier, identifier)) {
            return pos;
        }
    }

    return nullptr;
}


FieldMember* search_class_field(ClassDefinition* class_definition, char* identifier) {
    assert(class_definition != nullptr);

    FieldMember* res = nullptr;

    for (FieldMember* pos = class_definition->field_list; pos != nullptr; pos = pos->next) {
        if (str_eq(pos->identifier, identifier)) {
            res = pos;
            break;
        }
    }

    return res;
}

MethodMember* search_class_method(ClassDefinition* class_definition, char* identifier) {
    assert(class_definition != nullptr);

    MethodMember* res = nullptr;

    for (MethodMember* pos = class_definition->method_list; pos != nullptr; pos = (MethodMember*)pos->next) {
        if (str_eq(pos->identifier, identifier)) {
            res = pos;
            break;
        }
    }

    return res;
}

void fix_ternary_condition_expression(Expression*        expression,
                                      TernaryExpression* ternary_expression,
                                      Block*             block,
                                      FunctionTuple*     func) {

    if (ternary_expression == nullptr) {
        return;
    }

    Expression* condition_expression = ternary_expression->condition_expression;
    Expression* true_expression      = ternary_expression->true_expression;
    Expression* false_expression     = ternary_expression->false_expression;

    fix_expression(condition_expression, block, func, RingContext{});
    fix_expression(true_expression, block, func, RingContext{});
    fix_expression(false_expression, block, func, RingContext{});

    // Ring-Compiler-Error-Report ERROR_INVALID_TERNARY_EXPR_CONDITION
    if (condition_expression->convert_type_size != 1
        || condition_expression->convert_type == nullptr
        || condition_expression->convert_type[0]->kind != RING_BASIC_TYPE_BOOL) {
        std::string condition_expression_type = format_type_specifier(condition_expression->convert_type_size, condition_expression->convert_type);
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "requires type (bool) in ` ? : ` expression, but provided type (%s); E:%d.",
            condition_expression_type.c_str(),
            ERROR_INVALID_TERNARY_EXPR_CONDITION);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(condition_expression->line_number),
            .line_number             = condition_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    // true 和 false expression 里边的表达式类型应该一致
    // 类似于 assign 语句的 left right 的匹配


    // Ring-Compiler-Error-Report ERROR_INVALID_TERNARY_EXPR_TRUE_OR_FALSE
    if (true_expression->convert_type_size != false_expression->convert_type_size) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "<true_expression> and <false_expression> must same type in ternary expression; E:%d.",
            ERROR_INVALID_TERNARY_EXPR_CONDITION);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(condition_expression->line_number),
            .line_number             = condition_expression->line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    // Ring-Compiler-Error-Report ERROR_INVALID_TERNARY_EXPR_TRUE_OR_FALSE
    if (true_expression->convert_type_size) {

        std::vector<TypeSpecifier*> true_convert_type;
        for (unsigned int i = 0; i < true_expression->convert_type_size; i++) {
            true_convert_type.push_back(true_expression->convert_type[i]);
        }
        std::vector<TypeSpecifier*> false_convert_type;
        for (unsigned int i = 0; i < false_expression->convert_type_size; i++) {
            false_convert_type.push_back(false_expression->convert_type[i]);
        }

        std::string true_type_s;
        {
            // formate left to string
            std::vector<std::string> tmp;
            for (TypeSpecifier* type : true_convert_type) {
                tmp.push_back(format_type_specifier(type));
            }
            true_type_s = "(" + strings_join(tmp, ",") + ")";
        }
        std::string false_type_s;
        {
            // formate left to string
            std::vector<std::string> tmp;
            for (TypeSpecifier* type : false_convert_type) {
                tmp.push_back(format_type_specifier(type));
            }
            false_type_s = "(" + strings_join(tmp, ",") + ")";
        }

        for (unsigned int i = 0; i < true_convert_type.size(); i++) {

            // 比较 true 和 false 表达式是否一致
            if (!comp_type_specifier(true_convert_type[i], false_convert_type[i])) {
                DEFINE_ERROR_REPORT_STR;

                compile_err_buf = sprintf_string(
                    "<true_expression> type is %s, <false_expression> type is %s, not match; E:%d.",
                    true_type_s.c_str(),
                    false_type_s.c_str(),
                    ERROR_ASSIGNMENT_MISMATCH_TYPE);

                ErrorReportContext context = {
                    .package                 = nullptr,
                    .package_unit            = get_package_unit(),
                    .source_file_name        = get_package_unit()->current_file_name,
                    .line_content            = package_unit_get_line_content(expression->line_number),
                    .line_number             = expression->line_number,
                    .column_number           = package_unit_get_column_number(),
                    .error_message           = std::string(compile_err_buf),
                    .advice                  = std::string(compile_adv_buf),
                    .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
                    .ring_compiler_file      = (char*)__FILE__,
                    .ring_compiler_file_line = __LINE__,
                };
                ring_compile_error_report(&context);
            }
        }
    }


    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    if (true_expression->convert_type_size) {
        EXPRESSION_ADD_CONVERT_TYPE(expression, true_expression->convert_type[0]);
    }

    if (global_ring_command_arg.optimize_level > 0) {
        crop_ternary_condition_expression(expression, ternary_expression, block, func);
    }


    return;
}

/*
 * 是否为 method 在语法分析阶段就能确定
 * 需要通过语义 判断是否为 function 还是 closure
 */
void fix_launch_expression(Expression*       expression,
                           LaunchExpression* launch_expression,
                           Block*            block,
                           FunctionTuple*    func) {

    if (launch_expression == nullptr) {
        return;
    }

    switch (launch_expression->type) {
    case LAUNCH_EXPRESSION_TYPE_FUNCTION_CALL:
        fix_function_call_expression(expression, launch_expression->u.function_call_expression, block, func);
        break;

    case LAUNCH_EXPRESSION_TYPE_MEMBER_CALL:
        fix_member_call_expression(expression, launch_expression->u.member_call_expression, block, func);
        break;

    case LAUNCH_EXPRESSION_TYPE_IIFE:
        fix_iife_expression(expression, launch_expression->u.iife, block, func);
        break;

    default:
        break;
    }

    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, &int64_type_specifier);
}

void fix_anonymous_func_expression(Expression*              expression,
                                   AnonymousFuncExpression* closure_expression,
                                   Block*                   block,
                                   FunctionTuple*           func) {

    if (closure_expression == nullptr) {
        return;
    }

    AnonymousFunc* closure = closure_expression->anonymous_func;

    // 这里的实现方式和 fix_function_definition 一样
    FunctionReturnList* return_list = closure->return_list;
    for (; return_list != nullptr; return_list = return_list->next) {
        fix_type_specfier(return_list->type_specifier, block->package_unit->parent_package);
    }

    if (closure->block) {
        add_parameter_to_declaration(closure->parameter_list, closure->block);
        fix_statement_list(closure->block->statement_list, closure->block, (FunctionTuple*)closure);
    }

    // 从函数定义中，创建一个 type_specifier , 用于 后续比对 type_specifier 是否一致
    // 类型为匿名函数的类型
    // TODO: 每次都创建新内存，不能复用
    Ring_DeriveType_Func* func_type      = create_derive_type_func(closure->parameter_list, closure->return_list);

    TypeSpecifier*        type_specifier = (TypeSpecifier*)mem_alloc(get_front_mem_pool(), sizeof(TypeSpecifier));
    type_specifier->line_number          = closure_expression->line_number;
    type_specifier->identifier           = nullptr;
    type_specifier->kind                 = RING_BASIC_TYPE_FUNC;
    type_specifier->u.func_t             = func_type;
    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    EXPRESSION_ADD_CONVERT_TYPE(expression, type_specifier);
}

void fix_iife_expression(Expression*                   expression,
                         ImmediateInvokFuncExpression* iife,
                         Block*                        block,
                         FunctionTuple*                func) {

    if (iife == nullptr) {
        return;
    }

    // fix argument list
    for (ArgumentList* pos = iife->argument_list;
         pos != nullptr;
         pos = pos->next) {
        fix_expression(pos->expression, block, func, RingContext{});
    }

    AnonymousFunc* anonymous_func = iife->anonymous_func;

    Parameter*     parameter_list = anonymous_func->parameter_list;
    for (; parameter_list != nullptr; parameter_list = parameter_list->next) {
        fix_type_specfier(parameter_list->type_specifier, block->package_unit->parent_package);
    }

    // 这里的实现方式和 fix_function_definition 一样
    FunctionReturnList* return_list = anonymous_func->return_list;
    for (; return_list != nullptr; return_list = return_list->next) {
        fix_type_specfier(return_list->type_specifier, block->package_unit->parent_package);
    }

    if (anonymous_func->block) {
        add_parameter_to_declaration(anonymous_func->parameter_list, anonymous_func->block);
        fix_statement_list(anonymous_func->block->statement_list, anonymous_func->block, (FunctionTuple*)anonymous_func);
    }

    check_iife_call(iife);


    // 如果 iife 是一个单独的表达式，
    // 无需将 convert_type 透传到最外层
    if (expression == nullptr) {
        return;
    }
    // 函数返回值的类型作为整个表达式的类型
    EXPRESSION_CLEAR_CONVERT_TYPE(expression);
    for (FunctionReturnList* pos = anonymous_func->return_list;
         pos != nullptr;
         pos = pos->next) {
        EXPRESSION_ADD_CONVERT_TYPE(expression, pos->type_specifier);
    }
}

void add_parameter_to_declaration(Parameter* parameter, Block* block) {
    assert(block != nullptr);

    Parameter* pos = parameter;
    for (; pos; pos = pos->next) {

        TypeSpecifier* src_type_specifier = pos->type_specifier;
        TypeSpecifier* type_specifier     = nullptr;

        if (pos->is_variadic) {
            // fix type_specifier to array.
            type_specifier              = (TypeSpecifier*)mem_alloc(get_front_mem_pool(), sizeof(TypeSpecifier));
            type_specifier->line_number = src_type_specifier->line_number;
            type_specifier->kind        = RING_BASIC_TYPE_ARRAY;

            if (src_type_specifier->kind == RING_BASIC_TYPE_ARRAY) {
                // 直接增加一个维度即可
                type_specifier->u.array_t            = (Ring_DeriveType_Array*)mem_alloc(get_front_mem_pool(), sizeof(Ring_DeriveType_Array));
                type_specifier->u.array_t->dimension = src_type_specifier->u.array_t->dimension + 1;
                type_specifier->u.array_t->sub       = src_type_specifier->u.array_t->sub;

            } else {
                // 元素升级为数组
                type_specifier->u.array_t            = (Ring_DeriveType_Array*)mem_alloc(get_front_mem_pool(), sizeof(Ring_DeriveType_Array));
                type_specifier->u.array_t->dimension = 1;
                type_specifier->u.array_t->sub       = src_type_specifier;
            }
        } else {
            type_specifier = src_type_specifier;
        }


        VarDecl* declaration        = (VarDecl*)mem_alloc(get_front_mem_pool(), sizeof(VarDecl));
        declaration->line_number    = pos->line_number;
        declaration->blong_block    = block;
        declaration->type_specifier = type_specifier;
        declaration->identifier     = pos->identifier;
        declaration->is_const       = 0;
        declaration->is_local       = 1;
        declaration->variable_index = -1; // fix in add_declaration
        declaration->next           = nullptr;

        add_local_declaration(declaration, block, nullptr);
    }
}

// TODO: 目前只能使用当前package 的enum
// block current_package 二者选其一
EnumDeclaration* resolve_enum_decl(char*    enum_identifier,
                                   Block*   block,
                                   Package* current_package_) {

    Package* current_package = nullptr;
    if (block != nullptr) {
        current_package = block->package_unit->parent_package;
    } else if (current_package_ != nullptr) {
        current_package = current_package_;
    } else {
        // TODO: error-report
        return nullptr;
    }

    for (TypeAlias* pos : current_package->type_alias_list) {
        if (pos->is_enum && str_eq(pos->identifier, enum_identifier)) {
            return pos->enum_declaration;
        }
    }


    return nullptr;
}

EnumItemDeclaration* resolve_enum_item(EnumDeclaration* enum_decl, char* enum_item_identifier) {
    EnumItemDeclaration* pos = nullptr;
    for (pos = enum_decl->enum_item_list; pos != nullptr; pos = pos->next) {
        if (str_eq(pos->identifier, enum_item_identifier)) {
            return pos;
        }
    }
    return nullptr;
}

// block current_package 二者选其一
Package* resolve_package(char*        package_posit,
                         unsigned int line_number,
                         Block*       block,
                         Package*     current_package,
                         RingContext  ctx) {

    Package* package = nullptr;

    if (package_posit == nullptr || strlen(package_posit) == 0) {
        // 在当前 package中查找

        if (block != nullptr) {
            assert(block->package_unit != nullptr);
            assert(block->package_unit->parent_package != nullptr);
            return block->package_unit->parent_package;
        } else if (current_package != nullptr) {
            return current_package;
        } else if (ctx.package != nullptr) {
            return ctx.package;
        }
        // else 不合法的情况
    }

    // Ring-Compiler-Error-Report ERROR_USE_PACKAGE_MAIN_IMPORT
    if (str_eq(package_posit, PACKAGE_MAIN)) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "canot use package `%s` to find symbol; E:%d.",
            package_posit,
            ERROR_USE_PACKAGE_MAIN_IMPORT);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(line_number),
            .line_number             = line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }
    package = search_package(get_compiler_entry(), package_posit);


    // 未找到对应的Package
    // Ring-Compiler-Error-Report ERROR_CODE_UNKNOW_PACKAGE
    if (package == nullptr) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
            "unknow package `%s`; E:%d.",
            package_posit,
            ERROR_CODE_UNKNOW_PACKAGE);

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(line_number),
            .line_number             = line_number,
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }

    return package;
}

// -----------------
/*
 * resolve_variable 递归搜索一个变量
 *
 * 搜索顺序：
 * 1. 在当前Block搜索定义的局部变量
 * 2. 在当前Block搜素自由变量
 * 3. 去上一级搜索... 步骤为1.2
 * 4. 去全局变量搜索
 */
Variable* resolve_variable(Package* package, char* identifier, Block* block) {

    Variable* var = nullptr;

    var           = resolve_variable_recur(package, identifier, block);
    if (var != nullptr) {
        return var;
    }

    var = resolve_variable_global(package, identifier, block);
    return var;
}

Variable* resolve_variable_global(Package* package, char* identifier, Block* block) {
    if (package == nullptr) {
        return nullptr;
    }


    VarDecl* var_decl = nullptr;

    // 搜索全局变量
    for (VarDecl* pos : package->global_var_decl_list) {
        if (str_eq(identifier, pos->identifier)) {
            var_decl = pos;
            break;
        }
    }
    if (var_decl == nullptr) {
        return nullptr;
    }

    // TODO: 每次都创建新内存，不能复用
    Variable* variable        = (Variable*)mem_alloc(get_front_mem_pool(), sizeof(Variable));
    variable->decl            = var_decl;
    variable->is_free_value   = false;
    variable->free_value_desc = nullptr;

    // variable 添加到 block 中，方便后续直接跳过
    if (block != nullptr) {
        block->visable_var_size++;
        block->visable_var_list = variable_list_add_item(block->visable_var_list, variable);
    }

    return variable;
}

Variable* resolve_variable_recur(Package* package, char* identifier, Block* block) {

    if (block == nullptr) {
        return nullptr;
    }

    // 只有在 function直属的代码块中允许定义局部变量
    // if/for 这种代码块中，不允许自定义局部变量
    // 只能去上一级找
    // 同时 visable_var_list 只有在 BLOCK_TYPE_FUNCTION 中才有效
    if (block->type != BLOCK_TYPE_FUNCTION) {
        return resolve_variable_recur(package, identifier, block->parent_block);
    }

    VarDecl* decl = nullptr;

    // Step-0. 先看看是否已经直接resolve过
    // 可直接返回，不用再次递归查找一遍了
    for (Variable* pos = block->visable_var_list; pos != nullptr; pos = pos->next) {
        if (str_eq(pos->decl->identifier, identifier)) {
            return pos;
        }
    }


    // Step-1. 先找局部定义的变量
    for (VarDecl* pos = block->var_decl_list; pos; pos = pos->next) {
        if (str_eq(identifier, pos->identifier)) {
            decl = pos;
            {
                Variable* variable        = (Variable*)mem_alloc(get_front_mem_pool(), sizeof(Variable));
                variable->decl            = decl;
                variable->is_free_value   = false;
                variable->free_value_desc = nullptr;

                // 添加到 block->visable_var_list 中
                block->visable_var_size++;
                block->visable_var_list = variable_list_add_item(block->visable_var_list, variable);

                return variable;
            }
        }
    }

    // Step-2.
    // 继续向 block上搜索
    // 如果跨了 function-block, 那就是个FreeValue
    // 如果搜索到了，就添加到 parent_block 的 visable_var_list中
    Variable* par_var = resolve_variable_recur(package, identifier, block->parent_block);
    if (par_var == nullptr) {
        return nullptr;
    }

    if (par_var->decl != nullptr) {
        // 定义par_var 的block 需要添加一个free_value, 不能重复
        // par_block 所在的function 在return的时候, 要负责关闭所有的FreeValueAgent

        Block* par_block = par_var->decl->blong_block;

        bool   duplicate = false;

        for (FreeValueDesc* pos  = par_block->free_value_list;
             pos != nullptr; pos = pos->next) {
            if (str_eq(pos->identifier, identifier)) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) {
            // printf("+++\n");
            FreeValueDesc* free_value      = (FreeValueDesc*)mem_alloc(get_front_mem_pool(), sizeof(FreeValueDesc));
            free_value->identifier         = identifier;
            free_value->is_curr_local      = true;                          // 这里很关键
            free_value->u.curr_local_index = par_var->decl->variable_index; // 这里很关键
            free_value->free_value_index   = par_block->free_value_size;
            free_value->next               = nullptr;

            // 将这个 FreeValueDesc 添加到 block 的 free_value_list 中
            par_block->free_value_size++;
            par_block->free_value_list = free_value_list_add_item(par_block->free_value_list, free_value);

            par_var->free_value_desc   = free_value;
        }
    }

    // 因为能走到这里的 都是 BLOCK_TYPE_FUNCTION
    // 肯定是 Free_Value
    bool is_free_value = true;

    assert(par_var->free_value_desc != nullptr);


    Variable* variable        = (Variable*)mem_alloc(get_front_mem_pool(), sizeof(Variable));
    variable->decl            = par_var->decl;
    variable->is_free_value   = is_free_value;
    variable->free_value_desc = nullptr;
    if (is_free_value) {
        FreeValueDesc* free_value          = (FreeValueDesc*)mem_alloc(get_front_mem_pool(), sizeof(FreeValueDesc));
        free_value->identifier             = identifier;
        free_value->is_curr_local          = false;                                      // 这里很关键
        free_value->u.out_free_value_index = par_var->free_value_desc->free_value_index; // 这里很关键
        free_value->free_value_index       = block->free_value_size;
        free_value->next                   = nullptr;

        // 将这个 FreeValueDesc 添加到 block 的 free_value_list 中
        block->free_value_size++;
        block->free_value_list    = free_value_list_add_item(block->free_value_list, free_value);

        variable->free_value_desc = free_value;
    }
    // 添加到 block->visable_var_list 中
    block->visable_var_size++;
    block->visable_var_list = variable_list_add_item(block->visable_var_list, variable);

    return variable;
}


Function* search_function(Package* package, char* identifier) {
    assert(package != nullptr);
    for (auto function : package->function_list) {
        if (str_eq(function->identifier, identifier)) {
            return function;
        }
    }
    return nullptr;
}

FreeValueDesc* free_value_list_add_item(FreeValueDesc* head, FreeValueDesc* free_value) {
    if (head == nullptr) {
        return free_value;
    }

    FreeValueDesc* pos = head;
    for (; pos->next != nullptr; pos = pos->next) {
    }
    pos->next = free_value;
    return head;
}

Variable* variable_list_add_item(Variable* head, Variable* variable) {
    if (head == nullptr) {
        return variable;
    }

    Variable* pos = head;
    for (; pos->next != nullptr; pos = pos->next) {
    }
    pos->next = variable;
    return head;
}

std::vector<TypeSpecifier*> get_type_specifier_from_expression_list(Expression* expression) {
    std::vector<TypeSpecifier*> type_specifier_list;
    for (Expression* pos = expression; pos; pos = pos->next) {
        for (unsigned int i = 0; i < pos->convert_type_size; i++) {
            type_specifier_list.push_back(pos->convert_type[i]);
        }
    }
    return type_specifier_list;
}
std::vector<TypeSpecifier*> get_type_specifier_from_range_expression(RangeExpression* expression) {
    std::vector<TypeSpecifier*> type_specifier_list;
    for (unsigned int i = 0; i < expression->convert_type_size; i++) {
        type_specifier_list.push_back(expression->convert_type[i]);
    }
    return type_specifier_list;
}
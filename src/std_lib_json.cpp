#include "ring.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


std::string rvm_value_json_encode(RVM_Value* value,
                                  const int  indent,
                                  const char indent_char) {

    try {
        json j = rvm_value_2_json(value);
        return j.dump(indent, indent_char).c_str();
    } catch (const std::exception& e) {
        return std::string("rvm_value_json_encode failed:") + e.what();
    }
}

RVM_Value rvm_value_json_decode(Ring_VirtualMachine* rvm,
                                char*                str,
                                unsigned int         length,
                                RVM_Value*           expect) {

    // TODO: 后续优化掉
    std::string tmp(str, length);
    json        j = json::parse(tmp);
    return json_2_rvm_value(rvm, j, expect);
}

// 将 RVM_Value 转换为 nlohmann::json
json rvm_value_2_json(RVM_Value* value) {
    switch (value->type) {
    case RVM_VALUE_TYPE_BOOL:
        return json(bool(value->u.bool_value));
    case RVM_VALUE_TYPE_INT:
        return json(value->u.int_value);
    case RVM_VALUE_TYPE_INT64:
        return json(value->u.int64_value);
    case RVM_VALUE_TYPE_DOUBLE:
        return json(value->u.double_value);
    case RVM_VALUE_TYPE_STRING: {
        std::string tmp(value->u.string_value->data, value->u.string_value->length);
        return json(tmp);
    }
    case RVM_VALUE_TYPE_CLASS_OB:
        return rvm_class_ob_2_json(value->u.class_ob_value);
    case RVM_VALUE_TYPE_ARRAY:
        return rvm_array_2_json(value->u.array_value);
    default:
        return json(nullptr); // 无法识别的类型返回 null
    }
}

json rvm_class_ob_2_json(RVM_ClassObject* obj) {
    json j = json::object();
    if (!obj)
        return j;

    for (unsigned int i = 0; i < obj->field_count; i++) {
        const char* field_name = CLASS_GET_FIELD_IDENT(obj->class_def, i);
        j[field_name]          = rvm_value_2_json(&obj->field_list[i]);
    }
    return j;
}


json rvm_array_2_json(RVM_Array* arr) {
    json j = json::array();
    if (!arr) {
        return j;
    }

    for (unsigned int i = 0; i < arr->length; i++) {
        switch (arr->type) {
        case RVM_ARRAY_BOOL:
            j.push_back(arr->u.bool_array[i]);
            break;
        case RVM_ARRAY_INT:
            j.push_back(arr->u.int_array[i]);
            break;
        case RVM_ARRAY_INT64:
            j.push_back(arr->u.int64_array[i]);
            break;
        case RVM_ARRAY_DOUBLE:
            j.push_back(arr->u.double_array[i]);
            break;
        case RVM_ARRAY_STRING: {
            std::string tmp(arr->u.string_array[i]->data, arr->u.string_array[i]->length);
            j.push_back(tmp);
        } break;
        case RVM_ARRAY_CLASS_OBJECT:
            j.push_back(rvm_class_ob_2_json(arr->u.class_ob_array[i]));
            break;
        case RVM_ARRAY_A:
            j.push_back(rvm_array_2_json(arr->u.a_array[i]));
            break;
        default:
            break;
        }
    }

    return j;
}


RVM_Value json_2_rvm_value(Ring_VirtualMachine* rvm,
                           const json&          j,
                           RVM_Value*           expect) {

    RVM_Value value;

    switch (expect->type) {
    case RVM_VALUE_TYPE_BOOL:
        value.type         = RVM_VALUE_TYPE_BOOL;
        value.u.bool_value = (RVM_Bool)j.get<bool>();
        break;
    case RVM_VALUE_TYPE_INT:
        value.type        = RVM_VALUE_TYPE_INT;
        value.u.int_value = j.get<int>();
        break;
    case RVM_VALUE_TYPE_INT64:
        value.type          = RVM_VALUE_TYPE_INT64;
        value.u.int64_value = j.get<long long>();
        break;
    case RVM_VALUE_TYPE_DOUBLE:
        value.type           = RVM_VALUE_TYPE_DOUBLE;
        value.u.double_value = j.get<double>();
        break;
    case RVM_VALUE_TYPE_STRING: {
        RVM_String* string_value = rvm_gc_new_rvm_string(rvm, j.get<std::string>().c_str());
        value.type               = RVM_VALUE_TYPE_STRING;
        value.u.string_value     = string_value;
    } break;
    case RVM_VALUE_TYPE_CLASS_OB: {
        RVM_ClassDefinition* rvm_class_definition = expect->u.class_ob_value->class_def;

        value.type                                = RVM_VALUE_TYPE_CLASS_OB;
        value.u.class_ob_value                    = json_2_rvm_class_ob(rvm, j, rvm_class_definition);
    } break;
    case RVM_VALUE_TYPE_ARRAY: {
        value.type          = RVM_VALUE_TYPE_ARRAY;
        value.u.array_value = json_2_rvm_array(rvm, j,
                                               expect->u.array_value->dimension,
                                               expect->u.array_value->type,
                                               expect->u.array_value->item_type_kind,
                                               expect->u.array_value->class_def);
    } break;
    case RVM_VALUE_TYPE_CLOSURE:
        break;
    default:
        throw std::logic_error("Unsupported type");
    }


    return value;
}


RVM_ClassObject* json_2_rvm_class_ob(Ring_VirtualMachine* rvm,
                                     const json&          j,
                                     RVM_ClassDefinition* rvm_class_definition) {

    if (!j.is_object()) {
        throw std::logic_error("Expected JSON object");
    }

    RVM_ClassObject* class_ob = rvm_gc_new_class_ob_meta(rvm);

    // TODO:
    // 这里内存空间浪费了
    // 并且发生了内存泄漏
    rvm_fill_class_ob(rvm, class_ob, rvm_class_definition);

    for (unsigned int i = 0; i < rvm_class_definition->field_size; i++) {
        RVM_Field*         field            = &rvm_class_definition->field_list[i];
        char*              field_identifier = field->identifier;
        RVM_TypeSpecifier* type_specifier   = field->type_specifier;

        if (!j.contains(field_identifier)) {
            continue;
        }

        // 获取JSON值
        const json& json_value = j[field_identifier];

        // 类型检查：验证JSON值类型是否与RVM字段类型兼容
        if (!is_json_type_compatible_with_rvm_type(json_value, type_specifier)) {
            std::string error_msg = "Type mismatch for field '";
            error_msg += std::string(rvm_class_definition->identifier) + "." + std::string(field_identifier);
            error_msg += "': JSON value type '";
            error_msg += get_json_type_string(json_value);
            error_msg += "' is not compatible with type '";
            error_msg += format_rvm_type_specifier_brief(type_specifier);
            error_msg += "'";
            throw std::logic_error(error_msg);
        }

        // 内存泄漏
        class_ob->field_list[i] = json_2_rvm_value(rvm, json_value, &class_ob->field_list[i]);
    }

    return class_ob;
}


/*
 * is_json_type_compatible_with_rvm_type
 *
 * 检查JSON值类型是否与RVM类型兼容
 */
bool is_json_type_compatible_with_rvm_type(const json& json_value, RVM_TypeSpecifier* type_specifier) {
    // 如果RVM类型是动态类型或任意类型，总是兼容
    if (!type_specifier || type_specifier->kind == RING_BASIC_TYPE_ANY) {
        return true;
    }

    switch (type_specifier->kind) {
    case RING_BASIC_TYPE_BOOL:
        return json_value.is_boolean();
    case RING_BASIC_TYPE_INT:
        return json_value.is_number_integer() && json_value <= INT_MAX && json_value >= INT_MIN;
    case RING_BASIC_TYPE_INT64:
        return json_value.is_number_integer();
    case RING_BASIC_TYPE_DOUBLE:
        return json_value.is_number();
    case RING_BASIC_TYPE_STRING:
        return json_value.is_string();
    case RING_BASIC_TYPE_ARRAY:
        return json_value.is_array();
    case RING_BASIC_TYPE_CLASS:
        // 对于类类型，只检查JSON是否是对象
        return json_value.is_object();
    default:
        // 对于未知类型，默认认为不兼容
        return false;
    }
}

// TODO: 这个函数动态内存分配了，需要优化，直接交给gc分配，不然会gc 计算的逻辑会分散在各处
RVM_Array* json_2_rvm_array(Ring_VirtualMachine* rvm,
                            const json&          j,
                            unsigned int         expect_array_dimension,
                            RVM_Array_Type       expect_array_type,
                            Ring_BasicType       expect_array_item_type_kind,
                            RVM_ClassDefinition* expect_array_class_def) {

    if (!j.is_array()) {
        throw std::logic_error("Expected JSON array");
    }

    if (expect_array_dimension > 1) {
        if (expect_array_type != RVM_ARRAY_A) {
            throw std::logic_error("not multi-dimenson array");
        }
    } else {
        if (expect_array_type == RVM_ARRAY_A) {
            throw std::logic_error("invalid multi-dimenson array");
        }
    }


    RVM_Array* array = rvm_gc_new_array_meta(rvm,
                                             expect_array_type,
                                             expect_array_item_type_kind,
                                             expect_array_class_def,
                                             expect_array_dimension);
    array->length    = j.size();
    array->capacity  = j.size();

    if (j.empty()) {
        return array;
    }


    unsigned int alloc_size = 0;

    if (expect_array_dimension == 1) {
        array->capacity = ROUND_UP8(array->capacity);

        switch (expect_array_type) {
        case RVM_ARRAY_BOOL:
            alloc_size          = sizeof(bool) * array->capacity;
            array->u.bool_array = (bool*)mem_alloc(rvm->data_pool, alloc_size);
            for (unsigned int i = 0; i < j.size(); i++) {
                array->u.bool_array[i] = j[i].get<bool>();
            }
            break;
        case RVM_ARRAY_INT:
            alloc_size         = sizeof(int) * array->capacity;
            array->u.int_array = (int*)mem_alloc(rvm->data_pool, alloc_size);
            for (unsigned int i = 0; i < j.size(); i++) {
                array->u.int_array[i] = j[i].get<int>();
            }
            break;
        case RVM_ARRAY_INT64:
            alloc_size           = sizeof(long long) * array->capacity;
            array->u.int64_array = (long long*)mem_alloc(rvm->data_pool, alloc_size);
            for (unsigned int i = 0; i < j.size(); i++) {
                array->u.int64_array[i] = j[i].get<long long>();
            }
            break;
        case RVM_ARRAY_DOUBLE:
            alloc_size            = sizeof(double) * array->capacity;
            array->u.double_array = (double*)mem_alloc(rvm->data_pool, alloc_size);
            for (unsigned int i = 0; i < j.size(); i++) {
                array->u.double_array[i] = j[i].get<double>();
            }
            break;
        case RVM_ARRAY_STRING:
            alloc_size            = sizeof(RVM_String*) * array->capacity;
            array->u.string_array = (RVM_String**)mem_alloc(rvm->data_pool, alloc_size);
            alloc_size            = 0; // gc-object 元信息不计入 heap的空间
            for (unsigned int i = 0; i < j.size(); i++) {
                array->u.string_array[i] = rvm_gc_new_rvm_string(rvm, j[i].get<std::string>().c_str());
            }
            break;
        case RVM_ARRAY_CLASS_OBJECT:
            alloc_size              = sizeof(RVM_ClassObject*) * array->capacity;
            array->u.class_ob_array = (RVM_ClassObject**)mem_alloc(rvm->data_pool, alloc_size);
            alloc_size              = 0; // gc-object 元信息不计入 heap的空间
            for (unsigned int i = 0; i < j.size(); i++) {
                array->u.class_ob_array[i] = json_2_rvm_class_ob(rvm, j[i], expect_array_class_def);
            }
            break;
        case RVM_ARRAY_A:
            // 不会出现这种情况
            break;

        default:
            break;
        }
    } else {
        // 递归分配多维数组
        array->type      = RVM_ARRAY_A;
        alloc_size       = sizeof(RVM_Array*) * array->capacity;
        array->u.a_array = (RVM_Array**)mem_alloc(rvm->data_pool, alloc_size);
        alloc_size       = 0; // gc-object 元信息不计入 heap的空间

        for (unsigned int i = 0; i < j.size(); i++) {
            if (expect_array_dimension - 1 == 1) {
                // 如果下一维是最后一维
                // 手动更新下
                // TODO: 这个写法不太好
                expect_array_type = (RVM_Array_Type)expect_array_item_type_kind;
            }
            RVM_Array* sub_array = json_2_rvm_array(rvm, j[i],
                                                    expect_array_dimension - 1,
                                                    expect_array_type,
                                                    expect_array_item_type_kind,
                                                    expect_array_class_def);
            array->u.a_array[i]  = sub_array;
        }
    }

    rvm_heap_alloc_size_incr(rvm, alloc_size);
    debug_rvm_heap_alloc_with_green("json_2_rvm_array array_type:%d array->capacity:%d alloc_size:%u   [heap_size:%lld]",
                                    array_type,
                                    array->capacity,
                                    alloc_size,
                                    rvm_heap_size(rvm));

    return array;
}

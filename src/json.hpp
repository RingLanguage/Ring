
#ifndef RING_JSON_INCLUDE_H
#define RING_JSON_INCLUDE_H

#include <nlohmann/json.hpp>
#include <string>
#include <variant>

using json = nlohmann::json;

namespace nlohmann {
/*
 * 默认情况下，nlohmann::json 库不支持 std::optional 类型的序列化和反序列化。
 * 需要为 std::optional<T> 提供自定义的序列化和反序列化函数。
 * 这里使用 ADL（Argument Dependent Lookup）来确保 std::optional<T> 的序列化和反序列化函数可以被正确调用。
 */
template <typename T>
struct adl_serializer<std::optional<T>> {

    static void to_json(json& j, const std::optional<T>& opt) {
        if (opt.has_value()) {
            j = *opt;
        } else {
            j = nullptr;
        }
    }

    static void from_json(const json& j, std::optional<T>& opt) {
        if (j.is_null()) {
            opt = std::nullopt;
        } else {
            opt = j.get<T>();
        }
    }
};
} // namespace nlohmann

struct JsonError {
    std::string message;
    enum class Type {
        InvalidJson,
        TypeMismatch,
        OutOfRange,
        ParseError,
        Unknown
    } type;

    explicit JsonError(const std::string& msg, Type t = Type::Unknown) :
        message(msg), type(t) {
    }

    operator std::string() const {
        return message;
    }
};

template <typename T>
using DeserializeResult = std::variant<T, JsonError>;

using SerializeResult   = std::variant<std::string, JsonError>;

template <typename T>
DeserializeResult<T> json_decode(const std::string& json_str) {

    if (!json::accept(json_str)) {
        return JsonError{"json_decode::invalid json format", JsonError::Type::InvalidJson};
    }

    try {
        json j = json::parse(json_str);
        return j.get<T>();
    } catch (const json::type_error& e) {
        return JsonError{
            std::string("json_decode::type error: ") + e.what(),
            JsonError::Type::TypeMismatch,
        };
    } catch (const json::out_of_range& e) {
        return JsonError{
            std::string("json_decode::out of range: ") + e.what(),
            JsonError::Type::OutOfRange,
        };
    } catch (const json::parse_error& e) {
        return JsonError{
            std::string("json_decode::parse error: ") + e.what(),
            JsonError::Type::ParseError,
        };
    } catch (const json::exception& e) {
        return JsonError{
            std::string("json_decode::json error: ") + e.what(),
        };
    } catch (...) {
        return JsonError{"json_decode::unknown error"};
    }
}

template <typename T>
std::unique_ptr<JsonError> json_decode(const std::string& json_str, T* out) {

    if (!out) {
        return std::make_unique<JsonError>("json_decode::output pointer cannot be null");
    }

    if (!json::accept(json_str)) {
        return std::make_unique<JsonError>("json_decode::invalid json format");
    }

    try {
        json j = json::parse(json_str);
        j.get_to(*out);
        return nullptr;
    } catch (const json::type_error& e) {
        return std::make_unique<JsonError>(std::string("json_decode::type error: ") + e.what());
    } catch (const json::out_of_range& e) {
        return std::make_unique<JsonError>(std::string("json_decode::out of range: ") + e.what());
    } catch (const json::parse_error& e) {
        return std::make_unique<JsonError>(std::string("json_decode::parse error: ") + e.what());
    } catch (const json::exception& e) {
        return std::make_unique<JsonError>(std::string("json_decode::json error: ") + e.what());
    } catch (...) {
        return std::make_unique<JsonError>("json_decode::unknown error");
    }
}

template <typename T>
SerializeResult json_encode(const T& obj, int indent = -1, char indent_char = ' ') {

    try {
        json j = obj;
        return j.dump(indent, indent_char);
    } catch (const json::exception& e) {
        return JsonError{std::string("json_encode::error: ") + e.what()};
    }
}

inline std::string get_json_type_string(const json& json_value) {
    if (json_value.is_null())
        return "null";
    if (json_value.is_boolean())
        return "boolean";
    if (json_value.is_number_integer())
        return "integer";
    if (json_value.is_number_float())
        return "float";
    if (json_value.is_string())
        return "string";
    if (json_value.is_array())
        return "array";
    if (json_value.is_object())
        return "object";
    return "unknown";
}

#endif // RING_JSON_INCLUDE_H

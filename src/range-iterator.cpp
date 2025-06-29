#include "ring.hpp"
#include <memory>
#include <tuple>
#include <variant>
#include <vector>

/*
// 前置声明
class Object;
class Value; // 假设你的VM已有Value类型

// 所有可能的返回值类型
using RangeValue = std::variant<
    int64_t,                       // 单值 (1..10)
    std::tuple<int64_t, Value>,    // (index, value)
    std::tuple<std::string, Value> // (field_name, field_value)
    >;

class RangeIterator {
public:
    virtual ~RangeIterator()            = default;
    virtual bool       has_next() const = 0;
    virtual RangeValue get_next()       = 0;
    virtual void       reset() {
    }
};


class NumberRangeIterator : public RangeIterator {
    int64_t current_;
    int64_t end_;
    int64_t step_;

public:
    NumberRangeIterator(int64_t start, int64_t end, int64_t step = 1) :
        current_(start), end_(end), step_(step) {
    }
    bool has_next() const override {
        return (step_ > 0) ? (current_ < end_) : (current_ > end_);
    }
    RangeValue get_next() override {
        if (!has_next())
            throw std::runtime_error("No more elements");
        auto val = current_;
        current_ += step_;
        return val; // 返回单值
    }
    void reset() override {
        current_ = end_ - (current_ - end_); // 重置到初始状态
    }
};


class ObjectRangeIterator : public RangeIterator {
    Object*                                    obj_;
    size_t                                     field_idx_ = 0;
    std::vector<std::pair<std::string, Value>> fields_;

public:
    explicit ObjectRangeIterator(Object* obj) :
        obj_(obj) {
    }
    bool has_next() const override {
        return field_idx_ < fields_.size();
    }
    RangeValue get_next() override {
        if (!has_next())
            throw std::runtime_error("No more fields");
        auto& field = fields_[field_idx_++];
        return std::make_tuple(field.first, field.second); // (name, value)
    }
    void reset() override {
        field_idx_ = 0;
    }
};


class ArrayRangeIterator : public RangeIterator {
    const std::vector<Value>* array_;
    size_t                    index_ = 0;

public:
    explicit ArrayRangeIterator(const std::vector<Value>* arr) :
        array_(arr) {
    }

    bool has_next() const override {
        return array_ && index_ < array_->size();
    }

    RangeValue get_next() override {
        if (!has_next())
            throw std::runtime_error("Array index out of range");
        // 返回 (index, value) 对
        return std::make_tuple(
            static_cast<int64_t>(index_),
            (*array_)[index_++]);
    }

    void reset() override {
        index_ = 0;
    }
};


*/
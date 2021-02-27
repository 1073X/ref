#pragma once

#include <cassert>
#include <com/to_string.hpp>
#include <com/variant.hpp>

namespace miu::ref {

class price {
  public:
    static constexpr double epsilon() { return 0.00000001; }
    static constexpr double lower_bound() { return -1e8 + epsilon(); }
    static constexpr double upper_bound() { return -lower_bound(); }

    static constexpr price min() { return lower_bound() + epsilon(); }
    static constexpr price max() { return upper_bound() - epsilon(); }

  public:
    price() = default;

    template<typename T>
    constexpr price(T val)
        : _value(static_cast<double>(val)) {
        assert(_value > lower_bound() && "price underflow");
        assert(_value < upper_bound() && "price overflow");
    }

    operator double() const { return _value; }

  public:    // negative
    price operator-() const { return -_value; }

  public:    // equility
    bool operator!=(price rhs) const {
        auto diff = _value - rhs._value;
        return diff > epsilon() || diff < -epsilon();
    }
    auto operator==(price rhs) const { return !operator!=(rhs); }

  public:    // greater
    template<typename T>
    bool operator>(T rhs) const {
        auto diff = _value - rhs;
        return diff > epsilon();
    }
    auto operator>(price rhs) const { return operator>(rhs._value); }

    template<typename T>
    bool operator>=(T rhs) const {
        auto diff = _value - rhs;
        return diff > epsilon() || diff > -epsilon();
    }
    auto operator>=(price rhs) const { return operator>=(rhs._value); }

  public:    // less
    template<typename T>
    bool operator<(T rhs) const {
        auto diff = rhs - _value;
        return diff > epsilon();
    }
    auto operator<(price rhs) const { return operator<(rhs._value); }

    template<typename T>
    bool operator<=(T rhs) const {
        auto diff = rhs - _value;
        return diff > epsilon() || diff > -epsilon();
    }
    auto operator<=(price rhs) const { return operator<=(rhs._value); }

  public:    // addition
    template<typename T>
    price operator+(T rhs) const {
        return _value + rhs;
    }
    price operator+(price rhs) const { return operator+(rhs._value); }

    auto& operator+=(price rhs) {
        _value += rhs._value;
        return *this;
    }

  public:    // subtraction
    template<typename T>
    price operator-(T rhs) const {
        return _value - rhs;
    }
    auto operator-(price rhs) const { return operator-(rhs._value); }

    auto& operator-=(price rhs) {
        _value -= rhs._value;
        return *this;
    }

  public:    // multiplication
    template<typename T>
    price operator*(T rhs) const {
        return _value * rhs;
    }
    auto operator*(price rhs) const { return operator*(rhs._value); }

    auto& operator*=(price rhs) {
        _value *= rhs._value;
        return *this;
    }

  public:    // division
    template<typename T>
    price operator/(T rhs) const {
        assert(rhs > 0 || rhs < 0);
        return _value / rhs;
    }
    price operator/(price rhs) const { return operator/(rhs._value); }

    auto& operator/=(price rhs) {
        _value /= rhs._value;
        return *this;
    }

  private:
    double _value { 0.0 };
};
static_assert(sizeof(price) == sizeof(double));

}    // namespace miu::ref

template<typename T>
auto operator+(T lhs, miu::ref::price rhs) {
    return rhs + lhs;
}

template<typename T>
auto operator-(T lhs, miu::ref::price rhs) {
    return -rhs + lhs;
}

template<typename T>
auto operator*(T lhs, miu::ref::price rhs) {
    return rhs * lhs;
}

template<typename T>
auto operator/(T lhs, miu::ref::price rhs) {
    assert(rhs > 0.0 || rhs < 0.0);
    return miu::ref::price { lhs / static_cast<double>(rhs) };
}

DEF_VARIANT(miu::ref::price, miu::com::type_id<double>::value);
DEF_TO_STRING(miu::ref::price);

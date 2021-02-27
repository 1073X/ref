#pragma once

#include <cassert>
#include <com/to_string.hpp>

namespace miu::ref {

struct decimal {
    int32_t mantissa;
    int32_t exponent;

    auto value() const {
        assert(exponent > 0.0);
        return (double)mantissa / exponent;
    }

    bool operator>(decimal const& rhs) const { return value() > rhs.value(); }
    bool operator<(decimal const& rhs) const { return value() < rhs.value(); }

    bool operator!=(decimal const& rhs) const {
        return mantissa != rhs.mantissa || exponent != rhs.exponent;
    }
    bool operator==(decimal const& rhs) const { return !operator!=(rhs); }
};

}    // namespace miu::ref

DEF_TO_STRING(miu::ref::decimal);

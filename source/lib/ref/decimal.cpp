#include "decimal.hpp"

#include <sstream>

DEF_TO_STRING(miu::ref::decimal) {
    std::ostringstream ss;
    ss << v.mantissa << '/' << v.exponent;
    return ss.str();
}


#include "ref/price.hpp"

DEF_VAR_SET(miu::ref::price) {
    _id = type_id<double>::value;
    new (_value) double { v };
}

DEF_VAR_GET(miu::ref::price) {
    return get<double>().value();
}

DEF_TO_STRING(miu::ref::price) {
    return to_string(static_cast<double>(v));
}

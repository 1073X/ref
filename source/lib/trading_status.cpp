
#include "ref/trading_status.hpp"

#include "var_enum.hpp"

DEF_VAR_SET(miu::ref::trading_status) {
    new (_value) ref::trading_status { v };
}

DEF_VAR_GET(miu::ref::trading_status) {
    return get_enum<ref::trading_status>(this);
}

DEF_TO_STRING(miu::ref::trading_status) {
    return enum_to_str<ref::trading_status>(v);
}

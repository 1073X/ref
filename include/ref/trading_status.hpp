#pragma once

#include <com/variant.hpp>

namespace miu::ref {

enum struct trading_status : uint16_t {
    UNDEF = 0,

    AUCTION,
    PRE_OPEN,
    OPEN,
    PRE_CLOSE,
    CLOSE,

    MAX
};

}

DEF_VARIANT(miu::ref::trading_status, CUSTOM_TYPE_ID + 33);

#pragma once

#include <com/variant.hpp>

namespace miu::ref {

enum struct phase_type : uint16_t {
    UNDEF = 0,

    AUCTION,
    PRE_OPEN,
    OPEN,
    PRE_CLOSE,
    CLOSE,

    MAX
};

}

DEF_VARIANT(miu::ref::phase_type, CUSTOM_TYPE_ID + 33);

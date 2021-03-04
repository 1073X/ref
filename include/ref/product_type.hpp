#pragma once

#include <com/variant.hpp>

namespace miu::ref {

enum struct product_type : uint16_t {
    UNDEF = 0,

    STOCK,
    FUTURE,
    CALL,
    PUT,
    FOREX,
    ETF,
    WARRANT,
    SPREAD,
    BOND,
    SWAP,    // Equity SWAP
    SPOT,

    MAX
};

}

DEF_VARIANT(miu::ref::product_type, CUSTOM_TYPE_ID + 35);

#pragma once

#include <com/variant.hpp>

namespace miu::ref {

enum struct currency_type : uint16_t {
    UNDEF = 0,

    CNY,
    USD,
    USC,    // US cent
    HKD,
    JPY,
    CNH,
    EUR,
    GBP,
    GBp,    // NOLINT: Britain Pence
    AUD,

    MAX
};

}    // namespace miu::ref

DEF_VARIANT(miu::ref::currency_type, CUSTOM_TYPE_ID + 36);

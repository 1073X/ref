#pragma once

#include <com/variant.hpp>

namespace miu::ref {

enum struct status : uint16_t { AUCTION, PRE_OPEN, OPEN, PRE_CLOSE, CLOSE, MAX };

}

DEF_VARIANT(miu::ref::status, CUSTOM_TYPE_ID + 33);

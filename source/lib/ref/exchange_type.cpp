
#include "ref/exchange_type.hpp"

#include "var_enum.hpp"

DEF_VAR_SET(miu::ref::exchange_type) {
    new (_value) ref::exchange_type { v };
}

DEF_VAR_GET(miu::ref::exchange_type) {
    return get_enum<ref::exchange_type>(this);
}

DEF_TO_STRING(miu::ref::exchange_type) {
    return enum_to_str<ref::exchange_type>(v);
}

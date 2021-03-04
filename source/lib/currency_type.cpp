
#include "ref/currency_type.hpp"

#include "var_enum.hpp"

DEF_VAR_SET(miu::ref::currency_type) {
    new (_value) ref::currency_type { v };
}

DEF_VAR_GET(miu::ref::currency_type) {
    return get_enum<ref::currency_type>(this);
}

DEF_TO_STRING(miu::ref::currency_type) {
    return enum_to_str<ref::currency_type>(v);
}

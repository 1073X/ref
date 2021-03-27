
#include "ref/phase_type.hpp"

#include "var_enum.hpp"

DEF_VAR_SET(miu::ref::phase_type) {
    new (_value) ref::phase_type { v };
}

DEF_VAR_GET(miu::ref::phase_type) {
    return get_enum<ref::phase_type>(this);
}

DEF_TO_STRING(miu::ref::phase_type) {
    return enum_to_str<ref::phase_type>(v);
}

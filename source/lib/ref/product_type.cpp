
#include "ref/product_type.hpp"

#include "var_enum.hpp"

namespace miu::ref {

bool is_options(product_type v) {
    return product_type::CALL == v || product_type::PUT == v;
}

bool is_derivatives(product_type v) {
    return product_type::FUTURE == v || is_options(v) || product_type::SPREAD == v;
}

}    // namespace miu::ref

DEF_VAR_SET(miu::ref::product_type) {
    new (_value) ref::product_type { v };
}

DEF_VAR_GET(miu::ref::product_type) {
    return get_enum<ref::product_type>(this);
}

DEF_TO_STRING(miu::ref::product_type) {
    return enum_to_str<ref::product_type>(v);
}

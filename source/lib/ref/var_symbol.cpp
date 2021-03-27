
#include "ref/symbol.hpp"

DEF_VAR_SET(miu::ref::symbol) {
    new (_value) ref::symbol { v };
}

DEF_VAR_GET(miu::ref::symbol) {
    switch (id()) {
    case type_id<ref::symbol>::value:
        return *(ref::symbol const*)_value;
    case type_id<uint64_t>::value:
        return *(uint64_t const*)_value;
    default:
        return std::nullopt;
    }
}

DEF_TO_STRING(miu::ref::symbol) {
    return v.str();
}

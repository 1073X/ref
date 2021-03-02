
#include "ref/status.hpp"

#include <com/enum.hpp>
#include <functional>

DEF_VAR_SET(miu::ref::status) {
    new (_value) ref::status { v };
}

DEF_VAR_GET(miu::ref::status) {
    switch (id()) {
    case type_id<int8_t>::value:
    case type_id<int16_t>::value:
    case type_id<int32_t>::value:
    case type_id<int64_t>::value:
        return com::val_to_enum<ref::status>(get<int64_t>().value());
    case type_id<uint8_t>::value:
    case type_id<uint16_t>::value:
    case type_id<uint32_t>::value:
    case type_id<uint64_t>::value:
        return com::val_to_enum<ref::status>(get<uint64_t>().value());
    case type_id<ref::status>::value:
        return *(ref::status const*)_value;
    case type_id<const char*>::value:
    case type_id<std::string>::value:
        return com::val_to_enum<ref::status>(get<std::string>().value());
    default:
        return std::nullopt;
    }
}

DEF_TO_STRING(miu::ref::status) {
    return miu::com::enum_to_str<miu::ref::status>(v);
}

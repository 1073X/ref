#pragma once

#include <com/enum.hpp>
#include <com/variant.hpp>

namespace miu::com {

template<typename T>
std::optional<T> get_enum(variant const* var) {
    switch (var->id()) {
    case type_id<int8_t>::value:
    case type_id<int16_t>::value:
    case type_id<int32_t>::value:
    case type_id<int64_t>::value:
        return com::val_to_enum<T>(var->get<int64_t>().value());
    case type_id<uint8_t>::value:
    case type_id<uint16_t>::value:
    case type_id<uint32_t>::value:
    case type_id<uint64_t>::value:
        return com::val_to_enum<T>(var->get<uint64_t>().value());
    case type_id<T>::value:
        return *(T const*)var;
    case type_id<const char*>::value:
    case type_id<std::string>::value:
        return com::val_to_enum<T>(var->get<std::string>().value());
    default:
        return std::nullopt;
    }
}

}    // namespace miu::com


#include "ref/signature.hpp"

#include <cassert>
#include <cstring>    // std::memcpy, std::memcmp
#include <iomanip>
#include <sstream>

namespace miu::ref {

signature::signature(uint8_t const* data) {
    std::memcpy(_data, data, size());
}

bool signature::operator!=(signature const& rhs) const {
    return std::memcmp(_data, rhs._data, size()) != 0;
}

uint8_t signature::operator[](uint32_t i) const {
    assert(i < size() && "read signature overflow");
    return _data[i];
}

}    // namespace miu::ref

DEF_TO_STRING(miu::ref::signature) {
    std::ostringstream ss;
    for (auto i = 0U; i < miu::ref::signature::size(); i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)v[i];
    }
    return ss.str();
}

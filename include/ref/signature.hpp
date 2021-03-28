#pragma once

#include <com/to_string.hpp>

namespace miu::ref {

class signature {
  public:
    static auto constexpr size() { return sizeof(_data); }

    bool operator!=(signature const&) const;
    auto operator==(signature const& rhs) const { return !operator!=(rhs); }

    signature(uint8_t const*);
    uint8_t operator[](uint32_t i) const;

  private:
    uint8_t _data[16];
};

}    // namespace miu::ref

DEF_TO_STRING(miu::ref::signature);

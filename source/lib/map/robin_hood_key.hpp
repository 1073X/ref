#pragma once

#include <cstdint>

namespace miu::ref {

template<typename, std::size_t>
class robin_hood_key;

template<typename T>
class robin_hood_key<T, 8> {
  public:
    robin_hood_key() = default;

    robin_hood_key(T const& key) {
        auto src = (uint64_t const*)&key;
        _value   = src[0];
    }

    auto operator!=(T const& key) const {
        auto rhs = (uint64_t const*)&key;
        return _value != rhs[0];
    }
    auto operator==(T const& rhs) const { return !operator!=(rhs); }

  private:
    uint64_t _value { 0 };
};

template<typename T>
class robin_hood_key<T, 16> {
  public:
    robin_hood_key() = default;

    robin_hood_key(T const& key) {
        auto src  = (uint64_t const*)&key;
        _val_low  = src[0];
        _val_high = src[1];
    }

    auto operator!=(T const& key) const {
        auto rhs = (uint64_t const*)&key;
        return _val_low != rhs[0] || _val_high != rhs[1];
    }
    auto operator==(T const& rhs) const { return !operator!=(rhs); }

  private:
    uint64_t _val_low { 0 };
    uint64_t _val_high { 0 };
};

}    // namespace miu::ref

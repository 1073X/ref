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
        _val[0]  = src[0];
    }

    auto operator!=(robin_hood_key const& rhs) const { return _val[0] != rhs._val[0]; }
    auto operator==(robin_hood_key const& rhs) const { return !operator!=(rhs); }
    auto operator!=(T const& rhs) const { return operator!=(*(robin_hood_key const*)&rhs); }

    T const& get() const { return *(T const*)_val; }

  private:
    uint64_t _val[1] { 0 };
};

template<typename T>
class robin_hood_key<T, 16> {
  public:
    robin_hood_key() = default;

    robin_hood_key(T const& key) {
        auto src = (uint64_t const*)&key;
        _val[0]  = src[0];
        _val[1]  = src[1];
    }

    auto operator!=(robin_hood_key const& rhs) const {
        return _val[0] != rhs._val[0] || _val[1] != rhs._val[1];
    }
    auto operator==(robin_hood_key const& rhs) const { return !operator!=(rhs); }
    auto operator!=(T const& rhs) const { return operator!=(*(robin_hood_key const*)&rhs); }

    T const& get() const { return *(T const*)_val; }

  private:
    uint64_t _val[2] { 0, 0 };
};

}    // namespace miu::ref

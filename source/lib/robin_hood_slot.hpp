#pragma once

#include "robin_hood_key.hpp"

namespace miu::ref {

template<typename, std::size_t>
class robin_hood_slot;

template<typename T>
class robin_hood_slot<T, 8> {
  public:
    using key_type = robin_hood_key<T, 8>;

    robin_hood_slot() = default;
    robin_hood_slot(T const& key, uint32_t val)
        : _key { key }
        , _val { val }
        , _psl { 1 } {}

    auto const& key() const { return _key; }
    auto val() const { return _val; }

    auto psl() const { return _psl; }
    auto inc_psl() { _psl++; }

    auto used() const { return _psl > 0; }

  private:
    key_type _key;
    uint32_t _val { 0 };
    uint32_t _psl { 0 };
};

template<typename T>
class robin_hood_slot<T, 16> {
  public:
    using key_type = robin_hood_key<T, 16>;

    robin_hood_slot() = default;
    robin_hood_slot(T const& key, uint32_t val)
        : _key { key }
        , _val { val }
        , _psl { 1 } {}

    auto const& key() const { return _key; }
    auto val() const { return _val; }

    auto psl() const { return _psl; }
    auto inc_psl() { _psl++; }

    auto used() const { return _psl > 0; }

  private:
    key_type _key;
    uint32_t _val { 0 };
    uint32_t _psl { 0 };
    uint32_t _padding[2];
};

}    // namespace miu::ref

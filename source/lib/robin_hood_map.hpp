#pragma once

#include "robin_hood_hash.hpp"
#include "robin_hood_slot.hpp"

namespace miu::ref {

template<typename K, typename H = robin_hood_hash<K>>
class robin_hood_map {
  public:
    using slot_type = robin_hood_slot<K, sizeof(K)>;
    using key_type  = typename slot_type::key_type;

  public:
    static auto resolve_size(std::size_t slots) {
        static_assert(sizeof(slot_type) == sizeof(robin_hood_map));
        return sizeof(slot_type) * slots + sizeof(robin_hood_map);
    }

    static auto make(void* buf, std::size_t len) {
        auto map       = new (buf) robin_hood_map {};
        map->_capacity = len / sizeof(slot_type) - 1;
        map->_size     = 0;
        return map;
    }

    static auto open(const void* buf, std::size_t len) {
        return reinterpret_cast<robin_hood_map const*>(buf);
    }

    auto capacity() const { return _capacity; }
    auto size() const { return _size; }
    auto max_psl() const { return _max_psl; }

    template<typename T>
    auto insert(T const& key, uint32_t val) {
        assert(_size < _capacity && "robin_hood_map overflow");

        auto new_s = slot_type { key, val + 1 };
        insert(&new_s, H {}(key));
        _size++;
    }

    template<typename T>
    auto lookup(T const& key) const {
        auto idx = H {}(key);
        while (at(idx)->used() && at(idx)->key() != key) {
            idx++;
        }
        return at(idx)->val() - 1;
    }

  private:
    robin_hood_map() = default;

    void insert(slot_type* new_s, uint32_t idx) {
        while (new_s->psl() <= at(idx)->psl()) {
            new_s->inc_psl();
            idx++;
        }
        _max_psl = std::max(_max_psl, new_s->psl());
        std::swap(*new_s, *at(idx));

        if (new_s->used()) {
            new_s->inc_psl();
            idx++;
            insert(new_s, idx);
        }
    }

    slot_type* at(uint32_t idx) { return (slot_type*)(this + 1) + idx % _capacity; }
    slot_type const* at(uint32_t idx) const { return const_cast<robin_hood_map*>(this)->at(idx); }

  private:
    uint32_t _capacity;
    uint32_t _size;
    uint32_t _max_psl { 0 };
    uint8_t _paddings[sizeof(slot_type) - 12];
};

}    // namespace miu::ref


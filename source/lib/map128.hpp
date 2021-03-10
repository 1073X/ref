#pragma once

namespace miu::ref {

template<typename K, typename H>
class map128 {
    static_assert(sizeof(K) == 16);    // yeah, im map with 128bits key

  public:
    class key {
      public:
        key() {
            auto dst = (uint64_t*)&_key;
            dst[0]   = 0;
            dst[1]   = 0;
        }

        key(K const& key) {
            auto src = (uint64_t const*)&key;
            auto dst = (uint64_t*)&_key;
            dst[0]   = src[0];
            dst[1]   = src[1];
        }

        auto operator!=(K const& key) const {
            auto lhs = (uint64_t*)&_key;
            auto rhs = (uint64_t const*)&key;
            return lhs[0] != rhs[0] || lhs[1] != rhs[1];
        }
        auto operator==(K const& rhs) const { return !operator!=(rhs); }

      private:
        K _key;
    };

    class slot {
      public:
        slot() = default;
        slot(K const& key, uint16_t val)
            : _key { key }
            , _val { val }
            , _psl { 1 } {}

        auto const& key() const { return _key; }
        auto val() const { return _val; }

        auto psl() const { return _psl; }
        auto inc_psl() { _psl++; }

        auto used() const { return _psl > 0; }

      private:
        class key _key;
        uint32_t _val { 0 };
        uint32_t _psl { 0 };
        uint32_t _paddings[2];
    };
    static_assert(sizeof(slot) == 32);

  public:
    static auto make(char* buf, std::size_t len) {
        auto map       = new (buf) map128 {};
        map->_capacity = len / sizeof(slot) - 1;
        map->_size     = 0;
        return map;
    }

    static auto open(const char* buf, std::size_t len) {
        return reinterpret_cast<map128 const*>(buf);
    }

    auto capacity() const { return _capacity; }
    auto size() const { return _size; }
    auto max_psl() const { return _max_psl; }

    template<typename T>
    auto insert(T const& key, uint32_t val) {
        assert(_size < _capacity && "map128 overflow");

        auto new_s = slot { key, val + 1 };
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
    map128() = default;

    void insert(slot* new_s, uint32_t idx) {
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

    slot* at(uint32_t idx) { return (slot*)(this + 1) + idx % _capacity; }
    slot const* at(uint32_t idx) const { return const_cast<map128*>(this)->at(idx); }

  private:
    uint32_t _capacity;
    uint32_t _size;
    uint32_t _max_psl { 0 };
    uint32_t _paddings[5];
};

}    // namespace miu::ref


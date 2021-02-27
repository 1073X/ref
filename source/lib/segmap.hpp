#pragma once

#include <array>
#include <cassert>
#include <com/fatal_error.hpp>
#include <tuple>

namespace miu::ref {

template<typename KEY, typename VAL, uint32_t N>
class segmap {
  public:
    static uint32_t constexpr max_seg() { return N; }

    explicit segmap(KEY min_key, KEY max_key, VAL defval) {
        assert(min_key < max_key);
        std::fill_n(_vals.begin(), max_seg(), defval);
        std::fill_n(_keys.begin(), max_seg(), max_key);
        _keys[0] = min_key;
    }

    auto operator==(segmap const& lhs) const {
        for (auto i = 0U; i < N; i++) {
            if (!std::equal_to<KEY>()(_keys[i], lhs._keys[i])
                || !std::equal_to<VAL>()(_vals[i], lhs._vals[i])) {
                return false;
            }
        }
        return true;
    }

    void add(KEY key, VAL val) {
        for (auto i = 1U; i < max_seg() - 1; i++) {
            if (_keys[i] == key) {
                if (_vals[i] > val || _vals[i] < val) {
                    FATAL_ERROR("segmap key conflicted",
                                com::to_string(key),
                                "old",
                                com::to_string(_vals[i]),
                                "new",
                                com::to_string(val));
                }
            } else if (_keys[i] > key) {
                if (!(_vals[i - 1] > val || _vals[i - 1] < val)) {
                    // ommit it since the previous key
                    // has same val
                    return;
                }
                if (!(_vals[i] > val || _vals[i] < val)) {
                    _keys[i] = key;
                    return;
                }

                for (auto j = max_seg() - 1; j > i; j--) {
                    _keys[j] = _keys[j - 1];
                    _vals[j] = _vals[j - 1];
                }
                _keys[i] = key;
                _vals[i] = val;

                return;
            }
        }

        FATAL_ERROR("segmap overflow", max_seg());
    }

    VAL get(KEY key) const {
        for (auto i = 1U; i < max_seg(); i++) {
            if (_keys[i] > key) {
                return _vals[i - 1];
            }
        }

        return _vals[0];
    }

    uint32_t segements() const {
        for (auto i = 1U; i < max_seg(); i++) {
            if (_keys[i] == _keys[max_seg() - 1]) {
                return i;
            }
        }

        FATAL_ERROR("segmap overflow", max_seg());
        return 0;
    }

    // Return current value and next key
    std::tuple<KEY, VAL> locate(KEY key) const {
        for (auto i = 1U; i < max_seg(); i++) {
            if (_keys[i] > key) {
                return std::make_tuple(_keys[i], _vals[i - 1]);
            }
        }

        auto max_key = std::numeric_limits<KEY>::max();
        return std::make_tuple(max_key, _vals[0]);
    }

    std::array<KEY, N> const& keys() const { return _keys; }
    std::array<VAL, N> const& vals() const { return _vals; }

  private:
    std::array<KEY, N> _keys;
    std::array<VAL, N> _vals;
};

}    // namespace miu::ref


#include "tick_table.hpp"

namespace miu::ref {
tick_table::tick_table()
    : details::tick_table(0.0, ref::price::max(), decimal { 1, 1 }) {
}

int32_t tick_table::precision() const {
    auto max_exponent = 0;
    for (auto i = 1U; i < max_seg(); i++) {
        auto [man, exponent] = vals()[i - 1];
        max_exponent         = std::max(exponent, max_exponent);
    }

    auto ret = 0;
    while (max_exponent > 1) {
        max_exponent /= 10;
        ret++;
    }
    return ret;
}

void tick_table::add(price base, price tick) {
    auto exponent = 1;
    while (std::floor(tick * exponent) / exponent < tick) {
        exponent *= 10;
    }

    int32_t itick = std::round(tick * exponent);
    details::tick_table::add(base, decimal { itick, exponent });
}

price tick_table::up(price prc, int32_t n, uint32_t i) const {
    double val = prc;

    if (val < 0.0) {
        return -dn(-val, n);
    }

    if (i >= max_seg()) {
        return price::max();
    }

    double key = keys()[i];
    if (val < key) {
        auto [mantissa, exponent] = vals()[i - 1];

        auto tmp     = val * exponent / mantissa - price::epsilon();
        auto idx     = std::ceil(tmp) + n;
        auto max_idx = key * exponent / mantissa;
        if (idx <= max_idx) {
            return idx * mantissa / exponent;
        }

        n   = idx - max_idx;
        val = key;
    }

    return up(val, n, i + 1);
}

price tick_table::dn(price prc, int32_t n, uint32_t i) const {
    double val = prc;

    if (val <= 0.0) {
        return -up(-val, n);
    }

    assert(i < max_seg());

    double key = keys()[i];
    if (val > key) {
        auto [mantissa, exponent] = vals()[i];

        auto tmp     = val * exponent / mantissa + price::epsilon();
        auto idx     = std::floor(tmp) - n;
        auto min_idx = key * exponent / mantissa;
        if (idx > min_idx) {
            return idx * mantissa / exponent;
        }

        n   = min_idx - idx;
        val = key;
    }
    return dn(val, n, i - 1);
}

int32_t tick_table::to_tick_count(price val) const {
    if (val < 0.0) {
        return -to_tick_count(-val);
    }

    price cnt = 0.0;    // prevent int/double casting
    for (auto i = 1U; i < max_seg(); i++) {
        auto [mantissa, exponent] = vals()[i - 1];
        auto prev_key             = keys()[i - 1];

        auto key = keys()[i];
        if (val > key) {
            cnt = cnt + (key - prev_key) * exponent / mantissa;
        } else {
            cnt = cnt + (val - prev_key) * exponent / mantissa;
            break;
        }
    }
    return std::round(cnt);
}

int32_t tick_table::distance(price val1, price val2) const {
    return to_tick_count(val2) - to_tick_count(val1);
}

}    // namespace miu::ref

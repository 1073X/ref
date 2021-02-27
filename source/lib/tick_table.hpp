#pragma once

#include <cmath>
#include <tuple>

#include "ref/price.hpp"

#include "decimal.hpp"
#include "segmap.hpp"

namespace miu::ref {

namespace details {
    using tick_table = segmap<price, decimal, 11>;
}

class tick_table : public details::tick_table {
  public:
    tick_table();

    int32_t precision() const;

    void add(price base, price tick);

    price up(price prc, int32_t n, uint32_t i = 1U) const;
    price dn(price prc, int32_t n, uint32_t i = max_seg() - 1) const;

    int32_t to_tick_count(price) const;
    int32_t distance(price, price) const;
};

}    // namespace miu::ref


#pragma once

#include <time/daytime.hpp>

#include "currency_type.hpp"
#include "phase_type.hpp"
#include "symbol.hpp"

namespace miu::ref {

class layout;

class instrument {
  public:
    instrument() = default;
    instrument(layout const*, uint16_t id);

    auto operator!() const { return !_layout; }
    operator bool() const { return !operator!(); }

    auto id() const { return _id; }

    class symbol symbol() const;
    exchange_type exchange() const;
    product_type type() const;
    std::string name() const;

    std::string_view mkt_code() const;
    std::string_view trd_code() const;

    currency_type currency() const;
    int32_t lot_size() const;
    int32_t multiplier() const;

    time::date maturity() const;
    price strike_price() const;

    price tick_up(price, int32_t = 1) const;
    price tick_dn(price, int32_t = 1) const;

    phase_type determine(time::daytime) const;

  private:
    layout const* _layout { nullptr };
    uint16_t const _id { 0 };
};

}    // namespace miu::ref

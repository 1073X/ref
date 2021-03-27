#pragma once

#include "underlying_impl.hpp"

namespace miu::ref {

class instrument_impl : public referential {
  public:
    instrument_impl() = default;

    // equities
    instrument_impl(underlying_impl const& prod, std::string const& name)
        : instrument_impl(0, prod, prod.exchange(), prod.type(), name) {
        assert(!is_derivatives(prod.type()) && "not equity");
    }
    // futures
    instrument_impl(underlying_impl const& prod, time::date maturity)
        : instrument_impl(0, prod, prod.exchange(), prod.name(), maturity) {
        assert(product_type::FUTURE == prod.type() && "not future");
        _maturity = maturity;
    }
    // options
    instrument_impl(underlying_impl const& prod, price strike, time::date maturity)
        : instrument_impl(0, prod, prod.exchange(), prod.type(), prod.name(), strike, maturity) {
        assert(is_options(prod.type()) && "not options");
        _maturity = maturity;
        _strike   = strike;
    }

    int32_t lot_size() const { return _lot_size; }
    int32_t multiplier() const { return _multiplier; }
    currency_type currency() const { return _currency; }
    time::date maturity() const { return _maturity; }
    price strike_price() const { return _strike; }

    uint32_t tiktable_id() const { return _tiktable_id; }
    uint32_t schedule_id() const { return _schedule_id; }

    auto underlying() const {
        underlying_impl underlying { exchange(), type(), name() };
        underlying.set_currency(currency());
        underlying.set_lot_size(lot_size());
        underlying.set_multiplier(multiplier());
        underlying.set_tiktable_id(tiktable_id());
        underlying.set_schedule_id(schedule_id());
        return underlying;
    }

  private:
    template<typename... ARGS>
    instrument_impl(int, underlying_impl const& prod, ARGS&&... args)
        : referential { std::forward<ARGS>(args)... } {
        _tiktable_id = prod.tiktable_id();
        _schedule_id = prod.schedule_id();
        _lot_size    = prod.lot_size();
        _multiplier  = prod.multiplier();
        _currency    = prod.currency();
    }

  private:
    price _strike { 0 };
    time::date _maturity { symbol::min_maturity() };
    currency_type _currency { currency_type::MAX };
    int16_t _lot_size { 0 };
    int32_t _multiplier { 0 };
    uint16_t _tiktable_id { 0 };
    uint16_t _schedule_id { 0 };
};
static_assert(CACHE_LINE == sizeof(instrument_impl));

}    // namespace miu::ref

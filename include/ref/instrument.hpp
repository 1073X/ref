#pragma once

#include "underlying.hpp"

namespace miu::ref {

class instrument : public referential {
  public:
    // equities
    instrument(uint16_t id, underlying const& prod, std::string_view name)
        : instrument(id, prod.id(), prod.exchange(), prod.product(), name) {
        assert(!is_derivatives(prod.product()) && "not equity");
    }
    // futures
    instrument(uint16_t id, underlying const& cls, time::date maturity)
        : instrument(id, cls.id(), cls.exchange(), cls.name(), maturity) {
        assert(product_type::FUTURE == cls.product() && "not future");
        _maturity = maturity;
    }
    // options
    instrument(uint16_t id, underlying const& cls, price strike, time::date maturity)
        : instrument(id, cls.id(), cls.exchange(), cls.product(), cls.name(), strike, maturity) {
        assert(is_options(cls.product()) && "not options");
        _maturity = maturity;
        _strike   = strike;
    }

    auto id() const { return _id; }
    auto uid() const { return _uid; }

    auto maturity() const { return _maturity; }
    auto strike_price() const { return _strike; }

  private:
    template<typename... ARGS>
    instrument(uint16_t id, uint16_t uid, ARGS&&... args)
        : referential(std::forward<ARGS>(args)...) {
        _id  = id;
        _uid = uid;
    }

  private:
    price _strike { 0 };
    time::date _maturity { symbol::min_maturity() };
    uint16_t _id;
    uint16_t _uid;    // underlying id
    uint16_t _paddings[4];
};
static_assert(CACHE_LINE == sizeof(instrument));

}    // namespace miu::ref

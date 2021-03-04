#pragma once

#include <cstring>    // std::strncpy

#include "currency_type.hpp"
#include "referential.hpp"

namespace miu::ref {

class underlying : public referential {
  public:
    underlying(uint16_t id, exchange_type exchange, product_type product, std::string_view name)
        : referential(exchange, product, name) {
        _id = id;
    }

    auto id() const { return _id; }

    auto currency() const { return _currency; }
    auto set_currency(currency_type v) { _currency = v; }

    auto multiplier() const { return _multiplier; }
    auto set_multiplier(int32_t v) { _multiplier = v; }

    auto lot_size() const { return _lot_size; }
    auto set_lot_size(int32_t v) { _lot_size = v; }

    auto tick_tbl_id() const { return _tick_tbl_id; }
    auto set_tick_tbl_id(uint16_t v) { _tick_tbl_id = v; }

    auto schedule_id() const { return _schedule_id; }
    auto set_schedule_id(uint16_t v) { _schedule_id = v; }

  private:
    int32_t _multiplier { 0 };
    int32_t _lot_size { 0 };
    currency_type _currency { currency_type::UNDEF };
    uint16_t _id { 0 };
    uint16_t _tick_tbl_id { 0 };
    uint16_t _schedule_id { 0 };
    uint16_t _paddings[4];
};
static_assert(CACHE_LINE == sizeof(underlying));

}    // namespace miu::ref

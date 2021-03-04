#pragma once

#include <cstring>    // std::strncpy

#include "currency_type.hpp"
#include "symbol.hpp"

namespace miu::ref {

class underlying {
  public:
    underlying(uint16_t id, exchange_type exchange, product_type product, std::string_view name)
        : _symbol { exchange, product, name }
        , _id(id) {}

    uint16_t id() const { return _id; }
    std::string name() const { return _symbol.name(); }
    exchange_type exchange() const { return _symbol.exchange(); }
    product_type product() const { return _symbol.product(); }
    class symbol symbol() const {
        return _symbol;
    }

    std::string_view trd_code() const { return &_trd_code[0]; }
    void set_trd_code(std::string_view v) {
        assert(v.size() < sizeof(_trd_code) && "trd_code overflow");
        std::strncpy(_trd_code, v.data(), sizeof(_trd_code));
    }

    std::string_view mkt_code() const { return &_mkt_code[0]; }
    void set_mkt_code(std::string_view v) {
        assert(v.size() < sizeof(_mkt_code) && "mkt_code overflow");
        std::strncpy(_mkt_code, v.data(), sizeof(_mkt_code));
    }

    currency_type currency() const { return _currency; }
    void set_currency(currency_type v) { _currency = v; }

    int32_t multiplier() const { return _multiplier; }
    void set_multiplier(int32_t v) { _multiplier = v; }

    int32_t lot_size() const { return _lot_size; }
    void set_lot_size(int32_t v) { _lot_size = v; }

    uint16_t tick_tbl_id() const { return _tick_tbl_id; }
    void set_tick_tbl_id(uint16_t v) { _tick_tbl_id = v; }

    uint16_t schedule_id() const { return _schedule_id; }
    void set_schedule_id(uint16_t v) { _schedule_id = v; }

  private:
    char _trd_code[16] {};
    char _mkt_code[16] {};
    class symbol _symbol;
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

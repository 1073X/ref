#pragma once

#include <cstring>    // std::strncpy

#include "ref/currency_type.hpp"

#include "referential.hpp"

namespace miu::ref {

class underlying_impl : public referential {
  public:
    underlying_impl() = default;

    underlying_impl(std::string_view str)
        : referential(str) {}

    underlying_impl(exchange_type exchange, product_type type, std::string_view name)
        : referential(exchange, type, name) {}

    auto currency() const { return _currency; }
    auto set_currency(currency_type v) {
        if (v >= currency_type::MAX) {
            throw std::invalid_argument("invalid currency");
        }
        _currency = v;
    }

    auto multiplier() const { return _multiplier; }
    auto set_multiplier(int32_t v) {
        if (v <= 0) {
            throw std::invalid_argument("invalid multiplier");
        }
        _multiplier = v;
    }

    auto lot_size() const { return _lot_size; }
    auto set_lot_size(int32_t v) {
        if (v <= 0) {
            throw std::invalid_argument("invalid lot_size");
        }
        _lot_size = v;
    }

    auto tiktable_id() const { return _tiktable_id; }
    auto set_tiktable_id(uint16_t v) { _tiktable_id = v; }

    auto schedule_id() const { return _schedule_id; }
    auto set_schedule_id(uint16_t v) { _schedule_id = v; }

    auto paddings() const { return _paddings; }

  private:
    int32_t _multiplier { 0 };
    int32_t _lot_size { 0 };
    currency_type _currency { currency_type::UNDEF };
    uint16_t _tiktable_id { 0 };
    uint16_t _schedule_id { 0 };
    uint16_t _paddings[5];
};
static_assert(CACHE_LINE == sizeof(underlying_impl));

}    // namespace miu::ref

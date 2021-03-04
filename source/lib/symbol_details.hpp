#pragma once

#include "ref/exchange_type.hpp"
#include "ref/product_type.hpp"

namespace miu::ref::details {

static uint32_t constexpr base_year = 2019;

union layout {
    uint64_t value { 0 };

    struct {
        uint64_t name         : 21;    // 3 chars
        uint64_t strike       : 24;
        uint64_t maturity_yrs : 3;    // can represent the 8 years since base_year
        uint64_t maturity_mon : 4;
        uint64_t product      : 4;
        uint64_t exchange     : 8;
    } opt;

    struct {
        uint64_t name         : 45;    // 6 chars
        uint64_t maturity_yrs : 3;
        uint64_t maturity_mon : 4;
        uint64_t product      : 4;
        uint64_t exchange     : 8;
    } fut;

    struct {
        uint64_t name     : 52;    // 7 chars
        uint64_t product  : 4;
        uint64_t exchange : 8;
    } gen;
};
static_assert(sizeof(uint64_t) == sizeof(layout));

auto create_layout(exchange_type exchange, product_type product) {
    layout lay;
    lay.gen.exchange = static_cast<uint64_t>(exchange) & 0xFF;
    lay.gen.product  = static_cast<uint64_t>(product) & 0x0F;
    return lay;
}

template<typename T>
std::string contract(T t) {
    std::string ret;

    auto mon = t.maturity_mon;
    if (mon > 0) {
        auto yrs = (t.maturity_yrs + details::base_year) % 10;
        ret      = "FGHJKMNQUVXZ"[mon - 1] + com::to_string(yrs);
    }
    return ret;
}

uint64_t encode_name(std::string_view str, uint32_t bytes) {
    if (str.size() > bytes) {
        FATAL_ERROR<std::overflow_error>("name overflow", str, ">", bytes);
    }

    uint64_t val { 0 };
    for (auto i = 0U; i < std::min(bytes, (uint32_t)str.size()); i++) {
        val |= ((uint64_t)str[i] & 0x7F) << (7 * i);
    }
    return val;
}

template<typename T>
void set_maturity(T& t, time::date val) {
    if (val < symbol::min_maturity()) {
        FATAL_ERROR<std::underflow_error>("maturity underflow", val, "<", symbol::min_maturity());
    }

    if (val > symbol::max_maturity()) {
        FATAL_ERROR<std::overflow_error>("maturity overflow", val, ">", symbol::max_maturity());
    }

    auto [yrs, mon, day] = val.ytd();
    t.maturity_yrs       = yrs - details::base_year;
    t.maturity_mon       = mon;
}

}    // namespace miu::ref::details


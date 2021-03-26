#pragma once

#include <time/date.hpp>

#include "ref/exchange_type.hpp"
#include "ref/product_type.hpp"

namespace miu::ref::details {

static uint32_t constexpr base_year = 2019;
static double const max_strike      = std::pow(2, 24);
static char constexpr delimiter { '/' };

union layout {
    uint64_t value { 0 };

    struct {
        uint64_t name         : 21;    // 3 chars
        uint64_t strike       : 24;
        uint64_t maturity_yrs : 3;    // can represent the 8 years since base_year
        uint64_t maturity_mon : 4;
        uint64_t type         : 4;
        uint64_t exchange     : 8;
    } opt;

    struct {
        uint64_t name         : 45;    // 6 chars
        uint64_t maturity_yrs : 3;
        uint64_t maturity_mon : 4;
        uint64_t type         : 4;
        uint64_t exchange     : 8;
    } fut;

    struct {
        uint64_t name     : 52;    // 7 chars
        uint64_t type     : 4;
        uint64_t exchange : 8;
    } gen;
};
static_assert(sizeof(uint64_t) == sizeof(layout));

auto create_layout(exchange_type exchange, product_type type) {
    layout lay;
    lay.gen.exchange = static_cast<uint64_t>(exchange) & 0xFF;
    lay.gen.type     = static_cast<uint64_t>(type) & 0x0F;
    return lay;
}

uint64_t make_strike(double strike) {
    auto val = strike;

    if (val > max_strike) {
        FATAL_ERROR<std::overflow_error>("ref SYM strike price overflow", strike);
    }

    while (val * 10 < max_strike) {
        val *= 10;
    }

    if (std::round(val) < val) {
        FATAL_ERROR<std::overflow_error>("ref SYM strike price overflow", strike);
    }

    return std::round(val);
}

template<typename T>
std::string contract(T t) {
    char buf[8] {};
    uint32_t mon = t.maturity_mon;
    if (mon > 0) {
        auto yrs = (t.maturity_yrs + base_year) % 100;
        std::snprintf(buf, sizeof(buf), "%02u%02u", yrs, mon);
    }
    return buf;
}

template<typename T>
void set_maturity(T& t, time::date val) {
    if (val < symbol::min_maturity()) {
        FATAL_ERROR<std::underflow_error>(
            "ref SYM maturity underflow", val, "<", symbol::min_maturity());
    }

    if (val > symbol::max_maturity()) {
        FATAL_ERROR<std::overflow_error>(
            "ref SYM maturity overflow", val, ">", symbol::max_maturity());
    }

    auto [yrs, mon, day] = val.ytd();
    t.maturity_yrs       = yrs - base_year;
    t.maturity_mon       = mon;
}

uint64_t encode_name(std::string_view str, uint32_t bytes) {
    if (str.size() > bytes) {
        FATAL_ERROR<std::overflow_error>("ref SYM name overflow", str, ">", bytes);
    }

    uint64_t val { 0 };
    for (auto i = 0U; i < std::min(bytes, (uint32_t)str.size()); i++) {
        val |= ((uint64_t)str[i] & 0x7F) << (7 * i);
    }
    return val;
}

symbol from_string(std::string_view str) noexcept try {
    auto make_maturity = [](std::string_view str) -> time::date {
        if (str.size() != 4) {
            FATAL_ERROR<std::invalid_argument>("ref SYM invalid maturity");
        }
        auto c2i = [&](auto ch) {
            if (!std::isdigit(ch)) {
                FATAL_ERROR<std::invalid_argument>("ref SYM invalid maturity");
            }
            return ch - '0';
        };

        auto yrs = 2000 + c2i(str[0]) * 10 + c2i(str[1]);
        auto mon = c2i(str[2]) * 10 + c2i(str[3]);
        return { yrs, mon, 1 };
    };

    std::istringstream ss { str.data() };
    auto next_part = [&ss]() {
        std::string seg;
        std::getline(ss, seg, delimiter);
        return seg;
    };

    auto exchange = com::val_to_enum<exchange_type>(next_part());
    if (exchange_type::MAX == exchange) {
        FATAL_ERROR<std::invalid_argument>("ref SYM invalid exchange");
    }

    auto type = com::val_to_enum<product_type>(next_part());
    if (product_type::MAX == type) {
        FATAL_ERROR<std::invalid_argument>("ref SYM invalid type");
    }

    auto name = next_part();
    if (name.empty()) {
        FATAL_ERROR<std::invalid_argument>("ref SYM missing name");
    }

    switch (type) {
    case product_type::CALL:
    case product_type::PUT: {
        auto strike   = next_part();
        auto contract = next_part();
        if (strike.empty() ^ contract.empty()) {
            FATAL_ERROR<std::invalid_argument>("ref SYM invalid option");
        }

        if (!strike.empty() && !contract.empty()) {
            auto maturity = make_maturity(contract);
            return { exchange, type, name, make_strike(std::stod(strike)), maturity };
        }
    } break;
    case product_type::FUTURE: {
        auto contract = next_part();
        if (!contract.empty()) {
            return { exchange, name, make_maturity(contract) };
        }
    } break;
    default:
        break;
    }

    return { exchange, type, name };

} catch (std::exception const& err) {
    log::error(err.what(), '[', str, ']');
    return {};
}

}    // namespace miu::ref::details

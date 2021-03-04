
#include "ref/symbol.hpp"

#include "symbol_details.hpp"

namespace miu::ref {

time::date symbol::min_maturity() {
    return { details::base_year, 1, 1 };
}

time::date symbol::max_maturity() {
    return { details::base_year + 8, 12, 31 };
}

symbol::symbol(uint64_t v)
    : _value(v) {
}

symbol::symbol(exchange_type exchange, product_type product, std::string_view name) {
    auto layout = details::create_layout(exchange, product);

    switch (product) {
    case product_type::PUT:
    case product_type::CALL:
        layout.opt.name = details::encode_name(name, 3);
        break;
    case product_type::FUTURE:
        layout.fut.name = details::encode_name(name, 6);
        break;
    default:
        layout.gen.name = details::encode_name(name, 7);
        break;
    }

    _value = layout.value;
}

symbol::symbol(exchange_type exchange, std::string_view name, time::date maturity) {
    auto layout     = details::create_layout(exchange, product_type::FUTURE);
    layout.fut.name = details::encode_name(name, 6);
    details::set_maturity(layout.fut, maturity);

    _value = layout.value;
}

symbol::symbol(exchange_type exchange,
               product_type product,
               std::string_view name,
               price strike,
               time::date maturity) {
    if (product_type::CALL != product && product_type::PUT != product) {
        FATAL_ERROR<std::invalid_argument>(
            "not options", exchange, product, name, strike, maturity);
    }

    static auto max_strike = std::pow(2, 24);
    double strike_val      = strike;
    if (strike_val > max_strike) {
        FATAL_ERROR<std::overflow_error>(
            "strike overflow", exchange, product, name, strike, maturity);
    }

    auto layout     = details::create_layout(exchange, product);
    layout.opt.name = details::encode_name(name, 3);
    details::set_maturity(layout.opt, maturity);

    while (strike_val * 10 < max_strike) {
        strike_val *= 10;
    }
    layout.opt.strike = std::round(strike_val);

    _value = layout.value;
}

product_type symbol::product() const {
    auto layout = details::layout { _value };
    return com::val_to_enum<product_type>(layout.opt.product);
}

exchange_type symbol::exchange() const {
    auto layout = details::layout { _value };
    return com::val_to_enum<exchange_type>(layout.opt.exchange);
}

std::string symbol::name() const {
    char buf[8] {};
    auto decode_name = [&](uint64_t val, uint32_t bytes) {
        for (auto i = 0U; i < bytes; i++) {
            buf[i] = (val >> (i * 7)) & 0x7F;
        }
    };

    auto layout = details::layout { _value };
    switch (product()) {
    case product_type::PUT:
    case product_type::CALL:
        decode_name(layout.opt.name, 3);
        break;
    case product_type::FUTURE:
        decode_name(layout.fut.name, 6);
        break;
    default:
        decode_name(layout.gen.name, 7);
        break;
    }

    return { buf };
}

std::string symbol::str() const {
    auto layout = details::layout { _value };

    auto ret = com::strcat { exchange(), product(), name() }.str();
    switch (product()) {
    case product_type::PUT:
    case product_type::CALL: {
        auto contract = details::contract(layout.opt);
        if (!contract.empty()) {
            ret = com::strcat { ret, (uint64_t)layout.opt.strike, contract }.str();
        }
    } break;

    case product_type::FUTURE: {
        auto contract = details::contract(layout.fut);
        if (!contract.empty()) {
            ret = com::strcat { ret, contract }.str();
        }
    } break;

    default:
        break;
    }

    return ret;
}

}    // namespace miu::ref

DEF_VAR_SET(miu::ref::symbol) {
    new (_value) ref::symbol { v };
}

DEF_VAR_GET(miu::ref::symbol) {
    switch (id()) {
    case type_id<ref::symbol>::value:
        return *(ref::symbol const*)_value;
    case type_id<uint64_t>::value:
        return *(uint64_t const*)_value;
    default:
        return std::nullopt;
    }
}

DEF_TO_STRING(miu::ref::symbol) {
    return v.str();
}

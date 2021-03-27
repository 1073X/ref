
#include "ref/symbol.hpp"

#include <log/log.hpp>
#include <sstream>

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

symbol::symbol(std::string_view str)
    : symbol(details::from_string(str)) {
}

symbol::symbol(exchange_type exchange, product_type type, std::string_view name) {
    auto layout = details::create_layout(exchange, type);

    switch (type) {
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
               product_type type,
               std::string_view name,
               price strike,
               time::date maturity) {
    if (product_type::CALL != type && product_type::PUT != type) {
        FATAL_ERROR<std::invalid_argument>(
            "ref SYM not options", exchange, type, name, strike, maturity);
    }

    auto layout = details::create_layout(exchange, type);

    layout.opt.strike = details::make_strike(strike);

    layout.opt.name = details::encode_name(name, 3);
    details::set_maturity(layout.opt, maturity);

    _value = layout.value;
}

product_type symbol::type() const {
    auto layout = details::layout { _value };
    return com::val_to_enum<product_type>(layout.opt.type);
}

exchange_type symbol::exchange() const {
    auto layout = details::layout { _value };
    return com::val_to_enum<exchange_type>(layout.opt.exchange);
}

std::string symbol::name() const {
    char buf[8] {};
    auto decode_name = [&](uint64_t val, uint32_t bytes) {
        for (auto i = 0U; i < bytes; i++) {
            buf[i] = char((val >> (i * 7)) & 0x7F);
        }
    };

    auto layout = details::layout { _value };
    switch (type()) {
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
    static const com::strcat::delimiter DELIMITER { { details::delimiter } };

    auto layout = details::layout { _value };

    auto ret = com::strcat { exchange(), type(), name(), DELIMITER }.str();
    switch (type()) {
    case product_type::PUT:
    case product_type::CALL: {
        auto contract = details::contract(layout.opt);
        if (!contract.empty()) {
            ret = com::strcat { ret, (uint64_t)layout.opt.strike, contract, DELIMITER }.str();
        }
    } break;

    case product_type::FUTURE: {
        auto contract = details::contract(layout.fut);
        if (!contract.empty()) {
            ret = com::strcat { ret, contract, DELIMITER }.str();
        }
    } break;

    default:
        break;
    }

    return ret;
}

}    // namespace miu::ref

#pragma once

#include <cmath>    // std::pow
#include <com/enum.hpp>
#include <com/fatal_error.hpp>
#include <com/variant.hpp>
#include <time/date.hpp>

#include "exchange_type.hpp"
#include "price.hpp"
#include "product_type.hpp"

namespace miu::ref {

class symbol {
  public:
    static time::date min_maturity();
    static time::date max_maturity();

  public:
    symbol(uint64_t = 0);

    // equity
    symbol(exchange_type, product_type, std::string_view name);
    // future
    symbol(exchange_type, std::string_view name, time::date);
    // options
    symbol(exchange_type, product_type, std::string_view name, price strike, time::date);

    auto value() const { return _value; }

    auto operator!() const { return !_value; }
    operator bool() const { return !operator!(); }

    product_type product() const;
    exchange_type exchange() const;
    std::string name() const;
    std::string str() const;

  private:
    uint64_t _value;
};

}    // namespace miu::ref

DEF_VARIANT(miu::ref::symbol, 37);

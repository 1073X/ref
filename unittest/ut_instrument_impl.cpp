#include <gtest/gtest.h>

#include "source/lib/instrument_impl.hpp"

using namespace miu::ref;
using miu::time::date;

TEST(ut_instrument_impl, general) {
    underlying_impl prod { exchange_type::SSE, product_type::STOCK, "stock" };
    prod.set_currency(currency_type::USD);
    instrument_impl inst { prod, "name" };

    EXPECT_EQ(currency_type::USD, inst.currency());

    symbol sym(exchange_type::SSE, product_type::STOCK, "name");
    EXPECT_EQ(sym, inst.symbol());

    EXPECT_EQ("name", inst.name());
    EXPECT_EQ(exchange_type::SSE, inst.exchange());
    EXPECT_EQ(product_type::STOCK, inst.type());
    EXPECT_EQ(symbol::min_maturity(), inst.maturity());
    EXPECT_EQ(0, inst.strike_price());

    EXPECT_STREQ("", inst.trd_code().value);
    inst.set_trd_code("trd_code");
    EXPECT_STREQ("trd_code", inst.trd_code().value);

    EXPECT_STREQ("", inst.mkt_code().value);
    inst.set_mkt_code("mkt_code");
    EXPECT_STREQ("mkt_code", inst.mkt_code().value);
}

TEST(ut_instrument_impl, future) {
    underlying_impl prod { exchange_type::CFFEX, product_type::FUTURE, "abc" };
    prod.set_lot_size(100);
    prod.set_multiplier(100);

    auto maturity = date(2021, 3, 4);
    instrument_impl inst { prod, maturity };
    EXPECT_EQ(100, inst.lot_size());
    EXPECT_EQ(100, inst.multiplier());

    symbol sym(exchange_type::CFFEX, "abc", maturity);
    EXPECT_EQ(sym, inst.symbol());

    EXPECT_EQ("abc", inst.name());
    EXPECT_EQ(exchange_type::CFFEX, inst.exchange());
    EXPECT_EQ(product_type::FUTURE, inst.type());
    EXPECT_EQ(maturity, inst.maturity());
    EXPECT_EQ(0, inst.strike_price());
}

TEST(ut_instrument_impl, options) {
    underlying_impl prod { exchange_type::CFFEX, product_type::CALL, "abc" };
    prod.set_tiktable_id(99);
    prod.set_schedule_id(100);

    auto maturity = date(2021, 3, 4);
    instrument_impl inst { prod, 1.2, maturity };

    EXPECT_EQ(99U, inst.tiktable_id());
    EXPECT_EQ(100U, inst.schedule_id());

    symbol sym(exchange_type::CFFEX, product_type::CALL, "abc", 1.2, maturity);
    EXPECT_EQ(sym, inst.symbol());

    EXPECT_EQ("abc", inst.name());
    EXPECT_EQ(exchange_type::CFFEX, inst.exchange());
    EXPECT_EQ(product_type::CALL, inst.type());
    EXPECT_EQ(maturity, inst.maturity());
    EXPECT_EQ(1.2, inst.strike_price());
}

#include <gtest/gtest.h>

#include "ref/instrument.hpp"

using namespace miu::ref;
using miu::time::date;

TEST(ut_instrument, general) {
    underlying prod { 1, exchange_type::SSE, product_type::STOCK, "stock" };
    instrument inst { 99, prod, "name" };

    EXPECT_EQ(1U, inst.uid());
    EXPECT_EQ(99U, inst.id());

    symbol sym(exchange_type::SSE, product_type::STOCK, "name");
    EXPECT_EQ(sym, inst.symbol());

    EXPECT_EQ("name", inst.name());
    EXPECT_EQ(exchange_type::SSE, inst.exchange());
    EXPECT_EQ(product_type::STOCK, inst.product());
    EXPECT_EQ(symbol::min_maturity(), inst.maturity());
    EXPECT_EQ(0, inst.strike_price());

    EXPECT_EQ("", inst.trd_code());
    inst.set_trd_code("trd_code");
    EXPECT_EQ("trd_code", inst.trd_code());

    EXPECT_EQ("", inst.mkt_code());
    inst.set_mkt_code("mkt_code");
    EXPECT_EQ("mkt_code", inst.mkt_code());
}

TEST(ut_instrument, future) {
    underlying prod { 3, exchange_type::CFFEX, product_type::FUTURE, "abc" };
    auto maturity = date(2021, 3, 4);
    instrument inst { 2, prod, maturity };

    EXPECT_EQ(3U, inst.uid());
    EXPECT_EQ(2U, inst.id());

    symbol sym(exchange_type::CFFEX, "abc", maturity);
    EXPECT_EQ(sym, inst.symbol());

    EXPECT_EQ("abc", inst.name());
    EXPECT_EQ(exchange_type::CFFEX, inst.exchange());
    EXPECT_EQ(product_type::FUTURE, inst.product());
    EXPECT_EQ(maturity, inst.maturity());
    EXPECT_EQ(0, inst.strike_price());
}

TEST(ut_instrument, options) {
    underlying prod { 3, exchange_type::CFFEX, product_type::CALL, "abc" };
    auto maturity = date(2021, 3, 4);
    instrument inst { 2, prod, 1.2, maturity };

    EXPECT_EQ(3U, inst.uid());
    EXPECT_EQ(2U, inst.id());

    symbol sym(exchange_type::CFFEX, product_type::CALL, "abc", 1.2, maturity);
    EXPECT_EQ(sym, inst.symbol());

    EXPECT_EQ("abc", inst.name());
    EXPECT_EQ(exchange_type::CFFEX, inst.exchange());
    EXPECT_EQ(product_type::CALL, inst.product());
    EXPECT_EQ(maturity, inst.maturity());
    EXPECT_EQ(1.2, inst.strike_price());
}

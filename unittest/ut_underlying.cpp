#include <gtest/gtest.h>

#include "ref/underlying.hpp"

using namespace miu::ref;

struct ut_underlying : public testing::Test {
    underlying under { 99, exchange_type::SZE, product_type::STOCK, "stock" };
};

TEST_F(ut_underlying, symbol) {
    EXPECT_EQ(99U, under.id());
    EXPECT_EQ(symbol(exchange_type::SZE, product_type::STOCK, "stock"), under.symbol());
    EXPECT_EQ(exchange_type::SZE, under.exchange());
    EXPECT_EQ(product_type::STOCK, under.product());
    EXPECT_EQ("stock", under.name());
}

TEST_F(ut_underlying, code) {
    EXPECT_EQ("", under.trd_code());
    EXPECT_EQ("", under.mkt_code());

    under.set_trd_code("trd_code");
    under.set_mkt_code("mkt_code");

    EXPECT_EQ("trd_code", under.trd_code());
    EXPECT_EQ("mkt_code", under.mkt_code());
}

TEST_F(ut_underlying, currency) {
    EXPECT_EQ(currency_type::UNDEF, under.currency());

    under.set_currency(currency_type::USD);
    EXPECT_EQ(currency_type::USD, under.currency());
}

TEST_F(ut_underlying, multiplier) {
    EXPECT_EQ(0, under.multiplier());

    under.set_multiplier(100);
    EXPECT_EQ(100, under.multiplier());
}

TEST_F(ut_underlying, lot_size) {
    EXPECT_EQ(0, under.lot_size());

    under.set_lot_size(100);
    EXPECT_EQ(100, under.lot_size());
}

TEST_F(ut_underlying, dependencies) {
    EXPECT_EQ(0U, under.tick_tbl_id());
    EXPECT_EQ(0U, under.schedule_id());

    under.set_tick_tbl_id(1);
    under.set_schedule_id(2);

    EXPECT_EQ(1U, under.tick_tbl_id());
    EXPECT_EQ(2U, under.schedule_id());
}

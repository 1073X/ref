#include <gtest/gtest.h>

#include "source/lib/ref/underlying_impl.hpp"

using namespace miu::ref;

struct ut_underlying_impl : public testing::Test {
    underlying_impl under { exchange_type::SZE, product_type::STOCK, "stock" };
};

TEST_F(ut_underlying_impl, symbol) {
    EXPECT_EQ(symbol(exchange_type::SZE, product_type::STOCK, "stock"), under.symbol());
    EXPECT_EQ(exchange_type::SZE, under.exchange());
    EXPECT_EQ(product_type::STOCK, under.type());
    EXPECT_EQ("stock", under.name());
}

TEST_F(ut_underlying_impl, code) {
    EXPECT_STREQ("", under.trd_code().value);
    EXPECT_STREQ("", under.mkt_code().value);

    under.set_trd_code("trd_code");
    under.set_mkt_code("mkt_code");

    EXPECT_STREQ("trd_code", under.trd_code().value);
    EXPECT_STREQ("mkt_code", under.mkt_code().value);
}

TEST_F(ut_underlying_impl, currency) {
    EXPECT_EQ(currency_type::UNDEF, under.currency());

    under.set_currency(currency_type::USD);
    EXPECT_EQ(currency_type::USD, under.currency());

    EXPECT_ANY_THROW(under.set_currency(currency_type::MAX));
}

TEST_F(ut_underlying_impl, multiplier) {
    EXPECT_EQ(0, under.multiplier());

    under.set_multiplier(100);
    EXPECT_EQ(100, under.multiplier());

    EXPECT_ANY_THROW(under.set_multiplier(0));
    EXPECT_ANY_THROW(under.set_multiplier(-1));
}

TEST_F(ut_underlying_impl, lot_size) {
    EXPECT_EQ(0, under.lot_size());

    under.set_lot_size(100);
    EXPECT_EQ(100, under.lot_size());

    EXPECT_ANY_THROW(under.set_lot_size(0));
    EXPECT_ANY_THROW(under.set_lot_size(-1));
}

TEST_F(ut_underlying_impl, dependencies) {
    EXPECT_EQ(0U, under.tiktable_id());
    EXPECT_EQ(0U, under.schedule_id());

    under.set_tiktable_id(1);
    under.set_schedule_id(2);

    EXPECT_EQ(1U, under.tiktable_id());
    EXPECT_EQ(2U, under.schedule_id());
}

#include <gtest/gtest.h>

#include "source/lib/tick_table.hpp"

using miu::ref::price;

struct ut_tick_table : public testing::Test {
    void SetUp() override {
        tick_table.add(0.0, 0.1);
        tick_table.add(5.0, 0.5);
        tick_table.add(50.0, 1);
    }

    miu::ref::tick_table tick_table;
};

TEST_F(ut_tick_table, compute_precision) {
    miu::ref::tick_table tick_table;
    EXPECT_EQ(0, tick_table.precision());
    tick_table.add(0.0, 10);
    EXPECT_EQ(0, tick_table.precision());
    tick_table.add(5, 0.1);
    EXPECT_EQ(1, tick_table.precision());
    tick_table.add(1000, 0.01);
    EXPECT_EQ(2, tick_table.precision());
}

TEST_F(ut_tick_table, up) {
    EXPECT_EQ(4.6, tick_table.up(4.5, 1));
    EXPECT_EQ(5.0, tick_table.up(4.9, 1));
    EXPECT_EQ(6.0, tick_table.up(5.5, 1));
    EXPECT_EQ(50.0, tick_table.up(49.5, 1));
    EXPECT_EQ(51.0, tick_table.up(50, 1));
    EXPECT_EQ(2.6, tick_table.up(2.3, 3));
    EXPECT_EQ(6.5, tick_table.up(5, 3));
    EXPECT_EQ(53, tick_table.up(50, 3));

    // cross the edge
    EXPECT_EQ(5.5, tick_table.up(4.8, 3));
    EXPECT_EQ(52, tick_table.up(49.5, 3));

    // round to the nearest tick
    EXPECT_EQ(2.3, tick_table.up(2.21, 0));
    EXPECT_EQ(2.2, tick_table.up(2.2, 0));
    EXPECT_EQ(30.5, tick_table.up(30.26, 0));
    EXPECT_EQ(30.5, tick_table.up(30.5, 0));
    EXPECT_EQ(31, tick_table.up(30.51, 0));
    EXPECT_EQ(31, tick_table.up(31, 0));
    EXPECT_EQ(61, tick_table.up(60.1, 0));
    EXPECT_EQ(61, tick_table.up(61, 0));

    // across multiple levels
    EXPECT_EQ(51, tick_table.up(4.5, 5 + 90 + 1));

    // negative
    EXPECT_EQ(-tick_table.dn(1, 1), tick_table.up(-1, 1));
    EXPECT_EQ(tick_table.up(0, 10), tick_table.up(tick_table.dn(0, 5), 15));

    EXPECT_EQ(0, tick_table.up(0, 0));
}

TEST_F(ut_tick_table, dn) {
    EXPECT_EQ(4.2, tick_table.dn(4.3, 1));
    EXPECT_EQ(5.0, tick_table.dn(5.5, 1));
    EXPECT_EQ(50, tick_table.dn(51, 1));
    EXPECT_EQ(4.9, tick_table.dn(5, 1));
    EXPECT_EQ(2, tick_table.dn(2.3, 3));
    EXPECT_EQ(5.5, tick_table.dn(7, 3));
    EXPECT_EQ(52, tick_table.dn(55, 3));

    // cross the edge
    EXPECT_EQ(49.5, tick_table.dn(60, 11));
    EXPECT_EQ(4.9, tick_table.dn(5.5, 2));

    // round to the nearest tick
    EXPECT_EQ(2.3, tick_table.dn(2.39, 0));
    EXPECT_EQ(2.3, tick_table.dn(2.3, 0));
    EXPECT_EQ(30.5, tick_table.dn(30.59, 0));
    EXPECT_EQ(30.5, tick_table.dn(30.5, 0));
    EXPECT_EQ(51, tick_table.dn(51.9, 0));
    EXPECT_EQ(51, tick_table.dn(51, 0));

    // round to edge
    EXPECT_EQ(5.0, tick_table.dn(5.1, 0));
    EXPECT_EQ(50, tick_table.dn(50.1, 0));

    // round to edge and cross
    EXPECT_EQ(4.9, tick_table.dn(5.6, 2));
    EXPECT_EQ(49.5, tick_table.dn(51.1, 2));

    // across multiple edges
    EXPECT_EQ(4.5, tick_table.dn(51, 5 + 90 + 1));

    // negative
    EXPECT_EQ(-tick_table.up(0, 1), tick_table.dn(0, 1));
    EXPECT_EQ(-tick_table.up(1, 1), tick_table.dn(-1, 1));
    EXPECT_EQ(-tick_table.up(0, 5), tick_table.dn(tick_table.up(0, 5), 10));

    EXPECT_EQ(0, tick_table.dn(price(0), 0));
}

TEST_F(ut_tick_table, precision) {
    miu::ref::tick_table tick_table;
    tick_table.add(0.0, 0.01);
    tick_table.add(2.0, 0.00005);

    EXPECT_EQ(0.07, tick_table.dn(0.07, 0));
    EXPECT_EQ(0.06, tick_table.dn(0.07, 1));
    EXPECT_EQ(0.05, tick_table.dn(0.07, 2));
    EXPECT_EQ(0.07, tick_table.up(0.07, 0));
    EXPECT_EQ(0.08, tick_table.up(0.07, 1));
    EXPECT_EQ(0.09, tick_table.up(0.07, 2));

    EXPECT_EQ(2.00005, tick_table.up(2, 1));
    EXPECT_EQ(2.0002, tick_table.up(2.00005, 3));
    EXPECT_EQ(2.00005, tick_table.dn(2.00005, 0));
    EXPECT_EQ(2, tick_table.dn(2.00005, 1));
    EXPECT_EQ(2.0001, tick_table.dn(2.00035, 5));

    price prc = 0.0;
    for (auto i = 0; i < std::round(2.0 / 0.01); i++) {
        EXPECT_EQ(prc + 0.01 * i, tick_table.up(prc, i));
    }

    prc = 2.0;
    for (auto i = 0; i < std::round(3 / 0.00005); i++) {
        EXPECT_EQ(prc + 0.00005 * i, tick_table.up(prc, i));
    }
}

TEST_F(ut_tick_table, large) {
    miu::ref::tick_table tick_table;
    tick_table.add(0.5, 0.01);
    tick_table.add(5, 0.02);
    tick_table.add(20, 0.05);
    tick_table.add(100, 0.1);
    tick_table.add(200, 0.2);

    EXPECT_EQ(0.51, tick_table.up(0.5, 1));

    EXPECT_EQ(4.99, tick_table.dn(5, 1));
    EXPECT_EQ(5.02, tick_table.up(5, 1));

    EXPECT_EQ(19.98, tick_table.dn(20, 1));
    EXPECT_EQ(20.05, tick_table.up(20, 1));

    EXPECT_EQ(99.95, tick_table.dn(100, 1));
    EXPECT_EQ(100.1, tick_table.up(100, 1));

    EXPECT_EQ(199.9, tick_table.dn(200, 1));
    EXPECT_EQ(200.2, tick_table.up(200, 1));

    EXPECT_EQ(209.8, tick_table.dn(210.008, 1));
}

TEST_F(ut_tick_table, to_tick_count) {
    EXPECT_EQ(-tick_table.to_tick_count(1), tick_table.to_tick_count(-1));

    EXPECT_EQ(0, tick_table.to_tick_count(0.0));
    EXPECT_EQ(25, tick_table.to_tick_count(2.5));
    EXPECT_EQ(50, tick_table.to_tick_count(5.0));

    EXPECT_EQ(51, tick_table.to_tick_count(5.5));
    EXPECT_EQ(90, tick_table.to_tick_count(25));
    EXPECT_EQ(140, tick_table.to_tick_count(50));

    EXPECT_EQ(141, tick_table.to_tick_count(51));
    EXPECT_EQ(200, tick_table.to_tick_count(110));
}

TEST_F(ut_tick_table, distance) {
    EXPECT_EQ(25, tick_table.distance(0, 2.5));
    EXPECT_EQ(25, tick_table.distance(2.5, 5));
    EXPECT_EQ(65, tick_table.distance(2.5, 25));
    EXPECT_EQ(175, tick_table.distance(2.5, 110));

    EXPECT_EQ(-10, tick_table.distance(0, -1));
    EXPECT_EQ(10, tick_table.distance(-1, 0));

    EXPECT_EQ(20, tick_table.distance(-1, 1));
    EXPECT_EQ(-20, tick_table.distance(1, -1));
}

TEST_F(ut_tick_table, extreme) {
    miu::ref::tick_table tick_table;
    tick_table.add(0.5, 0.01);
    tick_table.add(5, 0.02);
    tick_table.add(20, 0.05);
    tick_table.add(100, 0.1);
    tick_table.add(200, 0.2);

    auto lower = price::min();
    auto upper = price::max();

    EXPECT_EQ(upper, tick_table.up(upper, 0));
    EXPECT_EQ(upper, tick_table.up(upper, 1));
    EXPECT_EQ(0, tick_table.up(0, 0));
    EXPECT_EQ(0, tick_table.dn(0, 0));
    EXPECT_EQ(lower, tick_table.dn(lower, 1));
    EXPECT_EQ(lower, tick_table.dn(lower, 0));
}

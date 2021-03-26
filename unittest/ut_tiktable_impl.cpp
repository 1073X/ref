#include <gtest/gtest.h>

#include "source/lib/tiktable_impl.hpp"

using miu::ref::price;

struct ut_tiktable_impl : public testing::Test {
    void SetUp() override {
        tiktable_impl.add(0.0, 0.1);
        tiktable_impl.add(5.0, 0.5);
        tiktable_impl.add(50.0, 1);
    }

    miu::ref::tiktable_impl tiktable_impl;
};

TEST_F(ut_tiktable_impl, compute_precision) {
    miu::ref::tiktable_impl tiktable_impl;
    EXPECT_EQ(0, tiktable_impl.precision());
    tiktable_impl.add(0.0, 10);
    EXPECT_EQ(0, tiktable_impl.precision());
    tiktable_impl.add(5, 0.1);
    EXPECT_EQ(1, tiktable_impl.precision());
    tiktable_impl.add(1000, 0.01);
    EXPECT_EQ(2, tiktable_impl.precision());
}

TEST_F(ut_tiktable_impl, up) {
    EXPECT_EQ(4.6, tiktable_impl.up(4.5, 1));
    EXPECT_EQ(5.0, tiktable_impl.up(4.9, 1));
    EXPECT_EQ(6.0, tiktable_impl.up(5.5, 1));
    EXPECT_EQ(50.0, tiktable_impl.up(49.5, 1));
    EXPECT_EQ(51.0, tiktable_impl.up(50, 1));
    EXPECT_EQ(2.6, tiktable_impl.up(2.3, 3));
    EXPECT_EQ(6.5, tiktable_impl.up(5, 3));
    EXPECT_EQ(53, tiktable_impl.up(50, 3));

    // cross the edge
    EXPECT_EQ(5.5, tiktable_impl.up(4.8, 3));
    EXPECT_EQ(52, tiktable_impl.up(49.5, 3));

    // round to the nearest tick
    EXPECT_EQ(2.3, tiktable_impl.up(2.21, 0));
    EXPECT_EQ(2.2, tiktable_impl.up(2.2, 0));
    EXPECT_EQ(30.5, tiktable_impl.up(30.26, 0));
    EXPECT_EQ(30.5, tiktable_impl.up(30.5, 0));
    EXPECT_EQ(31, tiktable_impl.up(30.51, 0));
    EXPECT_EQ(31, tiktable_impl.up(31, 0));
    EXPECT_EQ(61, tiktable_impl.up(60.1, 0));
    EXPECT_EQ(61, tiktable_impl.up(61, 0));

    // across multiple levels
    EXPECT_EQ(51, tiktable_impl.up(4.5, 5 + 90 + 1));

    // negative
    EXPECT_EQ(-tiktable_impl.dn(1, 1), tiktable_impl.up(-1, 1));
    EXPECT_EQ(tiktable_impl.up(0, 10), tiktable_impl.up(tiktable_impl.dn(0, 5), 15));

    EXPECT_EQ(0, tiktable_impl.up(0, 0));
}

TEST_F(ut_tiktable_impl, dn) {
    EXPECT_EQ(4.2, tiktable_impl.dn(4.3, 1));
    EXPECT_EQ(5.0, tiktable_impl.dn(5.5, 1));
    EXPECT_EQ(50, tiktable_impl.dn(51, 1));
    EXPECT_EQ(4.9, tiktable_impl.dn(5, 1));
    EXPECT_EQ(2, tiktable_impl.dn(2.3, 3));
    EXPECT_EQ(5.5, tiktable_impl.dn(7, 3));
    EXPECT_EQ(52, tiktable_impl.dn(55, 3));

    // cross the edge
    EXPECT_EQ(49.5, tiktable_impl.dn(60, 11));
    EXPECT_EQ(4.9, tiktable_impl.dn(5.5, 2));

    // round to the nearest tick
    EXPECT_EQ(2.3, tiktable_impl.dn(2.39, 0));
    EXPECT_EQ(2.3, tiktable_impl.dn(2.3, 0));
    EXPECT_EQ(30.5, tiktable_impl.dn(30.59, 0));
    EXPECT_EQ(30.5, tiktable_impl.dn(30.5, 0));
    EXPECT_EQ(51, tiktable_impl.dn(51.9, 0));
    EXPECT_EQ(51, tiktable_impl.dn(51, 0));

    // round to edge
    EXPECT_EQ(5.0, tiktable_impl.dn(5.1, 0));
    EXPECT_EQ(50, tiktable_impl.dn(50.1, 0));

    // round to edge and cross
    EXPECT_EQ(4.9, tiktable_impl.dn(5.6, 2));
    EXPECT_EQ(49.5, tiktable_impl.dn(51.1, 2));

    // across multiple edges
    EXPECT_EQ(4.5, tiktable_impl.dn(51, 5 + 90 + 1));

    // negative
    EXPECT_EQ(-tiktable_impl.up(0, 1), tiktable_impl.dn(0, 1));
    EXPECT_EQ(-tiktable_impl.up(1, 1), tiktable_impl.dn(-1, 1));
    EXPECT_EQ(-tiktable_impl.up(0, 5), tiktable_impl.dn(tiktable_impl.up(0, 5), 10));

    EXPECT_EQ(0, tiktable_impl.dn(price(0), 0));
}

TEST_F(ut_tiktable_impl, precision) {
    miu::ref::tiktable_impl tiktable_impl;
    tiktable_impl.add(0.0, 0.01);
    tiktable_impl.add(2.0, 0.00005);

    EXPECT_EQ(0.07, tiktable_impl.dn(0.07, 0));
    EXPECT_EQ(0.06, tiktable_impl.dn(0.07, 1));
    EXPECT_EQ(0.05, tiktable_impl.dn(0.07, 2));
    EXPECT_EQ(0.07, tiktable_impl.up(0.07, 0));
    EXPECT_EQ(0.08, tiktable_impl.up(0.07, 1));
    EXPECT_EQ(0.09, tiktable_impl.up(0.07, 2));

    EXPECT_EQ(2.00005, tiktable_impl.up(2, 1));
    EXPECT_EQ(2.0002, tiktable_impl.up(2.00005, 3));
    EXPECT_EQ(2.00005, tiktable_impl.dn(2.00005, 0));
    EXPECT_EQ(2, tiktable_impl.dn(2.00005, 1));
    EXPECT_EQ(2.0001, tiktable_impl.dn(2.00035, 5));

    price prc = 0.0;
    for (auto i = 0; i < std::round(2.0 / 0.01); i++) {
        EXPECT_EQ(prc + 0.01 * i, tiktable_impl.up(prc, i));
    }

    prc = 2.0;
    for (auto i = 0; i < std::round(3 / 0.00005); i++) {
        EXPECT_EQ(prc + 0.00005 * i, tiktable_impl.up(prc, i));
    }
}

TEST_F(ut_tiktable_impl, large) {
    miu::ref::tiktable_impl tiktable_impl;
    tiktable_impl.add(0.5, 0.01);
    tiktable_impl.add(5, 0.02);
    tiktable_impl.add(20, 0.05);
    tiktable_impl.add(100, 0.1);
    tiktable_impl.add(200, 0.2);

    EXPECT_EQ(0.51, tiktable_impl.up(0.5, 1));

    EXPECT_EQ(4.99, tiktable_impl.dn(5, 1));
    EXPECT_EQ(5.02, tiktable_impl.up(5, 1));

    EXPECT_EQ(19.98, tiktable_impl.dn(20, 1));
    EXPECT_EQ(20.05, tiktable_impl.up(20, 1));

    EXPECT_EQ(99.95, tiktable_impl.dn(100, 1));
    EXPECT_EQ(100.1, tiktable_impl.up(100, 1));

    EXPECT_EQ(199.9, tiktable_impl.dn(200, 1));
    EXPECT_EQ(200.2, tiktable_impl.up(200, 1));

    EXPECT_EQ(209.8, tiktable_impl.dn(210.008, 1));
}

TEST_F(ut_tiktable_impl, to_tick_count) {
    EXPECT_EQ(-tiktable_impl.to_tick_count(1), tiktable_impl.to_tick_count(-1));

    EXPECT_EQ(0, tiktable_impl.to_tick_count(0.0));
    EXPECT_EQ(25, tiktable_impl.to_tick_count(2.5));
    EXPECT_EQ(50, tiktable_impl.to_tick_count(5.0));

    EXPECT_EQ(51, tiktable_impl.to_tick_count(5.5));
    EXPECT_EQ(90, tiktable_impl.to_tick_count(25));
    EXPECT_EQ(140, tiktable_impl.to_tick_count(50));

    EXPECT_EQ(141, tiktable_impl.to_tick_count(51));
    EXPECT_EQ(200, tiktable_impl.to_tick_count(110));
}

TEST_F(ut_tiktable_impl, distance) {
    EXPECT_EQ(25, tiktable_impl.distance(0, 2.5));
    EXPECT_EQ(25, tiktable_impl.distance(2.5, 5));
    EXPECT_EQ(65, tiktable_impl.distance(2.5, 25));
    EXPECT_EQ(175, tiktable_impl.distance(2.5, 110));

    EXPECT_EQ(-10, tiktable_impl.distance(0, -1));
    EXPECT_EQ(10, tiktable_impl.distance(-1, 0));

    EXPECT_EQ(20, tiktable_impl.distance(-1, 1));
    EXPECT_EQ(-20, tiktable_impl.distance(1, -1));
}

TEST_F(ut_tiktable_impl, extreme) {
    miu::ref::tiktable_impl tiktable_impl;
    tiktable_impl.add(0.5, 0.01);
    tiktable_impl.add(5, 0.02);
    tiktable_impl.add(20, 0.05);
    tiktable_impl.add(100, 0.1);
    tiktable_impl.add(200, 0.2);

    auto lower = price::min();
    auto upper = price::max();

    EXPECT_EQ(upper, tiktable_impl.up(upper, 0));
    EXPECT_EQ(upper, tiktable_impl.up(upper, 1));
    EXPECT_EQ(0, tiktable_impl.up(0, 0));
    EXPECT_EQ(0, tiktable_impl.dn(0, 0));
    EXPECT_EQ(lower, tiktable_impl.dn(lower, 1));
    EXPECT_EQ(lower, tiktable_impl.dn(lower, 0));
}

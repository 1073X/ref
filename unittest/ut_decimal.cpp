#include <gtest/gtest.h>

#include "source/lib/decimal.hpp"

using miu::ref::decimal;

TEST(ut_decimal, comparison) {
    decimal dec1 { 1, 2 };
    decimal dec2 { 1, 2 };
    EXPECT_EQ(dec1, dec2);

    decimal dec3 { 2, 1 };
    EXPECT_NE(dec3, dec2);
}

TEST(ut_decimal, to_string) {
    decimal dec { 1, 2 };
    EXPECT_EQ("1/2", miu::com::to_string(dec));
}


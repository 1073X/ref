#include <gtest/gtest.h>

#include <com/reg_var_str.hpp>

#include "ref/currency_type.hpp"

using miu::com::variant;
using miu::ref::currency_type;

TEST(ut_currency_type, variant) {
    miu::com::reg_var_str<currency_type>();

    auto var = miu::com::variant { currency_type::USD };
    EXPECT_EQ(currency_type::USD, var.get<currency_type>());
    EXPECT_EQ("USD", var.get<std::string>());

    EXPECT_EQ(currency_type::USD, variant { +"USD" }.get<currency_type>());
}


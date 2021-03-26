#include <gtest/gtest.h>

#include <com/reg_var_str.hpp>

#include "ref/exchange_type.hpp"

using miu::ref::exchange_type;

TEST(ut_exchange_type, variant) {
    miu::com::reg_var_str<exchange_type>();

    auto var = miu::com::variant { exchange_type::HKEX };
    EXPECT_EQ(exchange_type::HKEX, var.get<exchange_type>());
    EXPECT_EQ("HKEX", var.get<std::string>());
}

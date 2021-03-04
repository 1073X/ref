#include <gtest/gtest.h>

#include <com/reg_var_str.hpp>

#include "ref/product_type.hpp"

using miu::ref::product_type;

TEST(ut_product_type, variant) {
    miu::com::reg_var_str<product_type>();

    auto var = miu::com::variant { product_type::FUTURE };
    EXPECT_EQ(product_type::FUTURE, var.get<product_type>());
    EXPECT_EQ("FUTURE", var.get<std::string>());
}

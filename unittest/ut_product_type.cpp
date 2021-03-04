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

TEST(ut_product_type, is_options) {
    EXPECT_TRUE(miu::ref::is_options(product_type::CALL));
    EXPECT_TRUE(miu::ref::is_options(product_type::PUT));

    EXPECT_FALSE(miu::ref::is_options(product_type::FUTURE));
    EXPECT_FALSE(miu::ref::is_options(product_type::STOCK));
}

TEST(ut_product_type, is_derivatives) {
    EXPECT_TRUE(miu::ref::is_derivatives(product_type::FUTURE));
    EXPECT_TRUE(miu::ref::is_derivatives(product_type::CALL));
    EXPECT_TRUE(miu::ref::is_derivatives(product_type::PUT));
    EXPECT_TRUE(miu::ref::is_derivatives(product_type::SPREAD));

    EXPECT_FALSE(miu::ref::is_derivatives(product_type::STOCK));
}

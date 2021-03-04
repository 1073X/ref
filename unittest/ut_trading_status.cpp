#include <gtest/gtest.h>

#include <com/reg_var_str.hpp>
#include <log/log.hpp>

#include "ref/trading_status.hpp"

using miu::com::variant;
using miu::ref::trading_status;

TEST(ut_trading_status, variant) {
    miu::com::reg_var_str<trading_status>();

    auto var = variant { trading_status::PRE_CLOSE };
    EXPECT_EQ(trading_status::PRE_CLOSE, var.get<trading_status>());
    EXPECT_EQ("PRE_CLOSE", var.get<std::string>());

    // from string
    EXPECT_EQ(trading_status::PRE_CLOSE, variant { +"PRE_CLOSE" }.get<trading_status>());
    EXPECT_EQ(trading_status::OPEN, variant { std::string("OPEN") }.get<trading_status>());

    // from number
    EXPECT_EQ(trading_status::AUCTION,
              variant { static_cast<int32_t>(trading_status::AUCTION) }.get<trading_status>());
    EXPECT_EQ(trading_status::PRE_OPEN,
              variant { static_cast<uint32_t>(trading_status::PRE_OPEN) }.get<trading_status>());

    // out of range
    EXPECT_EQ(trading_status::MAX, variant { +"unknown" }.get<trading_status>());
    EXPECT_EQ(trading_status::MAX, variant { -1 }.get<trading_status>());
    EXPECT_EQ(trading_status::MAX,
              variant { static_cast<uint32_t>(trading_status::MAX) + 1 }.get<trading_status>());
}

TEST(ut_trading_status, to_string) {
    EXPECT_EQ("OPEN", miu::com::to_string(trading_status::OPEN));
    EXPECT_EQ("CLOSE", miu::com::to_string(trading_status::CLOSE));
}


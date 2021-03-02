#include <gtest/gtest.h>

#include <com/reg_var_str.hpp>
#include <log/log.hpp>

#include "ref/status.hpp"

using miu::com::variant;
using miu::ref::status;

TEST(ut_status, variant) {
    miu::com::reg_var_str<status>();

    auto var = variant { status::PRE_CLOSE };
    EXPECT_EQ(status::PRE_CLOSE, var.get<status>());
    EXPECT_EQ("PRE_CLOSE", var.get<std::string>());

    // from string
    EXPECT_EQ(status::PRE_CLOSE, variant { +"PRE_CLOSE" }.get<status>());
    EXPECT_EQ(status::OPEN, variant { std::string("OPEN") }.get<status>());

    // from number
    EXPECT_EQ(status::AUCTION, variant { static_cast<int32_t>(status::AUCTION) }.get<status>());
    EXPECT_EQ(status::PRE_OPEN, variant { static_cast<uint32_t>(status::PRE_OPEN) }.get<status>());

    // out of range
    EXPECT_EQ(status::MAX, variant { +"unknown" }.get<status>());
    EXPECT_EQ(status::MAX, variant { -1 }.get<status>());
    EXPECT_EQ(status::MAX, variant { static_cast<uint32_t>(status::MAX) + 1 }.get<status>());
}

TEST(ut_status, to_string) {
    EXPECT_EQ("OPEN", miu::com::to_string(status::OPEN));
    EXPECT_EQ("CLOSE", miu::com::to_string(status::CLOSE));
}


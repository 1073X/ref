#include <gtest/gtest.h>

#include <com/reg_var_str.hpp>
#include <log/log.hpp>

#include "ref/phase_type.hpp"

using miu::com::variant;
using miu::ref::phase_type;

TEST(ut_phase_type, variant) {
    miu::com::reg_var_str<phase_type>();

    auto var = variant { phase_type::PRE_CLOSE };
    EXPECT_EQ(phase_type::PRE_CLOSE, var.get<phase_type>());
    EXPECT_EQ("PRE_CLOSE", var.get<std::string>());

    // from string
    EXPECT_EQ(phase_type::PRE_CLOSE, variant { +"PRE_CLOSE" }.get<phase_type>());
    EXPECT_EQ(phase_type::OPEN, variant { std::string("OPEN") }.get<phase_type>());

    // from number
    EXPECT_EQ(phase_type::AUCTION,
              variant { static_cast<int32_t>(phase_type::AUCTION) }.get<phase_type>());
    EXPECT_EQ(phase_type::PRE_OPEN,
              variant { static_cast<uint32_t>(phase_type::PRE_OPEN) }.get<phase_type>());

    // out of range
    EXPECT_EQ(phase_type::MAX, variant { +"unknown" }.get<phase_type>());
    EXPECT_EQ(phase_type::MAX, variant { -1 }.get<phase_type>());
    EXPECT_EQ(
        phase_type::MAX, variant { static_cast<uint32_t>(phase_type::MAX) + 1 }.get<phase_type>());
}

TEST(ut_phase_type, to_string) {
    EXPECT_EQ("OPEN", miu::com::to_string(phase_type::OPEN));
    EXPECT_EQ("CLOSE", miu::com::to_string(phase_type::CLOSE));
}

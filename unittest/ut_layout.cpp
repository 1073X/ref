#include <gtest/gtest.h>

#include "source/lib/layout.hpp"

using namespace miu::ref;

struct ut_layout : public testing::Test {
    char buf[4096] {};
};

TEST_F(ut_layout, resolve_size) {
    auto exp = sizeof(layout) + 8 * sizeof(instrument_impl) + 2 * sizeof(tiktable_impl)
             + 2 * sizeof(schedule_impl)
             // symbol map
             + 17 * 16
             // mkt_code map
             + 17 * 32;

    EXPECT_EQ(exp, layout::resolve_size(8, 2, 2));
}

TEST_F(ut_layout, make) {
    auto layout = layout::make(buf, "ut_layout", 8, 2, 2);
    ASSERT_NE(nullptr, layout);

    EXPECT_EQ("ut_layout", layout->name());

    EXPECT_EQ(8U, layout->instrument_capacity());
    EXPECT_EQ(0U, layout->instrument_count());
    auto schedule_addr = buf + CACHE_LINE;
    EXPECT_EQ(schedule_addr, (const char*)layout->schedules());

    EXPECT_EQ(2U, layout->tiktable_capacity());
    EXPECT_EQ(0U, layout->tiktable_count());
    auto instrument_addr = buf + CACHE_LINE * 3;
    EXPECT_EQ(instrument_addr, (const char*)layout->instruments());

    EXPECT_EQ(2U, layout->schedule_capacity());
    EXPECT_EQ(0U, layout->schedule_count());
    auto tiktable_addr = buf + CACHE_LINE * 11;
    EXPECT_EQ(tiktable_addr, (const char*)layout->tiktables());

    ASSERT_NE(nullptr, layout->symbol_map());
    EXPECT_EQ(16, layout->symbol_map()->capacity());

    ASSERT_NE(nullptr, layout->mkt_map());
    EXPECT_EQ(16, layout->mkt_map()->capacity());

    ASSERT_NE(nullptr, layout->trd_map());
    EXPECT_EQ(16, layout->trd_map()->capacity());
}

TEST_F(ut_layout, open) {
    layout::make(buf, "ut_layout", 8, 2, 2);

    auto layout = layout::open(buf);
    ASSERT_NE(nullptr, layout);

    EXPECT_EQ("ut_layout", layout->name());

    EXPECT_EQ(8U, layout->instrument_capacity());
    auto schedule_addr = buf + CACHE_LINE;
    EXPECT_EQ(schedule_addr, (const char*)layout->schedules());

    EXPECT_EQ(2U, layout->tiktable_capacity());
    auto instrument_addr = buf + CACHE_LINE * 3;
    EXPECT_EQ(instrument_addr, (const char*)layout->instruments());

    EXPECT_EQ(2U, layout->schedule_capacity());
    auto tiktable_addr = buf + CACHE_LINE * 11;
    EXPECT_EQ(tiktable_addr, (const char*)layout->tiktables());
}

TEST_F(ut_layout, structure) {
    auto layout = layout::make(buf, "ut_layout", 8, 2, 2);
    layout->restructure(4, 1, 1);
    EXPECT_EQ(4U, layout->instrument_count());
    EXPECT_EQ(1U, layout->tiktable_count());
    EXPECT_EQ(1U, layout->schedule_count());
    EXPECT_EQ(0x0000000400010001, layout->structure());
}


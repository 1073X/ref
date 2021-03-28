#include <gtest/gtest.h>

#include "source/lib/ref/layout.hpp"

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
             + 17 * 32
             // trd_code map
             + 17 * 32;

    EXPECT_EQ(exp, layout::resolve_size(8, 2, 2));
}

TEST_F(ut_layout, make) {
    auto layout = layout::make(buf, "ut_layout", 8, 2, 2);
    ASSERT_NE(nullptr, layout);

    EXPECT_EQ("ut_layout", layout->name());

    EXPECT_EQ(8U, layout->max_of_instrument());
    EXPECT_EQ(0U, layout->num_of_instrument());
    auto schedule_addr = buf + CACHE_LINE;
    EXPECT_EQ(schedule_addr, (const char*)layout->schedules());

    EXPECT_EQ(2U, layout->max_of_tiktable());
    EXPECT_EQ(0U, layout->num_of_tiktable());
    auto instrument_addr = buf + CACHE_LINE * 3;
    EXPECT_EQ(instrument_addr, (const char*)layout->instruments());

    EXPECT_EQ(2U, layout->max_of_schedule());
    EXPECT_EQ(0U, layout->num_of_schedule());
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

    EXPECT_EQ(8U, layout->max_of_instrument());
    auto schedule_addr = buf + CACHE_LINE;
    EXPECT_EQ(schedule_addr, (const char*)layout->schedules());

    EXPECT_EQ(2U, layout->max_of_tiktable());
    auto instrument_addr = buf + CACHE_LINE * 3;
    EXPECT_EQ(instrument_addr, (const char*)layout->instruments());

    EXPECT_EQ(2U, layout->max_of_schedule());
    auto tiktable_addr = buf + CACHE_LINE * 11;
    EXPECT_EQ(tiktable_addr, (const char*)layout->tiktables());
}

TEST_F(ut_layout, structure) {
    auto layout = layout::make(buf, "ut_layout", 8, 2, 2);

    // since lookup table doesn't accept duplicated key, we have to initialize the instruments;
    underlying_impl underlying { exchange_type::SSE, product_type::STOCK, "stock" };
    for (auto i = 0; i < 4; i++) {
        auto name = "stk" + std::to_string(i);
        auto inst = new (layout->instruments() + i) instrument_impl { underlying, name };
        inst->set_mkt_code(name);
        inst->set_trd_code(name);
    }

    layout->restructure(4, 1, 1);
    EXPECT_EQ(4U, layout->num_of_instrument());
    EXPECT_EQ(1U, layout->num_of_tiktable());
    EXPECT_EQ(1U, layout->num_of_schedule());
}

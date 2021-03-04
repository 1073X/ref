#include <gtest/gtest.h>

#include <time/time.hpp>

#include "source/lib/schedule.hpp"

using miu::ref::trading_status;
using miu::time::daytime;

struct ut_schedule : public testing::Test {
    void SetUp() override {
        schedule.add(daytime(8, 55, 0, 0), trading_status::AUCTION);
        schedule.add(daytime(8, 57, 0, 0), trading_status::PRE_OPEN);
        schedule.add(daytime(9, 0, 0, 0), trading_status::OPEN);
        schedule.add(daytime(10, 15, 0, 0), trading_status::CLOSE);
        schedule.add(daytime(10, 30, 0, 0), trading_status::OPEN);
        schedule.add(daytime(13, 30, 0, 0), trading_status::PRE_CLOSE);
        schedule.add(daytime(15, 0, 0, 0), trading_status::CLOSE);
        schedule.add(daytime(21, 0, 0, 0), trading_status::OPEN);
        schedule.add(daytime(0, 0, 0, 0), trading_status::OPEN);
        schedule.add(daytime(2, 0, 0, 0), trading_status::CLOSE);
    }

    miu::ref::schedule schedule;
};

TEST_F(ut_schedule, determine) {
    EXPECT_EQ(trading_status::CLOSE, schedule.determine(daytime(8, 50, 0, 0)));
    EXPECT_EQ(trading_status::AUCTION, schedule.determine(daytime(8, 55, 0, 0)));
    EXPECT_EQ(trading_status::OPEN, schedule.determine(daytime(9, 0, 0, 0)));
    EXPECT_EQ(trading_status::OPEN, schedule.determine(daytime(9, 3, 0, 0)));
    EXPECT_EQ(trading_status::CLOSE, schedule.determine(daytime(10, 15, 0, 0)));
    EXPECT_EQ(trading_status::PRE_CLOSE, schedule.determine(daytime(13, 31, 0, 0)));
    EXPECT_EQ(trading_status::CLOSE, schedule.determine(daytime(15, 0, 0, 0)));
    EXPECT_EQ(trading_status::OPEN, schedule.determine(daytime(21, 0, 0, 0)));
    EXPECT_EQ(trading_status::OPEN, schedule.determine(daytime(0, 0, 0, 0)));
    EXPECT_EQ(trading_status::CLOSE, schedule.determine((miu::time::today(), daytime(2, 0, 0, 0))));
}

TEST_F(ut_schedule, deep_copy) {
    EXPECT_EQ(schedule, schedule);

    miu::ref::schedule copy { schedule };
    EXPECT_EQ(copy, schedule);

    miu::ref::schedule copy2;
    copy2 = schedule;
    EXPECT_EQ(copy2, schedule);
}

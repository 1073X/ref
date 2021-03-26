#include <gtest/gtest.h>

#include <time/time.hpp>

#include "source/lib/schedule_impl.hpp"

using miu::ref::phase_type;
using miu::time::daytime;

struct ut_schedule_impl : public testing::Test {
    void SetUp() override {
        schedule.add(daytime(8, 55, 0, 0), phase_type::AUCTION);
        schedule.add(daytime(8, 57, 0, 0), phase_type::PRE_OPEN);
        schedule.add(daytime(9, 0, 0, 0), phase_type::OPEN);
        schedule.add(daytime(10, 15, 0, 0), phase_type::CLOSE);
        schedule.add(daytime(10, 30, 0, 0), phase_type::OPEN);
        schedule.add(daytime(13, 30, 0, 0), phase_type::PRE_CLOSE);
        schedule.add(daytime(15, 0, 0, 0), phase_type::CLOSE);
        schedule.add(daytime(21, 0, 0, 0), phase_type::OPEN);
        schedule.add(daytime(0, 0, 0, 0), phase_type::OPEN);
        schedule.add(daytime(2, 0, 0, 0), phase_type::CLOSE);
    }

    miu::ref::schedule_impl schedule;
};

TEST_F(ut_schedule_impl, determine) {
    EXPECT_EQ(phase_type::CLOSE, schedule.determine(daytime(8, 50, 0, 0)));
    EXPECT_EQ(phase_type::AUCTION, schedule.determine(daytime(8, 55, 0, 0)));
    EXPECT_EQ(phase_type::OPEN, schedule.determine(daytime(9, 0, 0, 0)));
    EXPECT_EQ(phase_type::OPEN, schedule.determine(daytime(9, 3, 0, 0)));
    EXPECT_EQ(phase_type::CLOSE, schedule.determine(daytime(10, 15, 0, 0)));
    EXPECT_EQ(phase_type::PRE_CLOSE, schedule.determine(daytime(13, 31, 0, 0)));
    EXPECT_EQ(phase_type::CLOSE, schedule.determine(daytime(15, 0, 0, 0)));
    EXPECT_EQ(phase_type::OPEN, schedule.determine(daytime(21, 0, 0, 0)));
    EXPECT_EQ(phase_type::OPEN, schedule.determine(daytime(0, 0, 0, 0)));
    EXPECT_EQ(phase_type::CLOSE, schedule.determine((miu::time::today(), daytime(2, 0, 0, 0))));
}

TEST_F(ut_schedule_impl, deep_copy) {
    EXPECT_EQ(schedule, schedule);

    miu::ref::schedule_impl copy { schedule };
    EXPECT_EQ(copy, schedule);

    miu::ref::schedule_impl copy2;
    copy2 = schedule;
    EXPECT_EQ(copy2, schedule);
}

TEST_F(ut_schedule_impl, invalid) {
    miu::ref::schedule_impl schedule2;
    EXPECT_ANY_THROW(schedule2.add(daytime(9, 0, 0, 0), phase_type::MAX));
}

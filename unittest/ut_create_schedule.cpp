#include <gtest/gtest.h>

#include "source/bin/loadref/json_source.hpp"

using namespace miu::ref;

TEST(ut_create_schedule, create) {
    schedule_impl schedule;

    miu::com::json json;
    json["00:00:00.000"] = "OPEN";
    json["21:00:00.000"] = "CLOSE";

    EXPECT_NO_THROW(json_source::create(&schedule, json, "name"));
    EXPECT_EQ(3U, schedule.segements());
}

TEST(ut_create_schedule, invalid_key) {
    schedule_impl schedule;

    miu::com::json json;
    json["abc"]          = "OPEN";
    json["21:00:00.000"] = "CLOSE";

    EXPECT_ANY_THROW(json_source::create(&schedule, json, "name"));
}

TEST(ut_create_schedule, invalid_val) {
    schedule_impl schedule;

    miu::com::json json;
    json["00:00:00.000"] = "xyz";
    json["21:00:00.000"] = "CLOSE";

    EXPECT_ANY_THROW(json_source::create(&schedule, json, "name"));
}

TEST(ut_create_schedule, empty) {
    schedule_impl schedule;

    miu::com::json json;

    EXPECT_ANY_THROW(json_source::create(&schedule, json, "name"));
}

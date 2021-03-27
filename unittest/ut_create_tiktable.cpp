#include <gtest/gtest.h>

#include "source/bin/loadref/json_source.hpp"

using namespace miu::ref;

TEST(ut_create_tiktable, create) {
    tiktable_impl tiktable;

    miu::com::json json;
    json["0.0"] = 0.1;
    json["10"]  = 1;

    EXPECT_NO_THROW(json_source::create(&tiktable, json, "name"));
    EXPECT_EQ(3U, tiktable.segements());
}

TEST(ut_create_tiktable, invalid_key) {
    tiktable_impl tiktable;

    miu::com::json json;
    json["abc"] = 0.1;
    json["10"]  = 0.5;

    EXPECT_ANY_THROW(json_source::create(&tiktable, json, "name"));
}

TEST(ut_create_tiktable, invalid_val) {
    tiktable_impl tiktable;

    miu::com::json json;
    json["0.0"] = 0.1;
    json["10"]  = "abc";

    EXPECT_ANY_THROW(json_source::create(&tiktable, json, "name"));
}

TEST(ut_create_tiktable, empty) {
    tiktable_impl tiktable;

    miu::com::json json;

    EXPECT_ANY_THROW(json_source::create(&tiktable, json, "name"));
}

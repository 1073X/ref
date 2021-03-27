#include <gtest/gtest.h>

#include "source/bin/loadref/json_source.hpp"

using namespace miu::ref;

TEST(ut_create_underlying, create) {
    underlying_impl underlying { "CFFEX/FUTURE/AG" };

    std::map<std::string, uint16_t> tiktable_ids { { "tk1", 99 } };
    std::map<std::string, uint16_t> schedule_ids { { "th1", 99 } };

    miu::com::json json;
    json["currency"]   = "CNH";
    json["lot_size"]   = 100;
    json["multiplier"] = 1;
    json["schedule"]   = "th1";
    json["tiktable"]   = "tk1";

    EXPECT_NO_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));
    EXPECT_EQ(currency_type::CNH, underlying.currency());
    EXPECT_EQ(100, underlying.lot_size());
    EXPECT_EQ(1, underlying.multiplier());
    EXPECT_EQ(99U, underlying.tiktable_id());
    EXPECT_EQ(99U, underlying.schedule_id());
}

TEST(ut_create_underlying, invalid_currency) {
    underlying_impl underlying { "CFFEX/FUTURE/AG" };

    std::map<std::string, uint16_t> tiktable_ids { { "tk1", 99 } };
    std::map<std::string, uint16_t> schedule_ids { { "th1", 99 } };

    // missing
    miu::com::json json;
    json["lot_size"]   = 100;
    json["multiplier"] = 1;
    json["schedule"]   = "th1";
    json["tiktable"]   = "tk1";
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    // invalid value
    json["currency"] = "abc";
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    json["currency"] = 1;
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    json["currency"] = true;
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));
}

TEST(ut_create_underlying, invalid_lot_size) {
    underlying_impl underlying { "CFFEX/FUTURE/AG" };

    std::map<std::string, uint16_t> tiktable_ids { { "tk1", 99 } };
    std::map<std::string, uint16_t> schedule_ids { { "th1", 99 } };

    // missing
    miu::com::json json;
    json["currency"]   = "USD";
    json["multiplier"] = 1;
    json["schedule"]   = "th1";
    json["tiktable"]   = "tk1";
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    // invalid value
    json["lot_size"] = 1.2;
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    json["lot_size"] = -1;
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    json["lot_size"] = 0;
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    json["lot_size"] = "abc";
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));
}

TEST(ut_create_underlying, invalid_multiplier) {
    underlying_impl underlying { "CFFEX/FUTURE/AG" };

    std::map<std::string, uint16_t> tiktable_ids { { "tk1", 99 } };
    std::map<std::string, uint16_t> schedule_ids { { "th1", 99 } };

    // missing
    miu::com::json json;
    json["currency"] = "USD";
    json["lot_size"] = 1;
    json["schedule"] = "th1";
    json["tiktable"] = "tk1";
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    // invalid value
    json["multiplier"] = 1.2;
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    json["multiplier"] = -1;
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    json["multiplier"] = 0;
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    json["multiplier"] = "abc";
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));
}

TEST(ut_create_underlying, invalid_tiktable) {
    underlying_impl underlying { "CFFEX/FUTURE/AG" };

    std::map<std::string, uint16_t> tiktable_ids { { "tk1", 99 } };
    std::map<std::string, uint16_t> schedule_ids { { "th1", 99 } };

    // missing
    miu::com::json json;
    json["currency"]   = "USD";
    json["lot_size"]   = 1;
    json["multiplier"] = 1;
    json["schedule"]   = "th1";
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    // invalid value
    json["tiktable"] = 1;
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    json["tiktable"] = "abc";
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));
}

TEST(ut_create_underlying, invalid_schedule) {
    underlying_impl underlying { "CFFEX/FUTURE/AG" };

    std::map<std::string, uint16_t> tiktable_ids { { "tk1", 99 } };
    std::map<std::string, uint16_t> schedule_ids { { "th1", 99 } };

    // missing
    miu::com::json json;
    json["currency"]   = "USD";
    json["lot_size"]   = 1;
    json["multiplier"] = 1;
    json["tiktable"]   = "tk1";
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    // invalid value
    json["schedule"] = 1;
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));

    json["schedule"] = "abc";
    EXPECT_ANY_THROW(json_source::create(&underlying, json, tiktable_ids, schedule_ids));
}

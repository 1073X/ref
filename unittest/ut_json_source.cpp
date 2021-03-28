#include <gtest/gtest.h>

#include "ref/reg_var_str.hpp"
#include "source/lib/loadref/json_source.hpp"

using namespace miu::ref;

struct ut_json_source : public testing::Test {
    void SetUp() override {
        miu::ref::reg_var_str();

        using miu::log::severity;
        // miu::log::reset(severity::DEBUG, 1024);

        ///
        json["tiktables"]["tk0"]["0.0"] = 0.5;
        json["tiktables"]["tk1"]["0.0"] = 0.01;

        ///
        json["schedules"]["th0"]["00:00:00.000"] = "OPEN";
        json["schedules"]["th0"]["23:59:59.999"] = "CLOSE";

        json["schedules"]["th1"]["08:00:00.000"] = "OPEN";
        json["schedules"]["th1"]["21:00:00.000"] = "CLOSE";

        ///
        miu::com::json a_stk;
        a_stk["currency"]                      = "CNH";
        a_stk["lot_size"]                      = 100;
        a_stk["multiplier"]                    = 1;
        a_stk["schedule"]                      = "th1";
        a_stk["tiktable"]                      = "tk1";
        json["underlyings"]["SSE/STOCK/A_STK"] = a_stk;
        miu::com::json b_fut;
        b_fut["currency"]                       = "USD";
        b_fut["lot_size"]                       = 1;
        b_fut["multiplier"]                     = 1;
        b_fut["schedule"]                       = "th0";
        b_fut["tiktable"]                       = "tk0";
        json["underlyings"]["SSE/FUTURE/B_FUT"] = b_fut;
        miu::com::json c_opt;
        c_opt["currency"]                  = "JPY";
        c_opt["lot_size"]                  = 1;
        c_opt["multiplier"]                = 5;
        c_opt["schedule"]                  = "th1";
        c_opt["tiktable"]                  = "tk1";
        json["underlyings"]["SSE/PUT/OPT"] = c_opt;

        ///
        miu::com::json inst1;
        inst1["name"]                             = "600000";
        inst1["mkt_code"]                         = "600000M";
        json["instruments"]["SSE/STOCK/A_STK"][0] = inst1;

        miu::com::json inst2;
        inst2["maturity"]                          = "20210306";
        inst2["trd_code"]                          = "B_FUT_T";
        json["instruments"]["SSE/FUTURE/B_FUT"][0] = inst2;

        miu::com::json inst3;
        inst3["strike_price"]                 = 1.2;
        inst3["maturity"]                     = "20210306";
        inst3["mkt_code"]                     = "OPT_M";
        inst3["trd_code"]                     = "OPT_T";
        json["instruments"]["SSE/PUT/OPT"][0] = inst3;
    }

    void TearDown() override {
        // miu::log::dump();
    }

    miu::com::json json;
};

TEST_F(ut_json_source, tiktable) {
    json_source source { json };
    EXPECT_EQ(2U, source.num_of_tiktable());

    tiktable_impl buf[4];
    auto ids = source.fill(buf, sizeof(buf));
    EXPECT_EQ(2U, ids.size());
    EXPECT_EQ(0U, ids["tk0"]);
    EXPECT_EQ(1U, ids["tk1"]);
}

TEST_F(ut_json_source, schedule) {
    json_source source { json };
    EXPECT_EQ(2U, source.num_of_schedule());

    schedule_impl buf[4];
    auto ids = source.fill(buf, sizeof(buf));
    EXPECT_EQ(2U, ids.size());
    EXPECT_EQ(0U, ids["th0"]);
    EXPECT_EQ(1U, ids["th1"]);
}

TEST_F(ut_json_source, underlying) {
    json_source source { json };
    EXPECT_EQ(3U, source.num_of_underlying());

    std::map<std::string, uint16_t> tiktable_ids { { "tk0", 100 }, { "tk1", 99 } };
    std::map<std::string, uint16_t> schedule_ids { { "th0", 100 }, { "th1", 99 } };

    underlying_impl buf[4];
    auto ids = source.fill(buf, sizeof(buf), tiktable_ids, schedule_ids);
    EXPECT_EQ(3U, ids.size());

    EXPECT_EQ(0U, ids[{ "SSE/STOCK/A_STK" }]);
    EXPECT_EQ(1U, ids[{ "SSE/FUTURE/B_FUT" }]);
    EXPECT_EQ(2U, ids[{ "SSE/PUT/OPT" }]);
}

TEST_F(ut_json_source, layout) {
    json_source source { json };

    char buf[4096] {};
    auto layout = layout::make(buf,
                               "name",
                               source.num_of_instrument(),
                               source.num_of_tiktable(),
                               source.num_of_schedule());

    auto ids = source.fill(layout);
    EXPECT_EQ(3U, ids.size());

    EXPECT_EQ(0U, ids[symbol("SSE/STOCK/600000")]);
    EXPECT_EQ(1U, ids[symbol("SSE/FUTURE/B_FUT/2103")]);
    EXPECT_EQ(2U, ids[symbol("SSE/PUT/OPT/12000000/2103")]);

    EXPECT_EQ(source.num_of_tiktable(), layout->num_of_tiktable());
    EXPECT_EQ(source.num_of_schedule(), layout->num_of_schedule());
    EXPECT_EQ(source.num_of_instrument(), layout->num_of_instrument());
}

TEST_F(ut_json_source, general_code) {
    json_source source { json };

    char buf[4096] {};
    auto layout = layout::make(buf,
                               "name",
                               source.num_of_instrument(),
                               source.num_of_tiktable(),
                               source.num_of_schedule());

    source.fill(layout);

    auto inst0 = layout->instruments();
    EXPECT_STREQ("600000M", inst0->mkt_code().value);
    EXPECT_STREQ("600000M", inst0->trd_code().value);

    auto inst1 = layout->instruments() + 1;
    EXPECT_STREQ("B_FUT_T", inst1->mkt_code().value);
    EXPECT_STREQ("B_FUT_T", inst1->trd_code().value);
}

#include <gtest/gtest.h>

#include <com/json.hpp>
#include <fstream>
#include <log/log.hpp>
#include <shm/tempfs.hpp>

#include "ref/database.hpp"
#include "ref/reg_var_str.hpp"
#include "source/bin/loadref/loader.hpp"

using namespace miu::ref;

struct ut_database : public testing::Test {
    void SetUp() override {
        reg_var_str();

        using miu::log::severity;
        miu::log::reset(severity::DEBUG, 1024);

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
        inst1["trd_code"]                         = "600000T";
        json["instruments"]["SSE/STOCK/A_STK"][0] = inst1;

        miu::com::json inst2;
        inst2["maturity"]                          = "20210306";
        inst2["mkt_code"]                          = "B_FUT_M";
        inst2["trd_code"]                          = "B_FUT_T";
        json["instruments"]["SSE/FUTURE/B_FUT"][0] = inst2;

        miu::com::json inst3;
        inst3["strike_price"]                 = 1.2;
        inst3["maturity"]                     = "20210306";
        inst3["mkt_code"]                     = "OPT_M";
        inst3["trd_code"]                     = "OPT_T";
        json["instruments"]["SSE/PUT/OPT"][0] = inst3;

        std::ofstream { "ut_database.json" } << std::setw(2) << json;
    }

    void TearDown() override {
        miu::log::dump();
        miu::shm::tempfs::remove("ut_database.ref");
    }

    miu::com::json json;
};

TEST_F(ut_database, load) {
    auto db = miu::ref::loader::load("ut_database.json");
    EXPECT_EQ("ut_database", db.name());
    EXPECT_EQ(3U, db.size());
}

TEST_F(ut_database, open) {
    miu::ref::loader::load("ut_database.json");

    auto db = database::open("ut_database", miu::shm::mode::READ);
    EXPECT_EQ("ut_database", db.name());
    EXPECT_EQ(3U, db.size());
}

TEST_F(ut_database, find_by_index) {
    auto db = miu::ref::loader::load("ut_database.json");

    EXPECT_EQ(symbol("SSE/STOCK/600000"), db.find(0).symbol());
    EXPECT_EQ(symbol("SSE/FUTURE/B_FUT/2103"), db.find(1).symbol());
    EXPECT_EQ(symbol("SSE/PUT/OPT/12000000/2103"), db.find(2).symbol());
    EXPECT_FALSE(db.find(3));
}

TEST_F(ut_database, find_by_symbol) {
    auto db = miu::ref::loader::load("ut_database.json");

    EXPECT_FALSE(db.find(symbol {}));

    EXPECT_EQ(0U, db.find(symbol("SSE/STOCK/600000")).id());
    EXPECT_EQ(1U, db.find(symbol("SSE/FUTURE/B_FUT/2103")).id());
    EXPECT_EQ(2U, db.find(symbol("SSE/PUT/OPT/12000000/2103")).id());

    EXPECT_FALSE(db.find(symbol("SSE/STOCK/noexist")));
}

TEST_F(ut_database, find_by_mkt_code) {
    auto db = miu::ref::loader::load("ut_database.json");

    EXPECT_FALSE(db.find(symbol {}));

    EXPECT_EQ(0U, db.find_by_mkt_code("600000M").id());
    EXPECT_EQ(1U, db.find_by_mkt_code("B_FUT_M").id());
    EXPECT_EQ(2U, db.find_by_mkt_code("OPT_M").id());

    EXPECT_FALSE(db.find_by_mkt_code("not_exists"));
}

TEST_F(ut_database, find_by_trd_code) {
    auto db = miu::ref::loader::load("ut_database.json");

    EXPECT_FALSE(db.find(symbol {}));

    EXPECT_EQ(0U, db.find_by_trd_code("600000T").id());
    EXPECT_EQ(1U, db.find_by_trd_code("B_FUT_T").id());
    EXPECT_EQ(2U, db.find_by_trd_code("OPT_T").id());

    EXPECT_FALSE(db.find_by_trd_code("not_exists"));
}

TEST_F(ut_database, signature) {
    auto db = miu::ref::loader::load("ut_database.json");

    auto signature = db.signature();
    std::cout << miu::com::to_string(signature) << std::endl;

    EXPECT_EQ(signature, db.signature());
}

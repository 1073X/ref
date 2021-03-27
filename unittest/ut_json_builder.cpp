#include <gtest/gtest.h>

#include "source/bin/refview/json_builder.hpp"

using namespace miu::ref;

struct ut_json_builder : public testing::Test {
    void SetUp() override {
        // schedules
        schedule_impl schedule1;
        schedule1.add(miu::time::daytime { 9, 0, 0, 0 }, phase_type::OPEN);
        schedule1.add(miu::time::daytime { 11, 0, 0, 0 }, phase_type::CLOSE);

        schedule_impl schedule2;
        schedule2.add(miu::time::daytime { 0, 0, 0, 0 }, phase_type::OPEN);
        schedule2.add(miu::time::daytime { 11, 0, 0, 0 }, phase_type::CLOSE);
        schedule2.add(miu::time::daytime { 13, 0, 0, 0 }, phase_type::OPEN);

        builder.append(&schedule1);
        builder.append(&schedule2);

        // tiktables
        tiktable_impl tiktable1;
        tiktable1.add(0.0, 0.1);

        tiktable_impl tiktable2;
        tiktable2.add(0.0, 0.5);
        tiktable2.add(2.5, 0.8);
        tiktable2.add(5, 1);

        builder.append(&tiktable1);
        builder.append(&tiktable2);
    }

    json_builder builder;
};

TEST_F(ut_json_builder, append_schedule) {
    auto json = builder.json();

    EXPECT_EQ(2U, json["schedules"]["TH00"].size());
    EXPECT_EQ("OPEN", json["schedules"]["TH00"]["09:00:00.000"]);
    EXPECT_EQ("CLOSE", json["schedules"]["TH00"]["11:00:00.000"]);

    EXPECT_EQ(3U, json["schedules"]["TH01"].size());
    EXPECT_EQ("OPEN", json["schedules"]["TH01"]["00:00:00.000"]);
    EXPECT_EQ("CLOSE", json["schedules"]["TH01"]["11:00:00.000"]);
    EXPECT_EQ("OPEN", json["schedules"]["TH01"]["13:00:00.000"]);
}

TEST_F(ut_json_builder, append_tiktable) {
    auto json = builder.json();

    EXPECT_EQ(1U, json["tiktables"]["TK00"].size());
    EXPECT_DOUBLE_EQ(0.1, json["tiktables"]["TK00"]["0"]);

    EXPECT_EQ(3U, json["tiktables"]["TK01"].size());
    EXPECT_DOUBLE_EQ(0.5, json["tiktables"]["TK01"]["0"]);
    EXPECT_DOUBLE_EQ(0.8, json["tiktables"]["TK01"]["2.5"]);
    EXPECT_DOUBLE_EQ(1.0, json["tiktables"]["TK01"]["5"]);
}

TEST_F(ut_json_builder, append_general) {
    underlying_impl underlying { exchange_type::NASDAQ, product_type::STOCK, "name" };
    underlying.set_currency(currency_type::CNH);
    underlying.set_lot_size(100);
    underlying.set_multiplier(1);
    underlying.set_tiktable_id(0);
    underlying.set_schedule_id(1);

    instrument_impl instrument1 { underlying, "stk1" };
    instrument1.set_mkt_code("mkt1");
    instrument1.set_trd_code("trd1");
    instrument_impl instrument2 { underlying, "stk2" };
    instrument2.set_mkt_code("mkt2");
    instrument2.set_trd_code("trd2");

    builder.append(&instrument1);
    builder.append(&instrument2);

    auto json = builder.json();

    EXPECT_EQ("mkt1", json["instruments"]["NASDAQ/STOCK/stk1"][0]["mkt_code"]);
    EXPECT_EQ("trd1", json["instruments"]["NASDAQ/STOCK/stk1"][0]["trd_code"]);

    // share the same underlying
    EXPECT_EQ("mkt2", json["instruments"]["NASDAQ/STOCK/stk1"][1]["mkt_code"]);
    EXPECT_EQ("trd2", json["instruments"]["NASDAQ/STOCK/stk1"][1]["trd_code"]);

    EXPECT_EQ(1U, json["underlyings"].size());
    EXPECT_EQ("CNH", json["underlyings"]["NASDAQ/STOCK/stk1"]["currency"]);
    EXPECT_EQ(100, json["underlyings"]["NASDAQ/STOCK/stk1"]["lot_size"]);
    EXPECT_EQ(1, json["underlyings"]["NASDAQ/STOCK/stk1"]["multiplier"]);
    EXPECT_EQ("TK00", json["underlyings"]["NASDAQ/STOCK/stk1"]["tiktable"]);
    EXPECT_EQ("TH01", json["underlyings"]["NASDAQ/STOCK/stk1"]["schedule"]);
}

TEST_F(ut_json_builder, append_future) {
    underlying_impl underlying { exchange_type::SSE, product_type::FUTURE, "name" };
    underlying.set_currency(currency_type::CNH);
    underlying.set_lot_size(10);
    underlying.set_multiplier(10);
    underlying.set_tiktable_id(1);
    underlying.set_schedule_id(0);

    instrument_impl instrument1 { underlying, miu::time::date { 2021, 3, 27 } };
    instrument1.set_mkt_code("mkt1");
    instrument1.set_trd_code("trd1");

    instrument_impl instrument2 { underlying, miu::time::date { 2021, 4, 27 } };
    instrument2.set_mkt_code("mkt2");
    instrument2.set_trd_code("trd2");

    builder.append(&instrument1);
    builder.append(&instrument2);

    auto json = builder.json();

    EXPECT_EQ("mkt1", json["instruments"]["SSE/FUTURE/name"][0]["mkt_code"]);
    EXPECT_EQ("trd1", json["instruments"]["SSE/FUTURE/name"][0]["trd_code"]);
    EXPECT_EQ("20210327", json["instruments"]["SSE/FUTURE/name"][0]["maturity"]);

    EXPECT_EQ("mkt2", json["instruments"]["SSE/FUTURE/name"][1]["mkt_code"]);
    EXPECT_EQ("trd2", json["instruments"]["SSE/FUTURE/name"][1]["trd_code"]);
    EXPECT_EQ("20210427", json["instruments"]["SSE/FUTURE/name"][1]["maturity"]);

    EXPECT_EQ(1U, json["underlyings"].size());
    EXPECT_EQ("CNH", json["underlyings"]["SSE/FUTURE/name"]["currency"]);
    EXPECT_EQ(10, json["underlyings"]["SSE/FUTURE/name"]["lot_size"]);
    EXPECT_EQ(10, json["underlyings"]["SSE/FUTURE/name"]["multiplier"]);
    EXPECT_EQ("TK01", json["underlyings"]["SSE/FUTURE/name"]["tiktable"]);
    EXPECT_EQ("TH00", json["underlyings"]["SSE/FUTURE/name"]["schedule"]);
}

TEST_F(ut_json_builder, append_diff_underlying) {
    underlying_impl underlying1 { exchange_type::SSE, product_type::FUTURE, "name1" };
    underlying1.set_currency(currency_type::CNH);
    underlying1.set_lot_size(10);
    underlying1.set_multiplier(10);
    underlying1.set_tiktable_id(1);
    underlying1.set_schedule_id(0);
    instrument_impl instrument1 { underlying1, miu::time::date { 2021, 3, 27 } };

    underlying_impl underlying2 { exchange_type::SSE, product_type::FUTURE, "name2" };
    underlying2.set_currency(currency_type::CNH);
    underlying2.set_lot_size(10);
    underlying2.set_multiplier(10);
    underlying2.set_tiktable_id(1);
    underlying2.set_schedule_id(0);
    instrument_impl instrument2 { underlying2, miu::time::date { 2021, 4, 27 } };

    builder.append(&instrument1);
    builder.append(&instrument2);

    auto json = builder.json();

    EXPECT_EQ(1U, json["instruments"]["SSE/FUTURE/name1"].size());
    EXPECT_EQ(1U, json["instruments"]["SSE/FUTURE/name2"].size());

    EXPECT_EQ(2U, json["underlyings"].size());
    EXPECT_TRUE(json["underlyings"].contains("SSE/FUTURE/name1"));
    EXPECT_TRUE(json["underlyings"].contains("SSE/FUTURE/name2"));
}

TEST_F(ut_json_builder, append_options) {
    underlying_impl underlying { exchange_type::TSE, product_type::PUT, "abc" };
    underlying.set_currency(currency_type::JPY);
    underlying.set_lot_size(10);
    underlying.set_multiplier(10);
    underlying.set_tiktable_id(1);
    underlying.set_schedule_id(0);

    instrument_impl instrument { underlying, 1.2, miu::time::date { 2021, 3, 27 } };
    instrument.set_mkt_code("mkt_code");
    instrument.set_trd_code("trd_code");

    builder.append(&instrument);

    auto json = builder.json();

    EXPECT_EQ("mkt_code", json["instruments"]["TSE/PUT/abc"][0]["mkt_code"]);
    EXPECT_EQ("trd_code", json["instruments"]["TSE/PUT/abc"][0]["trd_code"]);
    EXPECT_EQ("20210327", json["instruments"]["TSE/PUT/abc"][0]["maturity"]);
    EXPECT_EQ(1.2, json["instruments"]["TSE/PUT/abc"][0]["strike_price"]);

    EXPECT_EQ("JPY", json["underlyings"]["TSE/PUT/abc"]["currency"]);
    EXPECT_EQ(10, json["underlyings"]["TSE/PUT/abc"]["lot_size"]);
    EXPECT_EQ(10, json["underlyings"]["TSE/PUT/abc"]["multiplier"]);
    EXPECT_EQ("TK01", json["underlyings"]["TSE/PUT/abc"]["tiktable"]);
    EXPECT_EQ("TH00", json["underlyings"]["TSE/PUT/abc"]["schedule"]);
}


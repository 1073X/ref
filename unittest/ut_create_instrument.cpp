#include <gtest/gtest.h>

#include "source/lib/json_source.hpp"

using namespace miu::ref;

TEST(ut_create_instrument, create_general) {
    underlying_impl underlying { "SSE/SWAP/ABC" };

    miu::com::json json;
    json["name"]     = "abcde";
    json["mkt_code"] = "mkt_abcde";
    json["trd_code"] = "trd_abcde";

    instrument_impl instrument;

    EXPECT_NO_THROW(json_source::create(&instrument, json, underlying));
    EXPECT_EQ(symbol("SSE/SWAP/abcde"), instrument.symbol());
    EXPECT_STREQ("mkt_abcde", instrument.mkt_code().value);
    EXPECT_STREQ("trd_abcde", instrument.trd_code().value);
}

TEST(ut_create_instrument, invalid_name) {
    underlying_impl underlying { "SSE/SWAP/ABC" };

    miu::com::json json;
    json["mkt_code"] = "mkt_abcde";
    json["trd_code"] = "trd_abcde";

    instrument_impl instrument;

    // missing
    EXPECT_ANY_THROW(json_source::create(&instrument, json, underlying));

    // invalid
    json["name"] = 1;
    EXPECT_ANY_THROW(json_source::create(&instrument, json, underlying));
}

TEST(ut_create_instrument, create_future) {
    underlying_impl underlying { "SSE/FUTURE/ABC" };

    miu::com::json json;
    json["maturity"] = "20210306";
    json["mkt_code"] = "mkt_abcde";
    json["trd_code"] = "trd_abcde";

    instrument_impl instrument;

    EXPECT_NO_THROW(json_source::create(&instrument, json, underlying));
    EXPECT_EQ(symbol("SSE/FUTURE/ABC/2103"), instrument.symbol());
    EXPECT_EQ(miu::time::date(2021, 3, 6), instrument.maturity());
    EXPECT_STREQ(+"mkt_abcde", instrument.mkt_code().value);
    EXPECT_STREQ(+"trd_abcde", instrument.trd_code().value);
}

TEST(ut_create_instrument, invalid_maturity) {
    underlying_impl underlying { "SSE/FUTURE/ABC" };

    miu::com::json json;
    json["mkt_code"] = "mkt_abcde";
    json["trd_code"] = "trd_abcde";

    instrument_impl instrument;

    // missing
    EXPECT_ANY_THROW(json_source::create(&instrument, json, underlying));
}

TEST(ut_create_instrument, create_options) {
    underlying_impl underlying { "SSE/CALL/ABC" };

    miu::com::json json;
    json["maturity"]     = "20210306";
    json["strike_price"] = 1.2;
    json["mkt_code"]     = "mkt_abcde";
    json["trd_code"]     = "trd_abcde";

    instrument_impl instrument;

    EXPECT_NO_THROW(json_source::create(&instrument, json, underlying));
    EXPECT_EQ(symbol("SSE/CALL/ABC/12000000/2103"), instrument.symbol());
    EXPECT_EQ(1.2, instrument.strike_price());
    EXPECT_EQ(miu::time::date(2021, 3, 6), instrument.maturity());
    EXPECT_STREQ(+"mkt_abcde", instrument.mkt_code().value);
    EXPECT_STREQ(+"trd_abcde", instrument.trd_code().value);
}

TEST(ut_create_instrument, invalid_strike) {
    underlying_impl underlying { "SSE/CALL/ABC" };

    miu::com::json json;
    json["maturity"] = "20210306";
    json["mkt_code"] = "mkt_abcde";
    json["trd_code"] = "trd_abcde";

    instrument_impl instrument;

    // missing
    EXPECT_ANY_THROW(json_source::create(&instrument, json, underlying));
}


#include <gtest/gtest.h>

#include <com/reg_var_str.hpp>

#include "ref/symbol.hpp"

using namespace std::chrono_literals;
using namespace miu::ref;
using miu::com::variant;
using miu::time::date;

TEST(ut_symbol, default) {
    symbol sym;
    EXPECT_EQ(0U, sym.val());

    EXPECT_EQ(exchange_type::UNDEF, sym.exchange());
    EXPECT_EQ(product_type::UNDEF, sym.product());

    EXPECT_FALSE(sym);
    EXPECT_TRUE(!sym);
}

TEST(ut_symbol, equility) {
    symbol sym1 { 1 };
    symbol sym2 { 2 };
    EXPECT_NE(sym1, sym2);

    symbol sym3 { 2 };
    EXPECT_EQ(sym3, sym2);
}

TEST(ut_symbol, options) {
    symbol sym { exchange_type::SSE, product_type::CALL, "AB", 1.2, date { 2021, 3, 3 } };
    EXPECT_EQ(exchange_type::SSE, sym.exchange());
    EXPECT_EQ(product_type::CALL, sym.product());
    EXPECT_EQ("AB", sym.name());
    EXPECT_EQ("SSE/CALL/AB/12000000/2103", sym.str());

    EXPECT_EQ(sym, symbol { sym.val() });                 // from value
    EXPECT_EQ(sym, symbol { sym.str() });                 // from string
    EXPECT_EQ(sym, symbol { "SSE/CALL/AB/1.2/2103" });    // from string

    // not options
    EXPECT_ANY_THROW(symbol(exchange_type::SSE, product_type::STOCK, "ABC", 100, date(2021, 3, 3)));

    // strike price overflow
    EXPECT_ANY_THROW(
        symbol(exchange_type::SSE, product_type::PUT, "AB", 17000000, date(2021, 3, 3)));
}

TEST(ut_symbol, futures) {
    symbol sym { exchange_type::CME, "ABCDEF", date { 2021, 3, 3 } };
    EXPECT_EQ(exchange_type::CME, sym.exchange());
    EXPECT_EQ(product_type::FUTURE, sym.product());
    EXPECT_EQ("ABCDEF", sym.name());
    EXPECT_EQ("CME/FUTURE/ABCDEF/2103", sym.str());

    EXPECT_EQ(sym, symbol { sym.val() });    // from value
    EXPECT_EQ(sym, symbol { sym.str() });    // from string

    // maturity overflow
    date max_maturity { symbol::max_maturity().time_since_epoch() + 24h };
    EXPECT_ANY_THROW(symbol(exchange_type::SSE, "AB", max_maturity));
    // maturity underflow
    date min_maturity(symbol::min_maturity().time_since_epoch() - 24h);
    EXPECT_ANY_THROW(symbol(exchange_type::SSE, "AB", min_maturity));

    // name overflow
    EXPECT_ANY_THROW(symbol(exchange_type::SSE, "ABCDEFG", date(2029, 1, 1)));
}

TEST(ut_symbol, general) {
    symbol equity { exchange_type::OSE, product_type::STOCK, "ABCDEFG" };
    EXPECT_EQ("OSE/STOCK/ABCDEFG", equity.str());
    EXPECT_EQ(equity, symbol { equity.val() });    // from value
    EXPECT_EQ(equity, symbol { equity.str() });    // from string

    symbol futcls { exchange_type::SZE, product_type::FUTURE, "ABCDEF" };
    EXPECT_EQ("SZE/FUTURE/ABCDEF", futcls.str());
    EXPECT_EQ(futcls, symbol { futcls.val() });    // from value
    EXPECT_EQ(futcls, symbol { futcls.str() });    // from string

    symbol optcls { exchange_type::NYMEX, product_type::PUT, "XYZ" };
    EXPECT_EQ("NYMEX/PUT/XYZ", optcls.str());
    EXPECT_EQ(optcls, symbol { optcls.val() });    // from value
    EXPECT_EQ(optcls, symbol { optcls.str() });    // from string
}

TEST(ut_symbol, invalid_str) {
    EXPECT_FALSE(symbol { "SSE//ABC" });
    EXPECT_FALSE(symbol { "SSE/STOCK" });
    EXPECT_FALSE(symbol { "SSE/STOCK/" });

    EXPECT_FALSE(symbol { "UKN/FUTURE/ABC/2103" });
    EXPECT_FALSE(symbol { "CME/UKN/ABC/2103" });
    EXPECT_FALSE(symbol { "CME/FUTURE/1234567/2103" });
    EXPECT_FALSE(symbol { "CME/FUTURE/ABC/2x03" });
    EXPECT_FALSE(symbol { "CME/FUTURE/ABC/2100" });
    EXPECT_FALSE(symbol { "CME/FUTURE/ABC/02103" });
    EXPECT_FALSE(symbol { "CME/FUTURE/ABC/1701" });
    EXPECT_FALSE(symbol { "CME/FUTURE/ABC/3001" });

    EXPECT_FALSE(symbol { "NYSE/CALL/ABC/120000000/2103" });
    EXPECT_FALSE(symbol { "NYSE/CALL/ABCD/120000/2103" });
    EXPECT_FALSE(symbol { "NYSE/CALL/ABC/1.2" });
    EXPECT_FALSE(symbol { "NYSE/CALL/ABC//2103" });
}

TEST(ut_symbol, variant) {
    miu::com::reg_var_str<symbol>();

    symbol sym { exchange_type::COMEX, "future", date(2022, 1, 2) };
    EXPECT_EQ(sym, variant { sym }.get<symbol>());
    EXPECT_EQ(sym, variant { sym.val() }.get<symbol>());

    EXPECT_EQ(sym.str(), variant { sym }.get<std::string>().value());
}


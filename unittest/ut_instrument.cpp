#include <gtest/gtest.h>

#include "ref/instrument.hpp"
#include "source/lib/layout.hpp"

using namespace miu::ref;

TEST(ut_instrument, default) {
    auto inst = instrument {};
    EXPECT_FALSE(inst);
    EXPECT_TRUE(!inst);
}

TEST(ut_instrument, property) {
    underlying_impl underlying { "COMEX/FUTURE/abc" };
    underlying.set_currency(currency_type::CNH);
    underlying.set_lot_size(100);
    underlying.set_multiplier(200);

    uint16_t instrument_id = 1;
    char buf[4096] {};
    auto layout = layout::make(buf, "ut_instrument", 8, 4, 4);
    auto impl   = layout->instruments() + instrument_id;
    new (impl) instrument_impl { underlying, miu::time::date(2021, 3, 6) };
    impl->set_mkt_code("123");
    impl->set_trd_code("321");

    auto inst = instrument { layout, instrument_id };
    EXPECT_EQ(instrument_id, inst.id());
    EXPECT_EQ(impl->symbol(), inst.symbol());
    EXPECT_EQ(impl->exchange(), inst.exchange());
    EXPECT_EQ(impl->type(), inst.type());
    EXPECT_EQ(impl->name(), inst.name());
    EXPECT_EQ(impl->currency(), inst.currency());
    EXPECT_EQ(impl->lot_size(), inst.lot_size());
    EXPECT_EQ(impl->multiplier(), inst.multiplier());
    EXPECT_EQ(impl->maturity(), inst.maturity());
    EXPECT_EQ(impl->strike_price(), inst.strike_price());
    EXPECT_EQ(impl->mkt_code().value, inst.mkt_code());
    EXPECT_EQ(impl->trd_code().value, inst.trd_code());
}

TEST(ut_instrument, tiktable) {
    underlying_impl underlying { "COMEX/FUTURE/abc" };

    uint16_t tiktable_id = 2;
    underlying.set_tiktable_id(tiktable_id);

    uint16_t instrument_id = 2;
    char buf[4096] {};
    auto layout = layout::make(buf, "ut_instrument", 8, 4, 4);

    auto tiktable = new (layout->tiktables() + tiktable_id) tiktable_impl;
    tiktable->add(0.0, 0.2);

    auto impl = layout->instruments() + instrument_id;
    new (impl) instrument_impl { underlying, miu::time::date(2021, 3, 6) };

    auto inst = instrument { layout, instrument_id };
    EXPECT_EQ(10.2, inst.tick_up(10));
    EXPECT_EQ(10.2, inst.tick_up(10.1, 0));
    EXPECT_EQ(9.8, inst.tick_dn(10));
    EXPECT_EQ(9.8, inst.tick_dn(9.9, 0));
}

TEST(ut_instrument, schedule) {
    underlying_impl underlying { "COMEX/FUTURE/abc" };

    uint16_t schedule_id = 2;
    underlying.set_schedule_id(schedule_id);

    uint16_t instrument_id = 2;
    char buf[4096] {};
    auto layout = layout::make(buf, "ut_instrument", 8, 4, 4);

    auto schedule = new (layout->schedules() + schedule_id) schedule_impl;
    schedule->add({ "00:00:00.000" }, phase_type::OPEN);
    schedule->add({ "22:00:00.000" }, phase_type::CLOSE);

    auto impl = layout->instruments() + instrument_id;
    new (impl) instrument_impl { underlying, miu::time::date(2021, 3, 6) };

    auto inst = instrument { layout, instrument_id };
    EXPECT_EQ(phase_type::OPEN, inst.determine({ "00:30:00.000" }));
    EXPECT_EQ(phase_type::CLOSE, inst.determine({ "22:30:00.000" }));
}

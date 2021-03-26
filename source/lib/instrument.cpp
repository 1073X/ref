
#include "ref/instrument.hpp"

#include "layout.hpp"

namespace miu::ref {

instrument::instrument(layout const* layout, uint16_t id)
    : _layout(layout)
    , _id(id) {
}

symbol instrument::symbol() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].symbol();
}

exchange_type instrument::exchange() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].exchange();
}

product_type instrument::type() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].type();
}

std::string instrument::name() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].name();
}

currency_type instrument::currency() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].currency();
}

int32_t instrument::lot_size() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].lot_size();
}

int32_t instrument::multiplier() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].multiplier();
}

time::date instrument::maturity() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].maturity();
}

price instrument::strike_price() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].strike_price();
}

std::string_view instrument::mkt_code() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].mkt_code().value;
}

std::string_view instrument::trd_code() const {
    assert(_layout != nullptr && "null instrument");
    return _layout->instruments()[_id].trd_code().value;
}

price instrument::tick_up(price prc, int32_t n) const {
    assert(_layout != nullptr && "null instrument");
    auto tiktable_id = _layout->instruments()[_id].tiktable_id();
    return _layout->tiktables()[tiktable_id].up(prc, n);
}

price instrument::tick_dn(price prc, int32_t n) const {
    assert(_layout != nullptr && "null instrument");
    auto tiktable_id = _layout->instruments()[_id].tiktable_id();
    return _layout->tiktables()[tiktable_id].dn(prc, n);
}

phase_type instrument::determine(time::daytime time) const {
    assert(_layout != nullptr && "null instrument");
    auto schedule_id = _layout->instruments()[_id].schedule_id();
    return _layout->schedules()[schedule_id].determine(time);
}

}    // namespace miu::ref

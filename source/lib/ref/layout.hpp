#pragma once

#include "source/lib/map/robin_hood_map.hpp"

#include "instrument_impl.hpp"
#include "schedule_impl.hpp"
#include "tiktable_impl.hpp"
#include "underlying_impl.hpp"

namespace miu::ref {

class layout {
  private:
    using symbol_map_type = robin_hood_map<symbol>;
    using code_map_type   = robin_hood_map<referential::code>;

  public:
    static uint32_t resolve_size(uint32_t max_of_instrument,
                                 uint32_t max_of_tiktable,
                                 uint32_t max_of_schedule) {
        return sizeof(layout)
             //
             + max_of_schedule * sizeof(schedule_impl)
             //
             + max_of_instrument * sizeof(instrument_impl)
             //
             + max_of_tiktable * sizeof(tiktable_impl)
             //
             + symbol_map_type::resolve_size(max_of_instrument * 2)
             // mkt code map
             + code_map_type::resolve_size(max_of_instrument * 2)
             // trd code map
             + code_map_type::resolve_size(max_of_instrument * 2);
    }

    static layout* make(void* buf,
                        std::string_view name,
                        uint32_t max_of_instrument,
                        uint32_t max_of_tiktable,
                        uint32_t max_of_schedule) {
        // TBD: assert buf size > resolve_size
        auto layout = new (buf) class layout;
        std::strncpy(layout->_name, name.data(), sizeof(layout->_name));
        layout->_max_of_schedule   = max_of_schedule;
        layout->_max_of_instrument = max_of_instrument;
        layout->_max_of_tiktable   = max_of_tiktable;

        auto symbol_map_size = symbol_map_type::resolve_size(max_of_instrument * 2);
        symbol_map_type::make(layout->symbol_map(), symbol_map_size);

        auto code_map_size = code_map_type::resolve_size(max_of_instrument * 2);
        code_map_type::make(layout->mkt_map(), code_map_size);
        code_map_type::make(layout->trd_map(), code_map_size);

        return layout;
    }

    static layout* open(char* buf) { return (class layout*)buf; }
    static layout const* open(const char* buf) { return (class layout const*)buf; }

  public:
    std::string_view name() const { return _name; }

    auto max_of_schedule() const { return _max_of_schedule; }
    auto schedules() const { return (schedule_impl*)(this + 1); }
    auto num_of_schedule() const { return _num_of_schedule; }

    auto max_of_instrument() const { return _max_of_instrument; }
    auto instruments() const { return (instrument_impl*)(schedules() + max_of_schedule()); }
    auto num_of_instrument() const { return _num_of_instrument; }

    auto max_of_tiktable() const { return _max_of_tiktable; }
    auto tiktables() const { return (tiktable_impl*)(instruments() + max_of_instrument()); }
    auto num_of_tiktable() const { return _num_of_tiktable; }

    symbol_map_type* symbol_map() { return (symbol_map_type*)(tiktables() + max_of_tiktable()); }
    auto symbol_map() const { return const_cast<layout*>(this)->symbol_map(); }

    code_map_type* mkt_map() {
        return (code_map_type*)(symbol_map() + symbol_map()->capacity() + 1);
    }
    auto mkt_map() const { return const_cast<layout*>(this)->mkt_map(); }

    code_map_type* trd_map() { return mkt_map() + mkt_map()->capacity() + 1; }
    auto trd_map() const { return const_cast<layout*>(this)->trd_map(); }

    auto restructure(uint16_t num_of_instrument,
                     uint16_t num_of_tiktable,
                     uint16_t num_of_schedule) {
        _num_of_schedule   = num_of_schedule;
        _num_of_tiktable   = num_of_tiktable;
        _num_of_instrument = num_of_instrument;

        for (auto i = 0; i < num_of_instrument; i++) {
            auto inst = instruments() + i;
            symbol_map()->insert(inst->symbol(), i);
            mkt_map()->insert(inst->mkt_code(), i);
            trd_map()->insert(inst->trd_code(), i);
        }
    }

  private:
    layout() = default;

    auto paddings() const { return _paddings; }

  private:
    char _name[16];

    uint32_t _num_of_schedule;
    uint32_t _num_of_tiktable;
    uint32_t _num_of_instrument;

    uint32_t _max_of_schedule;
    uint32_t _max_of_instrument;
    uint32_t _max_of_tiktable;

    uint32_t _paddings[6];
};
static_assert(CACHE_LINE == sizeof(layout));

}    // namespace miu::ref

#pragma once

#include "instrument_impl.hpp"
#include "robin_hood_map.hpp"
#include "schedule_impl.hpp"
#include "tiktable_impl.hpp"
#include "underlying_impl.hpp"

namespace miu::ref {

class layout {
  private:
    using symbol_map_type = robin_hood_map<symbol>;
    using code_map_type   = robin_hood_map<referential::code>;

  public:
    static uint32_t resolve_size(uint32_t instrument_capacity,
                                 uint32_t tiktable_capacity,
                                 uint32_t schedule_capacity) {
        return sizeof(layout) + instrument_capacity * sizeof(instrument_impl)
             + tiktable_capacity * sizeof(tiktable_impl) + schedule_capacity * sizeof(schedule_impl)
             + symbol_map_type::resolve_size(instrument_capacity * 2)
             + code_map_type::resolve_size(instrument_capacity * 2);
    }

    static layout* make(void* buf,
                        std::string_view name,
                        uint32_t instrument_capacity,
                        uint32_t tiktable_capacity,
                        uint32_t schedule_capacity) {
        // TBD: assert buf size > resolve_size
        auto layout = new (buf) class layout;
        std::strncpy(layout->_name, name.data(), sizeof(layout->_name));
        layout->_schedule_capacity   = schedule_capacity;
        layout->_instrument_capacity = instrument_capacity;
        layout->_tiktable_capacity   = tiktable_capacity;

        auto symbol_map_size = symbol_map_type::resolve_size(instrument_capacity * 2);
        symbol_map_type::make(layout->symbol_map(), symbol_map_size);

        auto codetable_size = code_map_type::resolve_size(instrument_capacity * 2);
        code_map_type::make(layout->mkt_map(), codetable_size);
        code_map_type::make(layout->trd_map(), codetable_size);

        return layout;
    }

    static layout* open(char* buf) { return (class layout*)buf; }
    static layout const* open(const char* buf) { return (class layout const*)buf; }

  public:
    std::string_view name() const { return _name; }

    auto schedule_capacity() const { return _schedule_capacity; }
    auto schedules() const { return (schedule_impl*)(this + 1); }
    auto schedule_count() const { return _schedule_count; }

    auto instrument_capacity() const { return _instrument_capacity; }
    auto instruments() const { return (instrument_impl*)(schedules() + schedule_capacity()); }
    auto instrument_count() const { return _instrument_count; }

    auto tiktable_capacity() const { return _tiktable_capacity; }
    auto tiktables() const { return (tiktable_impl*)(instruments() + instrument_capacity()); }
    auto tiktable_count() const { return _tiktable_count; }

    symbol_map_type* symbol_map() { return (symbol_map_type*)(tiktables() + tiktable_capacity()); }
    auto symbol_map() const { return const_cast<layout*>(this)->symbol_map(); }

    code_map_type* mkt_map() {
        return (code_map_type*)(symbol_map() + symbol_map()->capacity() + 1);
    }
    auto mkt_map() const { return const_cast<layout*>(this)->mkt_map(); }

    code_map_type* trd_map() { return mkt_map() + mkt_map()->capacity() + 1; }
    auto trd_map() const { return const_cast<layout*>(this)->trd_map(); }

    auto structure() const { return _structure; }
    auto restructure(uint16_t instrument_count, uint16_t tiktable_count, uint16_t schedule_count) {
        _schedule_count   = schedule_count;
        _tiktable_count   = tiktable_count;
        _instrument_count = instrument_count;
        _padding          = 0;

        for (auto i = 0; i < _instrument_count; i++) {
            auto inst = instruments() + i;
            symbol_map()->insert(inst->symbol(), i);
            mkt_map()->insert(inst->mkt_code(), i);
            trd_map()->insert(inst->trd_code(), i);
        }
    }

  private:
    layout() = default;

  private:
    char _name[16];

    union {
        uint64_t _structure;
        struct {
            uint64_t _schedule_count   : 16;
            uint64_t _tiktable_count   : 16;
            uint64_t _instrument_count : 16;
            uint64_t _padding          : 16;
        };
    };

    uint32_t _schedule_capacity;
    uint32_t _instrument_capacity;
    uint32_t _tiktable_capacity;

    uint32_t _paddings[7];
};
static_assert(CACHE_LINE == sizeof(layout));

}    // namespace miu::ref

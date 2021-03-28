#pragma once

#include <log/log.hpp>

#include "source/lib/ref/layout.hpp"

#include "json_reader.hpp"

namespace miu::ref {

class json_source {
  public:
    template<typename T>
    using id_map = std::map<T, uint16_t>;

  public:
    json_source(com::json const& json)
        : _json(json) {}

    auto tiktable_count() const { return _json["tiktables"].size(); }
    auto schedule_count() const { return _json["schedules"].size(); }
    auto underlying_count() const { return _json["underlyings"].size(); }
    auto instrument_count() const {
        uint32_t count = 0;
        for (auto const& [key, val] : _json["instruments"].items()) {
            count += val.size();
        }
        return count;
    }

    /// tiktable
    static void create(tiktable_impl* tiktable,
                       json_reader const& reader,
                       std::string_view name) try {
        log::debug(+"ref TIKTABLE +++", name);

        if (reader.empty()) {
            throw std::invalid_argument("empty tiktable config");
        }

        for (auto const& [key, val] : reader.items()) {
            auto lev = price { std::stod(key) };
            auto tik = reader.get_double(key);
            log::debug(+"ref TIKTABLE", lev, '-', tik);
            tiktable->add(lev, tik);
        }
    } catch (std::exception const& err) {
        log::error(+"ref TIKTABLE", name, err.what());
        throw;
    }

    auto fill(tiktable_impl* buf, uint32_t len) const {
        return fill<tiktable_impl>(buf, len, "tiktables");
    }

    /// schedule
    static void create(schedule_impl* schedule,
                       json_reader const& reader,
                       std::string_view name) try {
        log::debug(+"ref SCHEDULE +++", name);

        if (reader.empty()) {
            throw std::invalid_argument("empty schedule config");
        }

        for (auto const& [key, val] : reader.items()) {
            auto time  = time::daytime { key };
            auto phase = reader.get_string(key);
            schedule->add(time, com::val_to_enum<phase_type>(phase));
            log::debug(+"ref SCHEDULE", time, '-', phase);
        }
    } catch (std::exception const& err) {
        log::error(+"ref SCHEDULE", name, err.what());
        throw;
    }

    auto fill(schedule_impl* buf, uint32_t len) const {
        return fill<schedule_impl>(buf, len, "schedules");
    }

    /// underlying
    static auto create(underlying_impl* underlying,
                       json_reader const& reader,
                       id_map<std::string> const& tiktable_ids,
                       id_map<std::string> const& schedule_ids) try {
        auto currency   = reader.get_string("currency");
        auto lot_size   = reader.get_number("lot_size");
        auto multiplier = reader.get_number("multiplier");
        auto tiktable   = reader.get_string("tiktable");
        auto schedule   = reader.get_string("schedule");

        underlying->set_currency(com::val_to_enum<currency_type>(currency));
        underlying->set_lot_size(lot_size);
        underlying->set_multiplier(multiplier);
        underlying->set_tiktable_id(tiktable_ids.at(tiktable));
        underlying->set_schedule_id(schedule_ids.at(schedule));

        log::debug(+"ref UNDERLYING",
                   underlying->symbol(),
                   underlying->currency(),
                   underlying->lot_size(),
                   underlying->multiplier(),
                   underlying->tiktable_id(),
                   tiktable,
                   underlying->schedule_id(),
                   schedule);

    } catch (std::exception const& err) {
        log::error(+"ref UNDERLYING", underlying->symbol(), err.what());
        throw;
    }

    auto fill(underlying_impl* buf,
              uint32_t /*len*/,
              id_map<std::string> const& tiktable_ids,
              id_map<std::string> const& schedule_ids) const {
        id_map<symbol> ids;
        for (auto const& [key, val] : _json["underlyings"].items()) {
            assert(val.is_object());    // TBD: verify structure

            auto underlying = new (buf + ids.size()) underlying_impl { key };
            create(underlying, val, tiktable_ids, schedule_ids);
            ids.emplace(underlying->symbol(), ids.size());
        }
        return ids;
    }

    /// instrument/layout

    static auto create(instrument_impl* instrument,
                       json_reader const& reader,
                       underlying_impl const& underlying) try {
        switch (underlying.type()) {
        case product_type::CALL:
        case product_type::PUT: {
            auto strike   = reader.get_double("strike_price");
            auto maturity = reader.get_string("maturity");
            new (instrument) instrument_impl { underlying, strike, time::date(maturity) };
        } break;
        case product_type::FUTURE: {
            auto maturity = reader.get_string("maturity");
            new (instrument) instrument_impl { underlying, time::date(maturity) };
        } break;
        default: {
            auto name = reader.get_string("name");
            new (instrument) instrument_impl { underlying, name };
        } break;
        }

        auto mkt_code = reader.get_string("mkt_code", "");
        auto trd_code = reader.get_string("trd_code", "");

        if (mkt_code.empty()) {
            mkt_code = trd_code;
        }
        if (trd_code.empty()) {
            trd_code = mkt_code;
        }

        instrument->set_mkt_code(mkt_code);
        instrument->set_trd_code(trd_code);

    } catch (std::exception const& err) {
        log::error(+"ref INSTRUMENT", err.what());
        throw;
    }

    auto fill(class layout* layout) const {
        // TBD: verify layout capacity
        assert(layout->tiktable_capacity() >= tiktable_count() && "tiktable overflow");
        assert(layout->schedule_capacity() >= schedule_count() && "schedule overflow");
        assert(layout->instrument_capacity() >= instrument_count() && "instrument overflow");

        auto tiktable_ids = fill(layout->tiktables(), layout->tiktable_capacity());
        auto schedule_ids = fill(layout->schedules(), layout->schedule_capacity());
        std::unique_ptr<underlying_impl[]> underlyings { new underlying_impl[underlying_count()] };
        auto underlying_ids = fill(
            underlyings.get(), underlying_count(), tiktable_ids, schedule_ids);

        id_map<symbol> instrument_ids;
        for (auto const& [key, val] : _json["instruments"].items()) {
            assert(val.is_array() && "illegal instruments structure");    // TBD: verify structure

            auto uid = underlying_ids.at({ key });    // throw if underlying not found
            for (auto const& inst : val) {
                auto instrument = layout->instruments() + instrument_ids.size();
                create(instrument, inst, underlyings[uid]);

                auto id = instrument_ids.size();
                log::debug(+"ref INSTRUMENT",
                           id,
                           instrument->symbol(),
                           std::string(instrument->mkt_code().value),
                           std::string(instrument->trd_code().value),
                           instrument->maturity(),
                           instrument->strike_price());

                instrument_ids.emplace(instrument->symbol(), id);
            }
        }

        layout->restructure(instrument_count(), tiktable_count(), schedule_count());
        return instrument_ids;
    }

  private:
    template<typename T>
    id_map<std::string> fill(T* buf, uint32_t /*len*/, std::string const& name) const {
        id_map<std::string> ids;
        for (auto const& [key, val] : _json[name].items()) {
            assert(val.is_object());    // TBD: verify structure

            auto item = new (buf + ids.size()) T {};
            create(item, val, key);
            ids.emplace(key, ids.size());
        }
        return ids;
    }

  private:
    com::json const& _json;
};

}    // namespace miu::ref

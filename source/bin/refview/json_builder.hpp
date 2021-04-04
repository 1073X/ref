#pragma once

#include <com/json.hpp>
#include <cstdio>    // std::snprintf

#include "source/lib/ref/instrument_impl.hpp"
#include "source/lib/ref/schedule_impl.hpp"
#include "source/lib/ref/tiktable_impl.hpp"

namespace miu::ref {

class json_builder {
  public:
    auto json() const { return _json; }

    auto append(schedule_impl const* schedule) {
        com::json data;

        auto set = [&](uint32_t i) {
            time::daytime time { std::chrono::minutes(schedule->keys()[i]) };
            data[com::to_string(time)] = com::to_string(schedule->vals()[i]);
        };

        // skip default close at 00:00:00.000
        if (phase_type::CLOSE != schedule->vals()[0]) {
            set(0);
        }

        for (auto i = 1U; i < schedule->segements(); i++) {
            set(i);
        }

        auto& table = _json["schedules"];
        auto name   = make_name("TH", table.size());
        table[name] = data;
    }

    auto append(tiktable_impl const* tiktable) {
        com::json data;
        for (auto i = 0U; i < tiktable->segements(); i++) {
            auto base = tiktable->keys()[i];
            auto tick = tiktable->vals()[i].value();

            data[com::to_string(base)] = tick;
        }

        auto& table = _json["tiktables"];
        auto name   = make_name("TK", table.size());
        table[name] = data;
    }

    auto append(instrument_impl const* inst) {
        auto underlying_symbol = append(inst->underlying());

        com::json data;
        data["mkt_code"] = inst->mkt_code().value;
        data["trd_code"] = inst->trd_code().value;

        if (is_derivatives(inst->type())) {
            data["maturity"] = com::to_string(inst->maturity());
            if (is_options(inst->type())) {
                data["strike_price"] = static_cast<double>(inst->strike_price());
            }
        } else {
            data["name"] = inst->name();
        }

        _json["instruments"][com::to_string(underlying_symbol)].push_back(data);
    }

  private:
    std::string make_name(const char* prefix, uint32_t id) const {
        char name[8] {};
        std::snprintf(name, sizeof(name), "%s%02u", prefix, id);
        return name;
    }

    symbol append(underlying_impl const& underlying) {
        for (auto const& prod : _products) {
            if (underlying.exchange() == prod.exchange() && underlying.type() == prod.type()
                && underlying.currency() == prod.currency()
                && underlying.lot_size() == prod.lot_size()
                && underlying.multiplier() == prod.multiplier()
                && underlying.tiktable_id() == prod.tiktable_id()
                && underlying.schedule_id() == prod.schedule_id()) {
                if (is_derivatives(underlying.type()) && underlying.name() != prod.name()) {
                    continue;
                }
                return prod.symbol();
            }
        }
        _products.push_back(underlying);

        com::json data;
        data["currency"]   = com::to_string(underlying.currency());
        data["lot_size"]   = underlying.lot_size();
        data["multiplier"] = underlying.multiplier();
        data["tiktable"]   = make_name("TK", underlying.tiktable_id());
        data["schedule"]   = make_name("TH", underlying.schedule_id());

        _json["underlyings"][com::to_string(underlying.symbol())] = data;

        return underlying.symbol();
    }

  private:
    com::json _json { { "instruments", com::json() } };
    std::vector<underlying_impl> _products;
};

}    // namespace miu::ref

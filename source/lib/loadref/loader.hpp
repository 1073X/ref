#pragma once

#include <com/json.hpp>
#include <filesystem>
#include <fstream>
#include <shm/buffer.hpp>

#include "ref/database.hpp"

#include "json_source.hpp"

namespace miu::ref {

class loader {
  public:
    static database load(std::filesystem::path const& file, std::string name = "") {
        auto json   = com::json::parse(std::ifstream(file));
        auto source = json_source { json };

        auto instrument_cap = source.num_of_instrument() + 4;
        auto tiktable_cap   = source.num_of_tiktable() + 4;
        auto schedule_cap   = source.num_of_schedule() + 4;
        auto buffer_size    = layout::resolve_size(instrument_cap, tiktable_cap, schedule_cap);

        if (name.empty()) {
            name = file.stem();
        }
        auto buf = shm::buffer { { name, "ref" }, buffer_size };

        auto layout = layout::make(buf.data(), name, instrument_cap, tiktable_cap, schedule_cap);
        source.fill(layout);

        return database { std::move(buf) };
    }
};

}    // namespace miu::ref

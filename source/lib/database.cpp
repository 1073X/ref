
#include "ref/database.hpp"

#include <fstream>

#include "json_source.hpp"
#include "layout.hpp"

namespace miu::ref {

database database::load(std::filesystem::path const& file) {
    auto json   = com::json::parse(std::ifstream(file));
    auto source = json_source { json };

    auto instrument_cap = source.instrument_count() + 4;
    auto tiktable_cap   = source.tiktable_count() + 4;
    auto schedule_cap   = source.schedule_count() + 4;
    auto buffer_size    = layout::resolve_size(instrument_cap, tiktable_cap, schedule_cap);

    std::string name = file.stem();
    auto buf         = shm::buffer { { name, "ref" }, buffer_size };

    auto layout = layout::make(buf.data(), name, instrument_cap, tiktable_cap, schedule_cap);
    source.fill(layout);

    return { std::move(buf) };
}

database database::open(std::string_view name, shm::mode mode) {
    auto buf = shm::buffer { { name, "ref" }, mode };
    // TBD: verify database
    return { std::move(buf) };
}

database::database(shm::buffer&& buf)
    : _buf(std::move(buf)) {
}

std::string_view database::name() const {
    return layout::open(_buf.data())->name();
}

uint32_t database::size() const {
    return layout::open(_buf.data())->instrument_count();
}

instrument database::find(uint16_t id) const {
    if (LIKELY(id < size())) {
        return { layout::open(_buf.data()), id };
    }
    return {};
}

instrument database::find(symbol sym) const {
    return find(layout::open(_buf.data())->symbol_map()->lookup(sym));
}

instrument database::find_by_mkt_code(std::string_view code) const {
    return find(layout::open(_buf.data())->mkt_map()->lookup(code));
}

instrument database::find_by_trd_code(std::string_view code) const {
    return find(layout::open(_buf.data())->trd_map()->lookup(code));
}

}    // namespace miu::ref

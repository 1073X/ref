
#include "ref/database.hpp"

#include "source/lib/md5/md5.hpp"

#include "layout.hpp"

namespace miu::ref {

database database::open(std::string_view name, shm::mode mode) {
    auto buf = shm::buffer { { name, "ref" }, mode };
    // TBD: verify database
    return database { std::move(buf) };
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

instrument database::find(symbol const& sym) const {
    return find(layout::open(_buf.data())->symbol_map()->lookup(sym));
}

instrument database::find_by_mkt_code(std::string_view code) const {
    return find(layout::open(_buf.data())->mkt_map()->lookup(code));
}

instrument database::find_by_trd_code(std::string_view code) const {
    return find(layout::open(_buf.data())->trd_map()->lookup(code));
}

signature database::signature() const {
    md5 decoder;
    for (auto i = 0U; i < size(); i++) {
        auto str = com::to_string(find(i).symbol());
        decoder.update(str.data(), str.size());
    }
    decoder.finalize();

    return { decoder.digest() };
}

}    // namespace miu::ref

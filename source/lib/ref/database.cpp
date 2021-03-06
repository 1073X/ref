
#include "ref/database.hpp"

#include "source/lib/md5/md5.hpp"

#include "layout.hpp"

namespace miu::ref {

database database::open(std::string_view name, shm::mode mode) {
    auto buf = shm::buffer { { name, "ref" }, mode };
    // TBD: verify database
    return database { buf };
}

database::database(shm::buffer const& buf)
    : _buf(buf) {
}

std::string_view database::name() const {
    return layout::open(_buf.data())->name();
}

uint32_t database::num_of_instrument() const {
    return layout::open(_buf.data())->num_of_instrument();
}

uint32_t database::max_of_instrument() const {
    return layout::open(_buf.data())->max_of_instrument();
}

instrument database::find(uint16_t id) const {
    if (LIKELY(id < num_of_instrument())) {
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
    for (auto i = 0U; i < num_of_instrument(); i++) {
        auto str = com::to_string(find(i).symbol());
        decoder.update(str.data(), str.size());
    }
    decoder.finalize();

    return { decoder.digest() };
}

}    // namespace miu::ref

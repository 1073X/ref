
#include "stub/ref.hpp"

#include <iomanip>
#include <shm/tempfs.hpp>
#include <sstream>
#include <time/stamp.hpp>

#include "source/lib/loadref/loader.hpp"

namespace miu::ref {

namespace details {
    static auto make_marker() {
        auto v = time::clock::now().time_since_epoch().count();
        std::ostringstream ss;
        ss << std::hex << v;
        return ss.str();
    }

    extern std::string build_json();

}    // namespace details

stub::stub()
    : _marker { details::make_marker() }
    , _db { nullptr } {
}

stub::~stub() {
    delete _db;
    shm::tempfs::remove(marker(), "ref");
}

database* stub::db() {
    if (!_db) {
        auto file = details::build_json();
        _db       = new database { loader::load(file, marker()) };
    }
    return _db;
}

}    // namespace miu::ref

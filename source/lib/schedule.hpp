#pragma once

#include <com/enum.hpp>
#include <time/daytime.hpp>
#include <time/stamp.hpp>

#include "ref/status.hpp"

#include "segmap.hpp"

namespace miu::ref {

using schedule_base = segmap<uint16_t, status, 16>;
class schedule : public schedule_base {
  public:
    static uint16_t const MIN_KEY = 0;          // 00:00:00
    static uint16_t const MAX_KEY = 24 * 60;    // 24:00:00

  public:
    schedule()
        : schedule_base(MIN_KEY, MAX_KEY, status::CLOSE) {}

    auto operator==(schedule const& lhs) const { return schedule_base::operator==(lhs); }

    auto add(time::daytime time, status status) {
        using namespace std::chrono;

        auto sec = duration_cast<minutes>(time.time_since_epoch());
        schedule_base::add(sec.count(), status);
    }

    auto determine(time::daytime time) const {
        using namespace std::chrono;

        auto sec = duration_cast<minutes>(time.time_since_epoch());
        return schedule_base::get(sec.count());
    }
    auto determine(time::stamp time) const { return determine(time.time()); }
};
static_assert(CACHE_LINE == sizeof(schedule));

}    // namespace miu::ref

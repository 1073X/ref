#pragma once

#include <com/enum.hpp>
#include <time/daytime.hpp>
#include <time/stamp.hpp>

#include "ref/phase_type.hpp"

#include "subsection.hpp"

namespace miu::ref {

namespace details {
    using schedule_impl = subsection<uint16_t, phase_type, 16>;
}

class schedule_impl : public details::schedule_impl {
  public:
    static uint16_t const MIN_KEY = 0;          // 00:00:00
    static uint16_t const MAX_KEY = 24 * 60;    // 24:00:00

  public:
    schedule_impl()
        : details::schedule_impl(MIN_KEY, MAX_KEY, phase_type::CLOSE) {}

    auto operator==(schedule_impl const& lhs) const {
        return details::schedule_impl::operator==(lhs);
    }

    auto add(time::daytime time, phase_type phase) {
        using namespace std::chrono;

        if (phase >= phase_type::MAX) {
            throw std::invalid_argument("invalid phase_type");
        }

        auto sec = duration_cast<minutes>(time.time_since_epoch());
        details::schedule_impl::add(sec.count(), phase);
    }

    auto determine(time::daytime time) const {
        using namespace std::chrono;

        auto sec = duration_cast<minutes>(time.time_since_epoch());
        return details::schedule_impl::get(sec.count());
    }
    auto determine(time::stamp time) const { return determine(time.time()); }
};
static_assert(CACHE_LINE == sizeof(schedule_impl));

}    // namespace miu::ref

#pragma once

#include <filesystem>
#include <shm/buffer.hpp>

#include "instrument.hpp"
#include "signature.hpp"

namespace miu::ref {

class database {
  public:
    static database open(std::string_view name, shm::mode);

  public:
    database() = default;
    explicit database(shm::buffer&&);

    auto operator!() const { return !_buf; }
    operator bool() const { return !operator!(); }

    std::string_view name() const;
    uint32_t size() const;

    // find by id
    instrument find(uint16_t id) const;

    // find by symbol
    instrument find(symbol const&) const;
    // find by symbol string
    auto find(std::string_view str) const { return find(symbol { str }); }

    instrument find_by_mkt_code(std::string_view) const;
    instrument find_by_trd_code(std::string_view) const;

    class signature signature() const;

  private:
    shm::buffer _buf;
};

}    // namespace miu::ref

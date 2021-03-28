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
    explicit database(shm::buffer&&);

    std::string_view name() const;
    uint32_t size() const;

    instrument find(uint16_t id) const;
    instrument find(symbol) const;
    instrument find_by_mkt_code(std::string_view) const;
    instrument find_by_trd_code(std::string_view) const;

    class signature signature() const;

  private:
    shm::buffer _buf;
};

}    // namespace miu::ref

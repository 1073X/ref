#pragma once

#include "ref/database.hpp"

namespace miu::ref {

class stub {
  public:
    stub();
    ~stub();

    auto marker() const { return _marker; }
    auto name() const { return "stub_" + _marker; }

    database* db();

  private:
    std::string _marker;
    database* _db;
};

}    // namespace miu::ref

#pragma once

#include "symbol.hpp"

namespace miu::ref {

class referential {
  public:
    template<typename... ARGS>
    referential(ARGS&&... args)
        : _symbol { std::forward<ARGS>(args)... } {}

    auto symbol() const { return _symbol; }
    auto name() const { return _symbol.name(); }
    auto exchange() const { return _symbol.exchange(); }
    auto product() const { return _symbol.product(); }

    std::string_view trd_code() const { return &_trd_code[0]; }
    void set_trd_code(std::string_view v) {
        assert(v.size() < sizeof(_trd_code) && "trd_code overflow");
        std::strncpy(_trd_code, v.data(), sizeof(_trd_code));
    }

    std::string_view mkt_code() const { return &_mkt_code[0]; }
    void set_mkt_code(std::string_view v) {
        assert(v.size() < sizeof(_mkt_code) && "mkt_code overflow");
        std::strncpy(_mkt_code, v.data(), sizeof(_mkt_code));
    }

  private:
    char _trd_code[16] {};
    char _mkt_code[16] {};
    class symbol _symbol;
};

}    // namespace miu::ref

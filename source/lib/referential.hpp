#pragma once

#include "ref/symbol.hpp"

namespace miu::ref {

class referential {
  public:
    struct code {
        char value[16] {};

        code() = default;
        code(std::string_view v) { operator=(v); }

        code& operator=(std::string_view v) {
            assert(v.size() < sizeof(value) && "referential::code overflow");
            std::strncpy(value, v.data(), sizeof(value));
            return *this;
        }
    };

  public:
    template<typename... ARGS>
    referential(ARGS&&... args)
        : _symbol { std::forward<ARGS>(args)... } {}

    auto symbol() const { return _symbol; }
    auto name() const { return _symbol.name(); }
    auto exchange() const { return _symbol.exchange(); }
    auto type() const { return _symbol.type(); }

    code const& trd_code() const { return _trd_code; }
    void set_trd_code(std::string_view v) { _trd_code = v; }

    code const& mkt_code() const { return _mkt_code; }
    void set_mkt_code(std::string_view v) { _mkt_code = v; }

  private:
    code _trd_code;
    code _mkt_code;
    class symbol _symbol;
};

}    // namespace miu::ref

#pragma once

#include <com/json.hpp>

namespace miu::ref {

class json_reader {
  public:
    json_reader(com::json const& json)
        : _json(json) {}

    auto empty() const { return _json.empty(); }
    auto items() const { return _json.items(); }

    auto get_string(std::string const& key) const {
        if (!_json.contains(key) || !_json[key].is_string()) {
            throw std::invalid_argument("ILL_STR_VAL [" + key + "]");
        }
        return _json[key].get<std::string>();
    }

    auto get_number(std::string const& key) const {
        if (!_json.contains(key) || !_json[key].is_number_integer()) {
            throw std::invalid_argument("ILL_NUM_VAL [" + key + "]");
        }
        return _json[key].get<int32_t>();
    }

    auto get_double(std::string const& key) const {
        if (!_json.contains(key) || !_json[key].is_number()) {
            throw std::invalid_argument("ILL_DEC_VAL [" + key + "]");
        }
        return _json[key].get<double>();
    }

  private:
    com::json const& _json;
};

}    // namespace miu::ref

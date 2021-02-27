
#include "ref/price.hpp"

namespace miu::com {

template<>
void miu::com::variant::set<ref::price>(ref::price const& v) {
    new (_value) ref::price { v };
}

template<>
std::optional<ref::price> miu::com::variant::get<ref::price>() const {
    auto opt = get<double>();
    return { opt.value() };
}

}    // namespace miu::com

DEF_TO_STRING(miu::ref::price) {
    return to_string(static_cast<double>(v));
}

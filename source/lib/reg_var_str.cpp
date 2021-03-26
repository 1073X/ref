
#include "ref/reg_var_str.hpp"

#include <com/reg_var_str.hpp>

#include "ref/currency_type.hpp"
#include "ref/exchange_type.hpp"
#include "ref/phase_type.hpp"
#include "ref/price.hpp"
#include "ref/product_type.hpp"
#include "ref/symbol.hpp"

namespace miu::ref {

void reg_var_str() {
    com::reg_var_str<price>();
    com::reg_var_str<phase_type>();
    com::reg_var_str<exchange_type>();
    com::reg_var_str<product_type>();
    com::reg_var_str<currency_type>();
    com::reg_var_str<symbol>();
}

}    // namespace miu::ref

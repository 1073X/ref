
#include <com/json.hpp>
#include <com/strcat.hpp>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <time/date.hpp>

namespace miu::ref::details {

static auto add_stocks(com::json& root) {
    miu::com::json prod;
    prod["currency"]                      = "CNH";
    prod["lot_size"]                      = 100;
    prod["multiplier"]                    = 1;
    prod["schedule"]                      = "TH01";
    prod["tiktable"]                      = "TK01";
    root["underlyings"]["STUB/STOCK/STK"] = prod;

    auto add = [&](uint32_t i) {
        char name[16] {};
        std::snprintf(name, sizeof(name), "STK%02d", i);

        miu::com::json inst;
        inst["name"]     = name;
        inst["mkt_code"] = com::strcat { "MKT", name }.str();
        inst["trd_code"] = com::strcat { "TRD", name }.str();
        root["instruments"]["STUB/STOCK/STK"].push_back(inst);
    };
    add(1);
    add(2);
}

static auto add_future(com::json& root) {
    miu::com::json prod;
    prod["currency"]                         = "USD";
    prod["lot_size"]                         = 1;
    prod["multiplier"]                       = 1;
    prod["schedule"]                         = "TH00";
    prod["tiktable"]                         = "TK00";
    root["underlyings"]["STUB/FUTURE/FUT01"] = prod;

    auto add = [&](time::date date) {
        auto [yrs, mon, day] = date.ytd();
        char name[16] {};
        std::snprintf(name, sizeof(name), "%02d%02d", yrs % 2000, mon);

        miu::com::json inst;
        inst["maturity"] = com::to_string(date);
        inst["mkt_code"] = com::strcat { "MKT", "FUT01", name }.str();
        inst["trd_code"] = com::strcat { "TRD", "FUT01", name }.str();
        root["instruments"]["STUB/FUTURE/FUT01"].push_back(inst);
    };

    add(time::date { 2021, 3, 28 });
    add(time::date { 2021, 4, 28 });
}

std::string build_json() {
    static auto constexpr filename = "stub_ref.json";

    com::json root;

    ///
    root["tiktables"]["TK00"]["0.0"] = 0.5;
    root["tiktables"]["TK01"]["0.0"] = 0.01;

    ///
    root["schedules"]["TH00"]["00:00:00.000"] = "OPEN";
    root["schedules"]["TH00"]["23:59:59.999"] = "CLOSE";

    root["schedules"]["TH01"]["08:00:00.000"] = "OPEN";
    root["schedules"]["TH01"]["21:00:00.000"] = "CLOSE";

    ///
    add_stocks(root);
    add_future(root);

    std::ofstream { filename } << std::setw(2) << root;
    return filename;
}

}    // namespace miu::ref::details

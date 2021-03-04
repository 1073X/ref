#pragma once

#include <com/variant.hpp>

namespace miu::ref {

enum struct exchange_type : uint16_t {
    UNDEF = 0,

    FOREX,    // LingLuGu forex
    SSE,
    SZE,
    CFFEX,
    SHFE,
    DCE,
    CZCE,
    INE,
    HKEX,
    HKFE,
    SGX,
    TSE,
    LME,
    LSE,
    CME,
    COMEX,
    NYSE,
    NYMEX,
    NASDAQ,
    ICE,
    CBOT,
    ASX,
    SFE,
    SGE,
    CBOE,
    EUREX,
    OSE,    // Osaka Exchange

    MAX
};

}

DEF_VARIANT(miu::ref::exchange_type, CUSTOM_TYPE_ID + 34);

#pragma once

#include <cstdint>

namespace miu::ref {

template<typename T>
struct robin_hood_hash {
    static std::size_t hash_bytes(void const* ptr, std::size_t len) noexcept {
        static constexpr uint64_t m     = UINT64_C(0xc6a4a7935bd1e995);
        static constexpr uint64_t seed  = UINT64_C(0xe17a1465);
        static constexpr unsigned int r = 47;

        auto const* const DATA64 = static_cast<uint64_t const*>(ptr);
        uint64_t h               = seed ^ (len * m);

        std::size_t const N_BLOCKS = len / 8;
        for (std::size_t i = 0; i < N_BLOCKS; ++i) {
            auto k = DATA64[i];

            k *= m;
            k ^= k >> r;
            k *= m;

            h ^= k;
            h *= m;
        }

        auto const* const DATA8 = reinterpret_cast<uint8_t const*>(DATA64 + N_BLOCKS);
        switch (len & 7U) {
        case 7:
            h ^= static_cast<uint64_t>(DATA8[6]) << 48U;
            [[fallthrough]];
        case 6:
            h ^= static_cast<uint64_t>(DATA8[5]) << 40U;
            [[fallthrough]];
        case 5:
            h ^= static_cast<uint64_t>(DATA8[4]) << 32U;
            [[fallthrough]];
        case 4:
            h ^= static_cast<uint64_t>(DATA8[3]) << 24U;
            [[fallthrough]];
        case 3:
            h ^= static_cast<uint64_t>(DATA8[2]) << 16U;
            [[fallthrough]];
        case 2:
            h ^= static_cast<uint64_t>(DATA8[1]) << 8U;
            [[fallthrough]];
        case 1:
            h ^= static_cast<uint64_t>(DATA8[0]);
            h *= m;
            [[fallthrough]];
        default:
            break;
        }

        h ^= h >> r;

        // not doing the final step here, because this will be done by keyToIdx anyways
        // h *= m;
        // h ^= h >> r;
        return static_cast<std::size_t>(h);
    }

    std::size_t operator()(T const& key) noexcept { return hash_bytes(&key, sizeof(T)); }
};

}    // namespace miu::ref

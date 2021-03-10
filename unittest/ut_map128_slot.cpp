#include <gtest/gtest.h>

#include "source/lib/map128.hpp"

struct hash {
    auto operator()(uint64_t const (&key)[2]) const { return key[0]; }
};

using map_type = miu::ref::map128<uint64_t[2], hash>;

TEST(ut_map128_slot, key) {
    uint64_t raw[2] { 1, 2 };
    auto key = map_type::key { raw };

    EXPECT_TRUE(key == raw);
    EXPECT_FALSE(key != raw);
}

TEST(ut_map128_slot, default) {
    auto slot = map_type::slot {};

    uint64_t key[2] { 0, 0 };
    EXPECT_EQ(slot.key(), key);

    EXPECT_EQ(0U, slot.val());

    EXPECT_FALSE(slot.used());
}

TEST(ut_map128_slot, create) {
    uint64_t key[2] { 1, 2 };
    auto slot = map_type::slot { key, 1 };

    EXPECT_EQ(1U, slot.val());
    EXPECT_EQ(slot.key(), key);
    EXPECT_TRUE(slot.used());
}

TEST(ut_map128_slot, psl) {
    auto slot = map_type::slot { { 1, 2 }, 1 };
    EXPECT_EQ(1, slot.psl());

    slot.inc_psl();
    EXPECT_EQ(2, slot.psl());
}

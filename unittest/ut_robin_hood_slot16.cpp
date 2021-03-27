#include <gtest/gtest.h>

#include "source/lib/map/robin_hood_slot.hpp"

using slot_type = miu::ref::robin_hood_slot<uint64_t[2], 16>;
static_assert(sizeof(slot_type) == 32);

TEST(ut_robin_hoold_slot16, key) {
    uint64_t raw[2] { 1, 2 };
    auto key = slot_type::key_type { raw };

    EXPECT_TRUE(key == raw);
    EXPECT_FALSE(key != raw);
}

TEST(ut_robin_hoold_slot16, default) {
    auto slot = slot_type {};

    uint64_t key[2] { 0, 0 };
    EXPECT_EQ(slot.key(), key);

    EXPECT_EQ(0U, slot.val());

    EXPECT_FALSE(slot.used());
}

TEST(ut_robin_hoold_slot16, create) {
    uint64_t key[2] { 1, 2 };
    auto slot = slot_type { key, 1 };

    EXPECT_EQ(1U, slot.val());
    EXPECT_EQ(slot.key(), key);
    EXPECT_TRUE(slot.used());
}

TEST(ut_robin_hoold_slot16, psl) {
    auto slot = slot_type { { 1, 2 }, 1 };
    EXPECT_EQ(1, slot.psl());

    slot.inc_psl();
    EXPECT_EQ(2, slot.psl());
}

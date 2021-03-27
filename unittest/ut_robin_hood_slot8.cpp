#include <gtest/gtest.h>

#include "source/lib/map/robin_hood_slot.hpp"

using slot_type = miu::ref::robin_hood_slot<uint64_t, sizeof(uint64_t)>;
static_assert(sizeof(slot_type) == 16);

TEST(ut_robin_hoold_slot8, key) {
    auto key = slot_type::key_type { 123 };
    EXPECT_TRUE(key == 123);
    EXPECT_FALSE(key != 123);
}

TEST(ut_robin_hoold_slot8, default) {
    auto slot = slot_type {};
    EXPECT_TRUE(slot.key() == 0);
    EXPECT_EQ(0U, slot.val());
    EXPECT_FALSE(slot.used());
}

TEST(ut_robin_hoold_slot8, create) {
    auto slot = slot_type { 123, 1 };
    EXPECT_EQ(1U, slot.val());
    EXPECT_TRUE(slot.key() == 123);
    EXPECT_TRUE(slot.used());
}

TEST(ut_robin_hoold_slot8, psl) {
    auto slot = slot_type { 123, 1 };
    EXPECT_EQ(1, slot.psl());

    slot.inc_psl();
    EXPECT_EQ(2, slot.psl());
}

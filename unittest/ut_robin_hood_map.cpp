#include <gtest/gtest.h>

#include "source/lib/robin_hood_map.hpp"

struct hash {
    auto operator()(uint64_t const (&key)[2]) const { return key[0]; }
};

using map_type = miu::ref::robin_hood_map<uint64_t[2], hash>;

TEST(ut_robin_hood_map, size) {
    EXPECT_EQ(sizeof(map_type::slot_type), sizeof(map_type));

    EXPECT_EQ(sizeof(map_type::slot_type) * 32, map_type::resolve_size(31));
}

TEST(ut_robin_hood_map, create) {
    char buf[32 * 9] {};
    auto map = map_type::make(buf, sizeof(buf));
    ASSERT_NE(nullptr, map);
    EXPECT_EQ(8U, map->capacity());
    EXPECT_EQ(0U, map->size());

    auto map2 = map_type::open(buf, sizeof(buf));
    ASSERT_NE(nullptr, map2);
    EXPECT_EQ(8U, map2->capacity());
    EXPECT_EQ(0U, map2->size());
}

TEST(ut_robin_hood_map, insert_and_lookup) {
    char buf[32 * 9] {};
    auto map = map_type::make(buf, sizeof(buf));

    uint64_t key1[] { 1, 0 };
    map->insert(key1, 1);
    uint64_t key2[] { 2, 0 };
    map->insert(key2, 2);

    EXPECT_EQ(2U, map->size());
    EXPECT_EQ(1U, map->max_psl());

    EXPECT_EQ(1U, map->lookup(key1));
    EXPECT_EQ(2U, map->lookup(key2));

    // not found
    uint64_t key3[] { 3, 0 };
    EXPECT_EQ(-1U, map->lookup(key3));
    uint64_t key4[] { 9, 0 };
    EXPECT_EQ(-1U, map->lookup(key4));
}

TEST(ut_robin_hood_map, collision) {
    char buf[32 * 9] {};
    auto map = map_type::make(buf, sizeof(buf));

    uint64_t key1[] { 1, 0 };
    map->insert(key1, 1);
    uint64_t key2[] { 9, 0 };
    map->insert(key2, 2);
    uint64_t key3[] { 18, 0 };
    map->insert(key3, 3);

    EXPECT_EQ(3U, map->size());
    EXPECT_EQ(2U, map->max_psl());

    EXPECT_EQ(1U, map->lookup(key1));
    EXPECT_EQ(2U, map->lookup(key2));
    EXPECT_EQ(3U, map->lookup(key3));
}

TEST(ut_robin_hood_map, collision_swap) {
    char buf[32 * 9] {};
    auto map = map_type::make(buf, sizeof(buf));

    uint64_t key1[] { 1, 0 };
    map->insert(key1, 1);
    uint64_t key2[] { 9, 0 };
    map->insert(key2, 2);
    uint64_t key3[] { 2, 0 };
    map->insert(key3, 3);
    uint64_t key4[] { 17, 0 };
    map->insert(key4, 4);

    EXPECT_EQ(4U, map->size());
    EXPECT_EQ(3U, map->max_psl());

    EXPECT_EQ(1U, map->lookup(key1));
    EXPECT_EQ(2U, map->lookup(key2));
    EXPECT_EQ(3U, map->lookup(key3));
    EXPECT_EQ(4U, map->lookup(key4));
}

TEST(ut_robin_hood_map, wrapping) {
    char buf[32 * 9] {};
    auto map = map_type::make(buf, sizeof(buf));

    uint64_t key1[] { 7, 0 };
    map->insert(key1, 1);
    uint64_t key2[] { 15, 0 };
    map->insert(key2, 2);

    EXPECT_EQ(2U, map->size());
    EXPECT_EQ(2U, map->max_psl());

    EXPECT_EQ(1U, map->lookup(key1));
    EXPECT_EQ(2U, map->lookup(key2));
}

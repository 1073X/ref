#include <gtest/gtest.h>

#include "source/lib/ref/subsection.hpp"

struct ut_subsection : public testing::Test {
    using subsection = miu::ref::subsection<int32_t, int32_t, 16>;
    subsection map { std::numeric_limits<int32_t>::lowest(),
                     std::numeric_limits<int32_t>::max(),
                     1 };
};

TEST_F(ut_subsection, size) {
    auto size = (4 + 4) * 16U;
    EXPECT_EQ(size, sizeof(map));
}

TEST_F(ut_subsection, defval) {
    EXPECT_EQ(1U, map.segements());

    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::min()));
    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::max()));
}

TEST_F(ut_subsection, add) {
    map.add(1, 100);

    EXPECT_EQ(2U, map.segements());

    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::min()));
    EXPECT_EQ(100, map.get(1));
    EXPECT_EQ(100, map.get(std::numeric_limits<int32_t>::max() - 1));
    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::max()));
}

TEST_F(ut_subsection, confliction) {
    map.add(1, 100);
    ASSERT_ANY_THROW(map.add(1, 200));
    ASSERT_NO_THROW(map.add(1, 100));
}

TEST_F(ut_subsection, overflow) {
    for (auto i = 0U; i < subsection::max_lev() - 2; i++) {
        map.add(i, i * 100);
    }
    ASSERT_ANY_THROW(map.add(32, 9));
}

TEST_F(ut_subsection, add_more) {
    map.add(1, 100);
    map.add(10, 200);
    map.add(100, 300);

    // min ~ 1 ~ 10 ~ 100 ~ max
    EXPECT_EQ(4U, map.segements());

    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::min()));
    EXPECT_EQ(100, map.get(1));
    EXPECT_EQ(200, map.get(10));
    EXPECT_EQ(300, map.get(100));
    EXPECT_EQ(300, map.get(std::numeric_limits<int32_t>::max() - 1));
    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::max()));
}

TEST_F(ut_subsection, disorder) {
    map.add(1, 100);
    map.add(100, 300);
    map.add(10, 200);

    // min ~ 1 ~ 10 ~ 100 ~ max
    EXPECT_EQ(4U, map.segements());

    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::min()));
    EXPECT_EQ(100, map.get(1));
    EXPECT_EQ(200, map.get(10));
    EXPECT_EQ(300, map.get(100));
    EXPECT_EQ(300, map.get(std::numeric_limits<int32_t>::max() - 1));
    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::max()));
}

TEST_F(ut_subsection, merge_backward) {
    map.add(1, 100);
    map.add(10, 100);
    map.add(100, 300);

    // min ~ 1 ~ 100 ~ max
    EXPECT_EQ(3U, map.segements());

    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::min()));
    EXPECT_EQ(100, map.get(1));
    EXPECT_EQ(100, map.get(10));
    EXPECT_EQ(300, map.get(100));
    EXPECT_EQ(300, map.get(std::numeric_limits<int32_t>::max() - 1));
    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::max()));
}

TEST_F(ut_subsection, merge_forward) {
    map.add(1, 100);
    map.add(10, 300);
    map.add(100, 300);

    // min ~ 1 ~ 10 ~ max
    EXPECT_EQ(3U, map.segements());

    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::min()));
    EXPECT_EQ(100, map.get(1));
    EXPECT_EQ(300, map.get(10));
    EXPECT_EQ(300, map.get(100));
    EXPECT_EQ(300, map.get(std::numeric_limits<int32_t>::max() - 1));
    EXPECT_EQ(1, map.get(std::numeric_limits<int32_t>::max()));
}

TEST_F(ut_subsection, locate) {
    for (auto i = 0U; i < subsection::max_lev() - 2; i++) {
        map.add(i, i * 100);
    }

    EXPECT_EQ(std::make_tuple(0, 1), map.locate(std::numeric_limits<int32_t>::min()));
    EXPECT_EQ(std::make_tuple(1, 0), map.locate(0));
    EXPECT_EQ(std::make_tuple(2, 100), map.locate(1));
    EXPECT_EQ(std::make_tuple(3, 200), map.locate(2));
    EXPECT_EQ(std::make_tuple(4, 300), map.locate(3));
    EXPECT_EQ(std::make_tuple(5, 400), map.locate(4));
    EXPECT_EQ(std::make_tuple(13, 1200), map.locate(12));
    EXPECT_EQ(std::make_tuple(std::numeric_limits<int32_t>::max(), 1300), map.locate(13));
    EXPECT_EQ(std::make_tuple(std::numeric_limits<int32_t>::max(), 1),
              map.locate(std::numeric_limits<int32_t>::max()));
}

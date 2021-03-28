#include <gtest/gtest.h>

#include <shm/tempfs.hpp>

#include "stub/ref.hpp"

TEST(ut_stub, create) {
    miu::ref::stub stub;
    EXPECT_FALSE(stub.marker().empty());
    EXPECT_EQ("stub_" + stub.marker(), stub.name());
    std::cout << stub.marker() << std::endl;
    std::cout << stub.name() << std::endl;
}

TEST(ut_stub, db) {
    std::string marker;
    {
        miu::ref::stub stub;
        marker = stub.marker();

        // lazy creating
        EXPECT_FALSE(miu::shm::tempfs::exists(marker, "ref"));

        ASSERT_NE(nullptr, stub.db());
        EXPECT_TRUE(miu::shm::tempfs::exists(marker, "ref"));
    }
    EXPECT_FALSE(miu::shm::tempfs::exists(marker, "ref"));
}

TEST(ut_stub, instruments) {
    miu::ref::stub stub;

    // stocks
    EXPECT_TRUE(stub.db()->find("STUB/STOCK/STK01"));
    EXPECT_TRUE(stub.db()->find("STUB/STOCK/STK02"));

    // futures
    EXPECT_TRUE(stub.db()->find("STUB/FUTURE/FUT01/2103"));
    EXPECT_TRUE(stub.db()->find("STUB/FUTURE/FUT01/2104"));
}

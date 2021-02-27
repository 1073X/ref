#include <gtest/gtest.h>

#include <iostream>
#include <meta/info.hpp>

#include "ref/version.hpp"

TEST(ut_version, version) {
    std::cout << miu::ref::version() << std::endl;
    std::cout << miu::ref::build_info() << std::endl;

    std::cout << miu::meta::info() << std::endl;
}

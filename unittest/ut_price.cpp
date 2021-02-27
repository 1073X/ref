#include <gtest/gtest.h>

#include <cmath>    // std::fabs

#include "ref/price.hpp"

using miu::ref::price;

TEST(ut_price, value) {
    EXPECT_DOUBLE_EQ(0.0, static_cast<double>(price {}));    // default value

    EXPECT_DOUBLE_EQ(1.234, static_cast<double>(price { 1.234 }));
    EXPECT_DOUBLE_EQ(0.000005, static_cast<double>(price { 0.000005 }));
}

TEST(ut_price, equility) {
    EXPECT_TRUE(price { 1.2 } == price { 1.2 });
    EXPECT_TRUE(price { 1.2 } == price { 1.2 + price::epsilon() });
    EXPECT_TRUE(price { 1.2 } == price { 1.2 - price::epsilon() });

    EXPECT_FALSE(price { 1.2 } == price { 1.1 });
}

TEST(ut_price, greater) {
    EXPECT_TRUE(price { 1.2 } > price { 1.1 });
    EXPECT_TRUE(price { 1.2 } > 1.1);

    EXPECT_FALSE(price(1.2) > price { 1.3 });
    EXPECT_FALSE(price { 1.2 + price::epsilon() } > price { 1.2 });
    EXPECT_FALSE(price { 1.2 } > price { 1.2 - price::epsilon() });
}

TEST(ut_price, greater_equal) {
    EXPECT_TRUE(price { 1.2 } >= price { 1.1 });
    EXPECT_TRUE(price { 1.2 } >= 1.1);
    EXPECT_TRUE(price { 1.2 } >= price { 1.2 });
    EXPECT_TRUE(price { 1.2 } >= price { 1.2 + price::epsilon() });
    EXPECT_TRUE(price { 1.2 - price::epsilon() } >= price { 1.2 });

    EXPECT_FALSE(price { 1.2 } >= price { 1.3 });
}

TEST(ut_price, less) {
    EXPECT_TRUE(price { 1.2 } < price { 1.3 });
    EXPECT_TRUE(price { 1.2 } < 1.3);

    EXPECT_FALSE(price(1.2) < price { 1.1 });
    EXPECT_FALSE(price { 1.2 - price::epsilon() } < price { 1.2 });
    EXPECT_FALSE(price { 1.2 } < price { 1.2 + price::epsilon() });
}

TEST(ut_price, less_equal) {
    EXPECT_TRUE(price { 1.2 } <= price { 1.3 });
    EXPECT_TRUE(price { 1.2 } <= 1.3);
    EXPECT_TRUE(price { 1.2 } <= price { 1.2 });
    EXPECT_TRUE(price { 1.2 } <= price { 1.2 + price::epsilon() });
    EXPECT_TRUE(price { 1.2 - price::epsilon() } <= price { 1.2 });

    EXPECT_FALSE(price { 1.2 } <= price { 1.1 });
}

TEST(ut_price, addition) {
    EXPECT_EQ(price { 0.8 }, price { 0.6 } + price { 0.2 });
    EXPECT_EQ(price { 0.8 }, price { 0.6 } + 0.2);
    EXPECT_EQ(price { 0.8 }, 0.6 + price { 0.2 });

    auto prc = price { 0.8 };
    EXPECT_EQ(price { 0.9 }, prc += price { 0.1 });
    EXPECT_EQ(price { 0.9 }, prc);
    EXPECT_EQ(price { 1.1 }, prc += 0.2);
    EXPECT_EQ(price { 1.1 }, prc);

    double val = 0.1;
    val += prc;
    EXPECT_DOUBLE_EQ(1.2, val);
}

TEST(ut_price, subtraction) {
    EXPECT_EQ(price { 0.8 }, price { 1.2 } - price { 0.4 });
    EXPECT_EQ(price { 0.8 }, price { 1.2 } - 0.4);
    EXPECT_EQ(price { 0.8 }, 1.2 - price { 0.4 });

    auto prc = price { 0.8 };
    EXPECT_EQ(price { 0.7 }, prc -= price { 0.1 });
    EXPECT_EQ(price { 0.7 }, prc);
    EXPECT_EQ(price { 0.5 }, prc -= 0.2);
    EXPECT_EQ(price { 0.5 }, prc);

    double val = 0.1;
    val -= prc;
    EXPECT_DOUBLE_EQ(-0.4, val);
}

TEST(ut_price, multiplication) {
    EXPECT_EQ(price { 0.8 }, price { 0.2 } * price { 4 });
    EXPECT_EQ(price { 0.8 }, price { 0.2 } * 4);
    EXPECT_EQ(price { 0.8 }, 0.2 * price { 4 });

    auto prc = price { 0.8 };
    EXPECT_EQ(price { 1.6 }, prc *= price { 2 });
    EXPECT_EQ(price { 1.6 }, prc);
    EXPECT_EQ(price { 0.32 }, prc *= 0.2);
    EXPECT_EQ(price { 0.32 }, prc);

    double val = 0.1;
    val *= prc;
    EXPECT_DOUBLE_EQ(0.032, val);
}

TEST(ut_price, division) {
    EXPECT_EQ(price { 0.8 }, price { 1.6 } / price { 2 });
    EXPECT_EQ(price { 0.8 }, price { 1.6 } / 2);
    EXPECT_EQ(price { 0.8 }, 1.6 / price { 2 });

    auto prc = price { 0.8 };
    EXPECT_EQ(price { 0.4 }, prc /= price { 2 });
    EXPECT_EQ(price { 0.4 }, prc);
    EXPECT_EQ(price { 2 }, prc /= 0.2);
    EXPECT_EQ(price { 2 }, prc);

    double val = 0.1;
    val /= prc;
    EXPECT_DOUBLE_EQ(0.05, val);
}

TEST(ut_price, variant) {
    using miu::com::type_id;
    using miu::com::variant;

    auto prc = price { 1.8 };
    auto var = variant { prc };
    EXPECT_EQ(price { 1.8 }, var.get<price>().value());

    EXPECT_EQ(price { 123 }, variant { 123 }.get<price>().value());
    EXPECT_EQ(price { 123.1 }, variant { +"123.1" }.get<price>().value());
}

TEST(ut_price, to_string) {
    using miu::com::to_string;

    EXPECT_EQ("2.1", to_string(price { 2.1 }));
    EXPECT_EQ("23.0011111", to_string(price { 23.0011111 }));
    EXPECT_EQ("123.0011111", to_string(price { 123.0011111 }));
    EXPECT_EQ("1234567.1234567", to_string(price { 1234567.1234567 }));
}

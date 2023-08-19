#include "power.hpp"

#include <gtest/gtest.h>

TEST(Math, Power) {
    static_assert(Math::Power(2, 0) == 1);
    static_assert(Math::Power(2, 1) == 2);
    static_assert(Math::Power(2, 2) == 4);
    static_assert(Math::Power(2, 3) == 8);
    static_assert(Math::Power(3, 2) == 9);
    EXPECT_EQ(Math::Power(1, 2), 1);
    EXPECT_EQ(Math::Power(2, 2), 4);
}

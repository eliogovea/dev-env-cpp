#include "math_power.hpp"

#include <gtest/gtest.h>

TEST(Math, Power)
{
    static_assert(Math::Power(2, 0U) == 1);  // NOLINT
    static_assert(Math::Power(2, 1U) == 2);  // NOLINT
    static_assert(Math::Power(2, 2U) == 4);  // NOLINT
    static_assert(Math::Power(2, 3U) == 8);  // NOLINT
    static_assert(Math::Power(3, 2U) == 9);  // NOLINT
    EXPECT_EQ(Math::Power(1, 2U), 1);        // NOLINT
    EXPECT_EQ(Math::Power(2, 2U), 4);        // NOLINT
}

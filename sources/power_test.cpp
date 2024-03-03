#include "power.hpp"

#include <gtest/gtest.h>

TEST(Math, Power)
{
    static_assert(Math::Power(2, 0) == 1);  // NOLINT
    static_assert(Math::Power(2, 1) == 2);  // NOLINT
    static_assert(Math::Power(2, 2) == 4);  // NOLINT
    static_assert(Math::Power(2, 3) == 8);  // NOLINT
    static_assert(Math::Power(3, 2) == 9);  // NOLINT
    EXPECT_EQ(Math::Power(1, 2), 1);        // NOLINT
    EXPECT_EQ(Math::Power(2, 2), 4);        // NOLINT
}

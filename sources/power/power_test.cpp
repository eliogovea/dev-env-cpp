#include "power.hpp"

#include <gtest/gtest.h>

TEST(Power, Power)
{
    static_assert(Power::Power(2, 0U) == 1);  // NOLINT
    static_assert(Power::Power(2, 1U) == 2);  // NOLINT
    static_assert(Power::Power(2, 2U) == 4);  // NOLINT
    static_assert(Power::Power(2, 3U) == 8);  // NOLINT
    static_assert(Power::Power(3, 2U) == 9);  // NOLINT
    EXPECT_EQ(Power::Power(1, 2U), 1);        // NOLINT
    EXPECT_EQ(Power::Power(2, 2U), 4);        // NOLINT
}

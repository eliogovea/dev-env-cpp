#include <gtest/gtest.h>

#include "index_coordinator.hpp"

TEST(IndexCoordinator, TakeAndGive)
{
    auto coordinator = IndexCoordinator<std::uint8_t, 256U>{};

    EXPECT_EQ(coordinator.size(), 0U);
    EXPECT_EQ(coordinator.capacity(), 256U);

    auto const index0 = coordinator.take();

    EXPECT_EQ(coordinator.size(), 1U);
    EXPECT_LT(index0, coordinator.capacity());

    auto const index1 = coordinator.take();

    EXPECT_EQ(coordinator.size(), 2U);
    EXPECT_LT(index1, coordinator.capacity());

    EXPECT_NE(index0, index1);

    coordinator.give(index0);

    EXPECT_EQ(coordinator.size(), 1U);

    coordinator.give(index1);

    EXPECT_EQ(coordinator.size(), 0U);
}

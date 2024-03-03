#include <gtest/gtest.h>

#include "geometry_point.hpp"
#include "geometry_point_operations.hpp"

TEST(GeometryPoint, CheckPointDot)
{
    auto const lhs = Geometry::Point<int>{0, 1};
    auto const rhs = Geometry::Point<int>{1, 0};
    auto const dot = int{0};
    EXPECT_EQ(Geometry::PointDot(lhs, rhs), dot);
}

TEST(GeometryPoint, CheckPointCross)
{
    auto const lhs   = Geometry::Point<int>{1, 2};
    auto const rhs   = Geometry::Point<int>{2, 4};
    auto const cross = int{0};
    EXPECT_EQ(Geometry::PointCross(lhs, rhs), cross);
}

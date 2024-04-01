#include "geometry_point.hpp"

#include <gtest/gtest.h>

template struct Geometry::Point<int>;

TEST(GeometryPoint, CheckPointConstructor)
{
    auto const point = Geometry::Point<int>(1, 2);
    EXPECT_EQ(point.x, 1);
    EXPECT_EQ(point.y, 2);
}

TEST(GeometryPoint, CheckPointEqual)
{
    auto const lhs = Geometry::Point<int>{1, 1};
    auto const rhs = Geometry::Point<int>{1, 1};
    EXPECT_EQ(lhs, rhs);
}

TEST(GeometryPoint, CheckLessDifferentX)
{
    auto const lhs = Geometry::Point<int>{1, 1};
    auto const rhs = Geometry::Point<int>{2, 0};
    EXPECT_TRUE(lhs < rhs);
    EXPECT_TRUE(!(rhs < lhs));
}

TEST(GeometryPoint, CheckLessSameX)
{
    auto const lhs = Geometry::Point<int>{1, 0};
    auto const rhs = Geometry::Point<int>{1, 1};
    EXPECT_TRUE(lhs < rhs);
    EXPECT_TRUE(!(rhs < lhs));
}

TEST(GeometryPoint, CheckPointAdd)
{
    auto const lhs   = Geometry::Point<int>{1, 1};
    auto const rhs   = Geometry::Point<int>{1, 1};
    auto const added = Geometry::Point<int>{2, 2};
    EXPECT_EQ(lhs + rhs, added);
}

TEST(GeometryPoint, CheckPointSubstract)
{
    auto const lhs         = Geometry::Point<int>{1, 1};
    auto const rhs         = Geometry::Point<int>{1, 1};
    auto const substracted = Geometry::Point<int>{0, 0};
    EXPECT_EQ(lhs - rhs, substracted);
}

TEST(GeometryPoint, CheckPointScaleUp)
{
    auto const lhs    = Geometry::Point<int>{1, 2};
    auto const rhs    = 2;
    auto const scaled = Geometry::Point<int>{2, 4};
    EXPECT_EQ(lhs * rhs, scaled);
    EXPECT_EQ(rhs * lhs, scaled);
}

TEST(GeometryPoint, CheckPointScaleDown)
{
    auto const lhs    = Geometry::Point<int>{2, 4};
    auto const rhs    = 2;
    auto const scaled = Geometry::Point<int>{1, 2};
    EXPECT_EQ(lhs / rhs, scaled);
}

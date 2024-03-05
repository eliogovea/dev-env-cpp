#include <gtest/gtest.h>

#include "geometry_point.hpp"
#include "geometry_point_operations.hpp"

template struct Geometry::PointComparatorXY<Geometry::Point<int>>;
template struct Geometry::PointComparatorYX<Geometry::Point<int>>;

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

TEST(GeometryPoint, CheckPointComparatorXY)
{
    using Point             = Geometry::Point<int>;
    using PointComparatorXY = Geometry::PointComparatorXY<Point>;

    auto const comparator_xy = PointComparatorXY{};

    // compare points with different x
    EXPECT_TRUE(comparator_xy(Point{0, 0}, Point{1, -1}));
    EXPECT_FALSE(comparator_xy(Point{1, -1}, Point{0, 0}));

    // compare points with same x
    EXPECT_TRUE(comparator_xy(Point{0, 0}, Point{0, +1}));
    EXPECT_FALSE(comparator_xy(Point{0, +1}, Point{0, 0}));

    // compare equal points
    EXPECT_FALSE(comparator_xy(Point{0, 0}, Point{0, 0}));
}

TEST(GeometryPoint, CheckPointComparatorYX)
{
    using Point             = Geometry::Point<int>;
    using PointComparatorYX = Geometry::PointComparatorYX<Point>;

    auto const comparator_yx = PointComparatorYX{};

    // compare points with different y
    EXPECT_TRUE(comparator_yx(Point{0, 0}, Point{-1, +1}));
    EXPECT_FALSE(comparator_yx(Point{-1, +1}, Point{0, 0}));

    // compare points with same y
    EXPECT_TRUE(comparator_yx(Point{0, 0}, Point{+1, 0}));
    EXPECT_FALSE(comparator_yx(Point{+1, 0}, Point{0, 0}));

    // compare equal points
    EXPECT_FALSE(comparator_yx(Point{0, 0}, Point{0, 0}));
}
#include <gtest/gtest.h>

#include "geometry_polygon_convex.hpp"

TEST(GeometryPolygonConvex, CheckConvexTriangle)
{
    Geometry::Point<int> const points[] = {
        {0, 0},
        {1, 0},
        {1, 1},
    };

    EXPECT_TRUE(Geometry::CheckConvex(std::span{points}));
}

TEST(GeometryPolygonConvex, CheckConvexCCW)
{
    Geometry::Point<int> const points[] = {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1},
    };

    EXPECT_TRUE(Geometry::CheckConvex(std::span{points}));
}

TEST(GeometryPolygonConvex, CheckConvexCW)
{
    Geometry::Point<int> const points[] = {
        {0, 0},
        {0, 1},
        {1, 1},
        {1, 0},
    };

    EXPECT_TRUE(Geometry::CheckConvex(std::span{points}));
}

TEST(GeometryPolygonConvex, CheckNoConvexCCW)
{
    Geometry::Point<int> const points[] = {
        {0, 0},
        {3, 0},
        {1, 1},
        {0, 3},
    };

    EXPECT_FALSE(Geometry::CheckConvex(std::span{points}));
}

TEST(GeometryPolygonConvex, CheckNoConvexCW)
{
    Geometry::Point<int> const points[] = {
        {0, 0},
        {0, 3},
        {1, 1},
        {3, 0},
    };

    EXPECT_FALSE(Geometry::CheckConvex(std::span{points}));
}

TEST(GeometryPolygonConvex, NormalizeThreePointsLine)
{
    Geometry::Point<int> points[] = {
        {0, 0},
        {1, 1},
        {2, 2}
    };

    EXPECT_TRUE(Geometry::CheckConvex(std::span{points}));

    auto points_normalized
        = Geometry::NormalizeConvexPolygon(std::span{points});

    EXPECT_EQ(points_normalized.size(), 2);
    EXPECT_EQ(points_normalized[0], Geometry::Point(0, 0));
    EXPECT_EQ(points_normalized[1], Geometry::Point(2, 2));
}

TEST(GeometryPolygonConvex, NormalizeThreePoints)
{
    Geometry::Point<int> points[] = {
        {1, 0},
        {0, 1},
        {0, 0}
    };

    auto points_normalized
        = Geometry::NormalizeConvexPolygon(std::span{points});

    EXPECT_EQ(points_normalized.size(), 3);
    EXPECT_EQ(points_normalized[0], Geometry::Point(0, 0));
    EXPECT_EQ(points_normalized[1], Geometry::Point(1, 0));
    EXPECT_EQ(points_normalized[2], Geometry::Point(0, 1));
}

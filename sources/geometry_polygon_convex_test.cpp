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

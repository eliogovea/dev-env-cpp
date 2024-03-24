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

TEST(GeometryPolygonConvex, CheckConvexHullGrahamScanOnePoint)
{
    Geometry::Point<int> points[] = {
        {0, 0},
    };

    auto points_convex_hull
        = Geometry::GetConvexHulGrahamScan(std::span{points});

    EXPECT_EQ(points_convex_hull.size(), 1);
    EXPECT_EQ(points_convex_hull[0], Geometry::Point(0, 0));
}

TEST(GeometryPolygonConvex, CheckConvexHullGrahamScanTwoEqualPoints)
{
    Geometry::Point<int> points[] = {
        {0, 0},
        {0, 0},
    };

    auto points_convex_hull
        = Geometry::GetConvexHulGrahamScan(std::span{points});

    EXPECT_EQ(points_convex_hull.size(), 1);
    EXPECT_EQ(points_convex_hull[0], Geometry::Point(0, 0));
}

TEST(GeometryPolygonConvex, CheckConvexHullGrahamScanThreeEqualPoints)
{
    Geometry::Point<int> points[] = {
        {0, 0},
        {0, 0},
        {0, 0},
    };

    auto points_convex_hull
        = Geometry::GetConvexHulGrahamScan(std::span{points});

    EXPECT_EQ(points_convex_hull.size(), 1);
    EXPECT_EQ(points_convex_hull[0], Geometry::Point(0, 0));
}

TEST(GeometryPolygonConvex, CheckConvexHullGrahamScan)
{
    Geometry::Point<int> points[] = {
        {0, 2},
        {1, 2},
        {2, 2},
        {0, 0},
        {1, 0},
        {2, 0},
        {0, 1},
        {1, 1},
        {2, 1},
    };

    auto const convex_hull
        = Geometry::GetConvexHulGrahamScan(std::span{points});

    EXPECT_EQ(convex_hull.size(), 4);
    EXPECT_EQ(convex_hull[0], Geometry::Point(0, 0));
    EXPECT_EQ(convex_hull[1], Geometry::Point(2, 0));
    EXPECT_EQ(convex_hull[2], Geometry::Point(2, 2));
    EXPECT_EQ(convex_hull[3], Geometry::Point(0, 2));
}

TEST(GeometryPolygonConvex, CheckPointVsConvexZeroPoints)
{
    Geometry::Point<int> convex[] = {
        {0, 0}
    };

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex, 0U}, {0, 0}),
              Geometry::PointRelativePosition::Out);
}

TEST(GeometryPolygonConvex, CheckPointVsConvexOnePoint)
{
    Geometry::Point<int> convex[] = {
        {0, 0},
    };

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {0, 0}),
              Geometry::PointRelativePosition::On);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {1, 0}),
              Geometry::PointRelativePosition::Out);
}

TEST(GeometryPolygonConvex, CheckPointVsConvexTwoPoints)
{
    Geometry::Point<int> convex[] = {
        {0, 0},
        {2, 0},
    };

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {-1, 0}),
              Geometry::PointRelativePosition::Out);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {0, 0}),
              Geometry::PointRelativePosition::On);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {1, 0}),
              Geometry::PointRelativePosition::On);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {2, 0}),
              Geometry::PointRelativePosition::On);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {3, 0}),
              Geometry::PointRelativePosition::Out);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {0, -1}),
              Geometry::PointRelativePosition::Out);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {0, 1}),
              Geometry::PointRelativePosition::Out);
}

TEST(GeometryPolygonConvex, CheckPointVsConvex)
{
    Geometry::Point<int> convex[] = {
        {0, 0},
        {2, 0},
        {2, 2},
        {0, 2},
    };

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {-1, 0}),
              Geometry::PointRelativePosition::Out);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {0, -1}),
              Geometry::PointRelativePosition::Out);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {0, 0}),
              Geometry::PointRelativePosition::On);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {1, 0}),
              Geometry::PointRelativePosition::On);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {2, 2}),
              Geometry::PointRelativePosition::On);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {3, 2}),
              Geometry::PointRelativePosition::Out);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {1, 1}),
              Geometry::PointRelativePosition::In);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {2, 2}),
              Geometry::PointRelativePosition::On);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {3, 3}),
              Geometry::PointRelativePosition::Out);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {0, 1}),
              Geometry::PointRelativePosition::On);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {0, 2}),
              Geometry::PointRelativePosition::On);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {0, 3}),
              Geometry::PointRelativePosition::Out);

    EXPECT_EQ(Geometry::PointVsConvexPolygon(std::span{convex}, {-1, 1}),
              Geometry::PointRelativePosition::Out);
}

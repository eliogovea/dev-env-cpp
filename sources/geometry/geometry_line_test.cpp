#include <gtest/gtest.h>

#include "geometry_line.hpp"

template struct Geometry::Line<Geometry::Point<int>>;

TEST(GeometryLine, CheckLineNormalize)
{
    using Point = Geometry::Point<int>;
    using Line  = Geometry::Line<Point>;

    auto const line0 = Line{
        .direction = Point{-1, -1},
        .cross     = -1,
    };

    EXPECT_EQ(line0.Normalize().direction, Point(1, 1));
    EXPECT_EQ(line0.Normalize().cross, 1);

    auto const line1 = Line{
        .direction = Point{-1, +1},
        .cross     = -1,
    };

    EXPECT_EQ(line1.Normalize().direction, Point(1, -1));
    EXPECT_EQ(line1.Normalize().cross, 1);

    auto const line2 = Line{
        .direction = Point{+1, -1},
        .cross     = -1,
    };

    EXPECT_EQ(line2.Normalize().direction, Point(+1, -1));
    EXPECT_EQ(line2.Normalize().cross, -1);

    auto const line3 = Line{
        .direction = Point{+1, +1},
        .cross     = +1,
    };

    EXPECT_EQ(line3.Normalize().direction, Point(+1, +1));
    EXPECT_EQ(line3.Normalize().cross, +1);
}

TEST(GeometryLine, CheckPointRelativePosition)
{
    using Point = Geometry::Point<int>;
    using Line  = Geometry::Line<Point>;

    auto const line = Line{
        .direction = Point{1, 1},
        .cross     = 0,
    };

    EXPECT_EQ(line.GetRelativePosition(Point{0, 1}),
              Geometry::LineRelativePositionIdentifier::Left);

    EXPECT_EQ(line.GetRelativePosition(Point{0, 0}),
              Geometry::LineRelativePositionIdentifier::On);

    EXPECT_EQ(line.GetRelativePosition(Point{1, 0}),
              Geometry::LineRelativePositionIdentifier::Right);
}

TEST(GeometryLine, CheckParallelLines)
{
    using Point = Geometry::Point<int>;
    using Line  = Geometry::Line<Point>;

    auto const lhs = Line{
        .direction = Point{+1, +1},
        .cross     = +1,
    };

    auto const rhs = Line{
        .direction = Point{-1, -1},
        .cross     = -1,
    };

    EXPECT_TRUE(lhs.CheckLineParallel(rhs));
    EXPECT_TRUE(rhs.CheckLineParallel(lhs));
}

TEST(GeometryLine, CheckNotParallelLines)
{
    using Point = Geometry::Point<int>;
    using Line  = Geometry::Line<Point>;

    auto const lhs = Line{
        .direction = Point{+1, +1},
        .cross     = +1,
    };

    auto const rhs = Line{
        .direction = Point{-1, +1},
        .cross     = -1,
    };

    EXPECT_FALSE(lhs.CheckLineParallel(rhs));
    EXPECT_FALSE(rhs.CheckLineParallel(lhs));
}

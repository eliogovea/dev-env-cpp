#include <gtest/gtest.h>

#include "geometry_segment.hpp"
#include "geometry_segment_traits.hpp"

template struct Geometry::Segment<Geometry::Point<int>>;

TEST(GeometrySegment, CheckPointRelativePosition)
{
    using Point   = Geometry::Point<int>;
    using Segment = Geometry::Segment<Point>;

    auto const segment = Segment{
        Point{0, 0},
        Point{2, 2}
    };

    EXPECT_TRUE(segment.CheckPointInside(Point{0, 0}));
    EXPECT_TRUE(segment.CheckPointInside(Point{1, 1}));
    EXPECT_TRUE(segment.CheckPointInside(Point{2, 2}));

    EXPECT_FALSE(segment.CheckPointInside(Point{3, 3}));
    EXPECT_FALSE(segment.CheckPointInside(Point{1, 2}));
}

TEST(GeometrySegment, CheckParallelSegments)
{
    using Point   = Geometry::Point<int>;
    using Segment = Geometry::Segment<Point>;

    auto const lhs = Segment{
        Point{0, 0},
        Point{1, 2}
    };

    auto const rhs = Segment{
        Point{0, 1},
        Point{1, 3}
    };

    EXPECT_TRUE(lhs.CheckSegmentParallel(rhs));
    EXPECT_TRUE(rhs.CheckSegmentParallel(lhs));
}

TEST(GeometrySegment, CheckNotParallelSegments)
{
    using Point   = Geometry::Point<int>;
    using Segment = Geometry::Segment<Point>;

    auto const lhs = Segment{
        Point{0, 0},
        Point{1, 2}
    };

    auto const rhs = Segment{
        Point{0, 1},
        Point{1, 4}
    };

    EXPECT_FALSE(lhs.CheckSegmentParallel(rhs));
    EXPECT_FALSE(rhs.CheckSegmentParallel(lhs));
}

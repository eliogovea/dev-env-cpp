#pragma once

#include "geometry_point.hpp"
#include "geometry_point_operations.hpp"
#include "geometry_point_traits.hpp"

namespace Geometry {

enum class SegmentRelativeHalfPlaneIdentifier
{
    Up   = -1,
    Down = +1
};

template <typename Point>
    requires PointCheckValue<Point>
struct Segment
{
    Point from = Point{};
    Point to   = Point{};

    auto CheckPointInside(Point const& point) const -> bool
    {
        return PointCross(point - from, to - from) == PointCoordinateZero<Point>
            && PointDot(point - from, point - to) <= PointCoordinateZero<Point>;
    }

    auto CheckSegmentParallel(Segment const& segment) const -> bool
    {
        return PointCross(from - to, segment.from - segment.to)
            == PointCoordinateZero<Point>;
    }
};

}  // namespace Geometry

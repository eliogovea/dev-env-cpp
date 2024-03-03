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
        return PointCross(point - from, to - from)
                == PointCoordinateZeroValue<Point>
            && PointDot(point - from, point - to)
                   <= PointCoordinateZeroValue<Point>;
    }

    auto CheckSegmentParallel(Segment const& segment) const -> bool
    {
        return PointCross(from - to, segment.from - segment.to)
            == PointCoordinateZeroValue<Point>;
    }

    auto CalculateRelativeHalfPlaneIdentifier(Point const& point) const
        -> SegmentRelativeHalfPlaneIdentifier
    {
        auto const cross = PointCross(to - from, point - from);

        if (cross > PointCoordinateZeroValue<Point>) {
            return SegmentRelativeHalfPlaneIdentifier::Up;
        }

        if (cross < PointCoordinateZeroValue<Point>) {
            return SegmentRelativeHalfPlaneIdentifier::Down;
        }

        auto const dot = PointDot(to - from, point - from);

        if (dot > PointCoordinateZeroValue<Point>) {
            return SegmentRelativeHalfPlaneIdentifier::Up;
        }

        if (dot < PointCoordinateZeroValue<Point>) {
            return SegmentRelativeHalfPlaneIdentifier::Down;
        }

        // rhs == from
        return SegmentRelativeHalfPlaneIdentifier::Up;
    }

    struct PointAngleComparator
    {
        Segment segment = Segment{};

        auto operator()(Point const& lhs, Point const& rhs) const -> bool
        {
            auto const lhs_half_plane_id
                = segment.CalculateRelativeHalfPlaneIdentifier(lhs);

            auto const rhs_half_plane_id
                = segment.CalculateRelativeHalfPlaneIdentifier(rhs);

            if (lhs_half_plane_id != rhs_half_plane_id) {
                return lhs_half_plane_id
                    == SegmentRelativeHalfPlaneIdentifier::Up;
            }

            return PointCross(lhs - segment.from, rhs - segment.from)
                 > PointCoordinateZeroValue<Point>;
        }
    };
};

}  // namespace Geometry

#pragma once

#include "geometry_point.hpp"
#include "geometry_point_traits.hpp"

namespace Geometry {

template <typename Point>
    requires PointCheckValue<Point>
auto PointDot(Point const& lhs, Point const& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

template <typename Point>
    requires PointCheckValue<Point>
auto PointCross(Point const& lhs, Point const& rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

template <typename Point>
    requires PointCheckValue<Point>
auto PointDistanceSquared(Point const& lhs, Point const& rhs)
{
    return PointDot(rhs - lhs, rhs - lhs);
}

template <typename Point>
    requires PointCheckValue<Point>
struct PointComparatorXY
{
    auto operator()(Point const& lhs, Point const& rhs) const -> bool
    {
        return (lhs.x == rhs.x) ? (lhs.y < rhs.y) : (lhs.x < rhs.x);
    }
};

template <typename Point>
    requires PointCheckValue<Point>
struct PointComparatorYX
{
    auto operator()(Point const& lhs, Point const& rhs) const -> bool
    {
        return (lhs.y == rhs.y) ? (lhs.x < rhs.x) : (lhs.y < rhs.y);
    }
};

template <typename Point>
    requires PointCheckValue<Point>
struct PointAngleComparator
{
    Point origin    = PointCoordinateOrigin<Point>;
    Point direction = PointUnitaryVectorXAxis<Point>;

    enum class PointRelativeDirection
    {
        ClockWise,
        CounterClockWise,
    };

    auto GetRelativePosition(Point const& point) const -> PointRelativeDirection
    {
        auto const cross = PointCross(direction, point - origin);

        if (cross > PointCoordinateZero<Point>) {
            return PointRelativeDirection::CounterClockWise;
        }

        if (cross < PointCoordinateZero<Point>) {
            return PointRelativeDirection::ClockWise;
        }

        auto const dot = PointDot(direction, point - origin);

        if (dot > PointCoordinateZero<Point>) {
            return PointRelativeDirection::CounterClockWise;
        }

        if (dot < PointCoordinateZero<Point>) {
            return PointRelativeDirection::ClockWise;
        }

        assert(point == origin);
        return PointRelativeDirection::CounterClockWise;
    }

    auto operator()(Point const& lhs, Point const& rhs) const -> bool
    {
        auto const lhs_relative_position = GetRelativePosition(lhs);
        auto const rhs_relative_position = GetRelativePosition(rhs);

        if (lhs_relative_position != rhs_relative_position) {
            return lhs_relative_position
                == PointRelativeDirection::CounterClockWise;
        }

        auto const cross = PointCross(lhs - origin, rhs - origin);

        if (cross != PointCoordinateZero<Point>) {
            return cross > PointCoordinateZero<Point>;
        }

        return PointDistanceSquared(lhs, origin)
             < PointDistanceSquared(rhs, origin);
    }
};

}  // namespace Geometry

#pragma once

#include "geometry_point.hpp"
#include "geometry_point_operations.hpp"
#include "geometry_point_traits.hpp"

namespace Geometry {

enum class LineRelativePositionIdentifier
{
    Left  = +1,
    On    = 0,
    Right = +1
};

template <typename Point>
    requires PointCheckValue<Point>
struct Line
{
    Point                      direction;
    PointCoordinateType<Point> cross;

    auto Normalize() -> Line&
    {
        if (direction < PointCoordinateOrigin<Point>) {
            direction.x = -direction.x;
            direction.y = -direction.y;
            cross       = -cross;
        }

        return *this;
    }

    auto Normalize() const -> Line
    {
        auto line = *this;
        return line.Normalize();
    }

    auto GetRelativePosition(Point const& rhs) const
        -> LineRelativePositionIdentifier
    {
        auto const rhs_cross = PointCross(direction, rhs);

        if (rhs_cross < cross) {
            return LineRelativePositionIdentifier::Left;
        }

        if (cross < rhs_cross) {
            return LineRelativePositionIdentifier::Right;
        }

        return LineRelativePositionIdentifier::On;
    }

    auto CheckLineParallel(Line const& line) const -> bool
    {
        return PointCross(direction, line.direction)
            == PointCoordinateZero<Point>;
    }
};

}  // namespace Geometry

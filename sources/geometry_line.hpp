#pragma once

#include "geometry_point.hpp"
#include "geometry_point_operations.hpp"
#include "geometry_point_traits.hpp"

namespace Geometry {

enum class LineRelativePositionIdentifier
{
    Down = -1,
    On   = 0,
    Up   = +1
};

template <typename Point>
    requires PointCheckValue<Point>
struct Line
{
    Point                      direction;
    PointCoordinateType<Point> cross;

    auto Normalize() -> Line&
    {
        if (direction < PointCoordinateOriginValue<Point>) {
            direction.x = -direction.x;
            direction.y = -direction.y;
            cross       = -cross;
        }

        return *this;
    }

    auto GetRelativePosition(Point const& rhs) const
        -> LineRelativePositionIdentifier
    {
        auto const rhs_cross = PointCross(direction, rhs);

        if (rhs_cross < cross) {
            return LineRelativePositionIdentifier::Down;
        }

        if (cross < rhs_cross) {
            return LineRelativePositionIdentifier::Up;
        }

        return LineRelativePositionIdentifier::On;
    }
};

}  // namespace Geometry

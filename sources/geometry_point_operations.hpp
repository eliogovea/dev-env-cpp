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

}  // namespace Geometry

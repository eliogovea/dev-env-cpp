#pragma once

#include "geometry_line.hpp"
#include "geometry_point_traits.hpp"

namespace Geometry {

template <typename Line>
struct LineCheck : public std::false_type
{
};

template <typename Point>
    requires PointCheckValue<Point>
struct LineCheck<Line<Point>> : public std::true_type
{
};

template <typename Line>
inline constexpr auto LineCheckValue = LineCheck<Line>::value;

template <typename Line>
    requires LineCheckValue<Line>
struct LinePoint;

template <typename Point>
struct LinePoint<Line<Point>>
{
    using Type = Point;
};

template <typename Line>
    requires LineCheckValue<Line>
using LinePointType = typename LinePoint<Line>::Type;

template <typename Line>
    requires LineCheckValue<Line>
struct LineCross;

template <typename Point>
struct LineCross<Line<Point>>
{
    using Type = PointCoordinateType<Point>;
};

template <typename Line>
    requires LineCheckValue<Line>
using LineCrossType = typename LineCross<Line>::Type;

}  // namespace Geometry

#pragma once

#include <type_traits>

#include "geometry_point.hpp"

namespace Geometry {

template <typename Point>
struct PointCheck : public std::false_type
{
};

template <typename Coordinate>
struct PointCheck<Point<Coordinate>> : public std::true_type
{
};

template <typename Point>
inline constexpr bool PointCheckValue = PointCheck<Point>::value;

template <typename Point>
    requires PointCheckValue<Point>
struct PointCoordinate;

template <typename Coordinate>
struct PointCoordinate<Point<Coordinate>>
{
    using Type = Coordinate;
};

template <typename Point>
    requires PointCheckValue<Point>
using PointCoordinateType = typename PointCoordinate<Point>::Type;

template <typename Point>
    requires PointCheckValue<Point>
inline constexpr auto PointCoordinateZero = PointCoordinateType<Point>{0};

template <typename Point>
    requires PointCheckValue<Point>
inline constexpr auto PointCoordinateOne = PointCoordinateType<Point>{1};

template <typename Point>
    requires PointCheckValue<Point>
inline constexpr auto PointCoordinateOrigin
    = Point{PointCoordinateZero<Point>, PointCoordinateZero<Point>};

template <typename Point>
    requires PointCheckValue<Point>
inline constexpr auto PointUnitaryVectorXAxis
    = Point{PointCoordinateOne<Point>, PointCoordinateZero<Point>};

template <typename Point>
    requires PointCheckValue<Point>
inline constexpr auto PointUnitaryVectorYAxis
    = Point{PointCoordinateZero<Point>, PointCoordinateOne<Point>};

}  // namespace Geometry

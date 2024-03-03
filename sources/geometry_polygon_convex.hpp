#pragma once

#include <algorithm>
#include <span>

#include "geometry_point.hpp"
#include "geometry_point_traits.hpp"

namespace Geometry {

template <typename Point>
    requires PointCheckValue<Point>
auto NormalizeConvexPolygon(std::span<Point> points) -> std::span<Point>
{
    auto points_begin = std::begin(points);
    auto points_end   = std::end(points);

    // Remove duplicates
    points_end = std::unique(points_begin, points_end, points_end);

    // Remove duplicates
    points_end
        = std::find_if(std::next(points_begin), points_end, *points_begin);

    // Get an iterator to the minimum point
    auto points_min  = std::min_element(points_begin, points_end);
    auto points_size = std::distance(points_begin, points_end);

    // Move `points_min` to the begining
    std::rotate(points_begin, points_min, points_end);
    points_begin = std::begin(points);
    points_min   = points_begin;
    points_end   = std::next(points_begin, points_size);

    // TODO

    return std::span<Point>{points_begin, points_size};
}

}  // namespace Geometry

#pragma once

#include <algorithm>
#include <span>

#include "geometry_point.hpp"
#include "geometry_point_operations.hpp"
#include "geometry_point_traits.hpp"
#include "geometry_value_sign.hpp"

namespace Geometry {

template <typename Point, std::size_t Size>
    requires PointCheckValue<Point>
auto CheckConvex(std::span<Point const, Size> points) -> bool
{
    if (points.size() <= 3) {
        return true;
    }

    auto sign_cross_all = ValueSign::Zero;

    for (std::size_t index = 0; index < points.size(); index++) {
        auto const& point = points[index];

        auto const& point_prev
            = points[index != 0 ? index - 1 : points.size() - 1];

        auto const& point_next
            = points[index != points.size() - 1 ? index + 1 : 0];

        auto const cross = PointCross(point_prev - point, point_next - point);

        auto const sign_cross = GetValueSign(cross);

        if (sign_cross == ValueSign::Zero) {
            continue;
        }

        if (sign_cross == sign_cross_all) {
            continue;
        }

        if (sign_cross_all != ValueSign::Zero) {
            return false;
        }

        sign_cross_all = sign_cross;
    }

    return true;
}

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

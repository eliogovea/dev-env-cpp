#pragma once

#include <algorithm>
#include <span>

#include "geometry_point.hpp"
#include "geometry_point_operations.hpp"
#include "geometry_point_traits.hpp"
#include "geometry_value_sign.hpp"

namespace Geometry {

template <typename Point, std::size_t Size>
    requires PointCheckValue<std::remove_cvref_t<Point>>
auto CheckConvex(std::span<Point, Size> points) -> bool
{
    if (points.size() <= 3U) {
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

template <typename Point, std::size_t Size>
    requires PointCheckValue<Point>
auto NormalizeConvexPolygon(std::span<Point, Size> points) -> std::span<Point>
{
    std::rotate(std::begin(points),
                std::min_element(std::begin(points), std::end(points)),
                std::end(points));

    auto const unique_end  = std::unique(std::begin(points), std::end(points));
    auto const unique_size = static_cast<std::size_t>(
        std::distance(std::cbegin(points), unique_end));

    std::size_t normalized_size = 2U;

    for (std::size_t index = normalized_size; index < unique_size; index++) {
        auto const cross
            = PointCross(points[index] - points[normalized_size - 1],
                         points[index] - points[normalized_size - 2]);
        if (cross == PointCoordinateZero<Point>) {
            normalized_size--;
        }
        points[normalized_size++] = points[index];
    }

    return points.subspan(0U, normalized_size);
}

}  // namespace Geometry

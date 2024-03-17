#pragma once

#include <algorithm>
#include <cassert>
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

    std::size_t size = 2U;

    for (std::size_t index = size; index < unique_size; index++) {
        auto const cross = PointCross(points[index] - points[size - 1],
                                      points[index] - points[size - 2]);
        if (cross == PointCoordinateZero<Point>) {
            size--;
        }
        points[size++] = points[index];
    }

    return points.subspan(0U, size);
}

template <typename Point, std::size_t Size>
    requires PointCheckValue<Point>
auto GetConvexHulGrahamScan(std::span<Point, Size> points) -> std::span<Point>
{
    if (std::size(points) <= 1U) {
        return points;
    }

    using std::swap;
    swap(*std::begin(points),
         *std::min_element(
             std::begin(points), std::end(points), PointComparatorYX<Point>{}));

    PointAngleComparator<Point> const comparator = {
        .origin    = *std::begin(points),
        .direction = PointUnitaryVectorXAxis<Point>,
    };

    std::sort(std::next(std::begin(points)), std::end(points), comparator);

    auto size = std::size_t{2U};

    for (auto index = size; index < std::size(points); index++) {
        while (size >= 2U
               && PointCross(points[index] - points[size - 2],
                             points[index] - points[size - 1])
                      <= PointCoordinateZero<Point>) {
            size--;
        }
        points[size++] = points[index];
    }

    if (*std::begin(points) == points[size - 1]) {
        size--;
    }

    return points.subspan(0U, size);
}

enum class PointRelativePosition
{
    In  = -1,
    On  = 0,
    Out = +1,
};

template <typename Point, std::size_t Size>
    requires PointCheckValue<std::remove_cvref_t<Point>>
auto PointVsConvexPolygon(std::span<Point, Size> const& convex,
                          Point const& point) -> PointRelativePosition
{
    if (std::size(convex) == 0U) {
        return PointRelativePosition::Out;
    }

    if (std::size(convex) == 1U) {
        if (point != convex[0]) {
            return PointRelativePosition::Out;
        }

        return PointRelativePosition::On;
    }

    if (std::size(convex) == 2U) {
        auto const cross = PointCross(point - convex[0], convex[1] - convex[0]);

        if (cross != PointCoordinateZero<Point>) {
            return PointRelativePosition::Out;
        }

        auto const dot = PointDot(point - convex[0], point - convex[1]);

        if (dot > PointCoordinateZero<Point>) {
            return PointRelativePosition::Out;
        }

        return PointRelativePosition::On;
    }

    {  // check segment convex[0] to convex[1]
        auto const cross = PointCross(point - convex[0], convex[1] - convex[0]);

        if (cross > PointCoordinateZero<Point>) {
            return PointRelativePosition::Out;
        }

        auto const dot = PointDot(convex[0] - point, convex[1] - point);

        if (cross == PointCoordinateZero<Point>
            && dot <= PointCoordinateZero<Point>) {
            return PointRelativePosition::On;
        }
    }

    auto const CompareRotation = [&](Point const& lhs, Point const& rhs) {
        return PointCross(lhs - convex[0], rhs - convex[0])
             > PointCoordinateZero<Point>;
    };

    auto const upper_bound = std::upper_bound(std::next(std::cbegin(convex)),
                                              std::cend(convex),
                                              point,
                                              CompareRotation);

    if (upper_bound == std::cend(convex)) {
        auto const cross
            = PointCross(convex.back() - convex[0], point - convex[0]);

        if (cross > PointCoordinateZero<Point>) {
            return PointRelativePosition::Out;
        }

        assert(cross == PointCoordinateZero<Point>);

        auto const dot = PointDot(convex.back() - point, convex[0] - point);

        if (dot > PointCoordinateZero<Point>) {
            return PointRelativePosition::Out;
        }

        return PointRelativePosition::On;
    }

    auto const cross = PointCross(*upper_bound - *std::prev(upper_bound),
                                  point - *std::prev(upper_bound));

    if (cross > PointCoordinateZero<Point>) {
        return PointRelativePosition::In;
    }

    if (cross < PointCoordinateZero<Point>) {
        return PointRelativePosition::Out;
    }

    return PointRelativePosition::On;
}

}  // namespace Geometry

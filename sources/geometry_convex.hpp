#pragma once

#include <algorithm>
#include <span>

#include "geometry_vector.hpp"

namespace Geometry
{

template <typename CoordinateValue, std::size_t Size>
auto NormalizeConvexPolygon(std::span<Vector<CoordinateValue>, Size> points) -> std::span<Vector<CoordinateValue>>
{
    std::rotate(std::begin(points), std::min_element(std::begin(points), std::end(points)), std::end(points));

    auto const unique_end  = std::unique(std::begin(points), std::end(points));
    auto const unique_size = static_cast<std::size_t>(std::distance(std::cbegin(points), unique_end));

    std::size_t size = 2U;

    for (std::size_t index = size; index < unique_size; ++index) {
        if (Vector<CoordinateValue>::CrossProduct(points[index] - points[size - 1], points[index] - points[size - 2])
            == 0) {
            size--;
        }
        points[size++] = points[index];
    }

    return points.subspan(0U, size);
}

template <typename CoordinateValue, std::size_t Size>
auto GetConvexHulGrahamScan(std::span<Vector<CoordinateValue>, Size> points) -> std::span<Vector<CoordinateValue>>
{
    if (std::size(points) <= 1U) {
        return points;
    }

    using std::swap;
    swap(*std::begin(points),
         *std::min_element(std::begin(points), std::end(points), Vector<CoordinateValue>::CompareXThenY));

    Vector<CoordinateValue>::ComparatorAngle const comparator = {
        .center    = *std::begin(points),
        .direction = Vector<CoordinateValue>::UnitAxisX(),
    };

    std::sort(std::next(std::begin(points)), std::end(points), comparator);

    auto size = std::size_t{2U};

    for (auto index = size; index < std::size(points); index++) {
        while (
            size >= 2U
            && Vector<CoordinateValue>::CrossProduct(points[index] - points[size - 2], points[index] - points[size - 1])
                   <= 0) {
            size--;
        }
        points[size++] = points[index];
    }

    if (*std::begin(points) == points[size - 1]) {
        size--;
    }

    return points.subspan(0U, size);
}

}  // namespace Geometry

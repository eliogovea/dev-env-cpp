#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <span>
#include <vector>

#include "geometry_point.hpp"
#include "geometry_polygon_convex.hpp"

extern "C" int LLVMFuzzerTestOneInput(uint8_t const* data, size_t size)
{
    uint16_t points_size = 0U;

    if (size < sizeof(points_size)) {
        return 0;
    }

    points_size = (points_size << 8U) | *data;
    data++;
    size--;

    points_size = (points_size << 8U) | *data;
    data++;
    size--;

    if (size <= static_cast<size_t>(2U * points_size)) {
        return 0;
    }

    auto points = std::vector<Geometry::Point<int>>(points_size, {0, 0});
    for (auto& point : points) {
        point.x = static_cast<int>(*data);
        data++;
        size--;
        point.y = static_cast<int>(*data);
        data++;
        size--;
    }

    auto const convex = Geometry::GetConvexHulGrahamScan(std::span{points});

    assert(Geometry::CheckConvex(convex));

    while (size >= 2U) {
        Geometry::Point<int> point = {0, 0};

        point.x = static_cast<int>(*data);
        data++;
        size--;
        point.y = static_cast<int>(*data);
        data++;
        size--;

        auto position = Geometry::PointVsConvexPolygon(convex, point);
        (void)position;
    }

    return EXIT_SUCCESS;
}

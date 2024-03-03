#include <type_traits>

#include <gtest/gtest.h>

#include "geometry_point.hpp"
#include "geometry_point_traits.hpp"

namespace Geometry {

static_assert(PointCheckValue<Point<int>>);
static_assert(PointCoordinateZeroValue<Point<int>> == 0);
static_assert(PointCoordinateOriginValue<Point<int>> == Point<int>{0, 0});
static_assert(std::is_same_v<PointCoordinateType<Point<int>>, int>);

}  // namespace Geometry

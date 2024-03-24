#include "geometry_line_traits.hpp"

namespace Geometry {

static_assert(LineCheckValue<Line<Point<int>>>);
static_assert(std::is_same_v<LinePointType<Line<Point<int>>>, Point<int>>);
static_assert(std::is_same_v<LineCrossType<Line<Point<int>>>, int>);

}  // namespace Geometry

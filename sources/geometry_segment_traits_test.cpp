#include "geometry_segment_traits.hpp"

namespace Geometry {

static_assert(SegmentCheckValue<Segment<Point<int>>>);
static_assert(
    std::is_same_v<SegmentPointType<Segment<Point<int>>>, Point<int>>);

}  // namespace Geometry

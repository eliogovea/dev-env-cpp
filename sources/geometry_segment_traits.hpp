#pragma once

#include <type_traits>

#include "geometry_segment.hpp"

namespace Geometry {

template <typename Segment>
struct SegmentCheck : public std::false_type
{
};

template <typename Point>
struct SegmentCheck<Segment<Point>> : public std::true_type
{
};

template <typename Segment>
inline constexpr bool SegmentCheckValue = SegmentCheck<Segment>::value;

template <typename Segment>
    requires SegmentCheckValue<Segment>
struct SegmentPoint;

template <typename Point>
struct SegmentPoint<Segment<Point>>
{
    using Type = Point;
};

template <typename Point>
    requires SegmentCheckValue<Point>
using SegmentPointType = typename SegmentPoint<Point>::Type;

}  // namespace Geometry

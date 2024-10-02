#pragma once

#include <cmath>
#include <type_traits>

namespace Geometry
{

template <typename CoordinateValue>
struct Vector {
    CoordinateValue x = {0};
    CoordinateValue y = {0};

    constexpr Vector(CoordinateValue x, CoordinateValue y)
        : x(x)
        , y(y)
    {
    }

    auto constexpr operator+=(Vector const& rhs) -> Vector&
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    constexpr friend auto operator+(Vector lhs, Vector const& rhs) -> Vector
    {
        lhs += rhs;
        return lhs;
    }

    auto constexpr operator-=(Vector const& rhs) -> Vector&
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    friend auto constexpr operator-(Vector lhs, Vector const& rhs) -> Vector
    {
        lhs -= rhs;
        return lhs;
    }

    auto constexpr operator*=(CoordinateValue const& rhs) -> Vector&
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    friend auto constexpr operator*(Vector lhs, CoordinateValue const& rhs) -> Vector
    {
        lhs *= rhs;
        return lhs;
    }

    friend auto constexpr operator*(CoordinateValue const& lhs, Vector rhs) -> Vector
    {
        rhs *= lhs;
        return rhs;
    }

    auto constexpr operator/=(CoordinateValue const& rhs) -> Vector&
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    friend auto constexpr operator/(Vector lhs, CoordinateValue const& rhs) -> Vector
    {
        lhs /= rhs;
        return lhs;
    }

    friend auto constexpr operator==(Vector const& lhs, Vector const& rhs) -> bool
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    friend auto constexpr operator<(Vector const& lhs, Vector const& rhs) -> bool
    {
        return (lhs.x == rhs.x) ? (lhs.y < rhs.y) : (lhs.x < rhs.x);
    }

    auto static constexpr DotProduct(Vector const& lhs, Vector const& rhs) -> CoordinateValue
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    auto static constexpr CrossProduct(Vector const& lhs, Vector const& rhs) -> CoordinateValue
    {
        return lhs.x * rhs.y - lhs.y * rhs.x;
    }

    auto constexpr SquaredLength() const -> CoordinateValue
    {
        return DotProduct(*this, *this);
    }

    auto static constexpr SquaredLength(Vector const& vector) -> CoordinateValue
    {
        return vector.SquaredLength();
    }

    template <typename CoordinateValueOther = CoordinateValue>
    auto constexpr Length() const
        -> std::enable_if_t<std::is_floating_point_v<CoordinateValueOther>, CoordinateValueOther>
    {
        return std::sqrt(static_cast<CoordinateValueOther>(SquaredLength()));
    }

    template <typename CoordinateValueOther = CoordinateValue>
    auto static constexpr Length(Vector<CoordinateValueOther> const& vector)
        -> std::enable_if_t<std::is_floating_point_v<CoordinateValueOther>, CoordinateValueOther>
    {
        return vector.Length();
    }

    auto static constexpr Zero() -> Vector
    {
        return {0, 0};
    }

    auto static constexpr UnitAxisX() -> Vector
    {
        return {1, 0};
    }

    auto static constexpr UnitAxisY() -> Vector
    {
        return {0, 1};
    }

    auto static constexpr CompareXThenY(Vector const& lhs, Vector const& rhs) -> bool
    {
        return (lhs.x != rhs.x) ? (lhs.x < rhs.x) : (lhs.y < rhs.y);
    }

    auto static constexpr CompareYThenX(Vector const& lhs, Vector const& rhs) -> bool
    {
        return (lhs.y != rhs.y) ? (lhs.y < rhs.y) : (lhs.x < rhs.x);
    }

    auto static constexpr CalculateSemiPlane(Vector const& vector) -> int
    {
        if (vector.y != 0) {
            return 0 < vector.y ? -1 : +1;
        }

        if (vector.x != 0) {
            return 0 < vector.x ? -1 : +1;
        }

        return 0;
    }

    auto static constexpr CalculateSemiPlane(Vector direction, Vector vector)
    {
        auto const cross = CrossProduct(direction, vector);

        if (cross != 0) {
            return (0 < cross) ? -1 : +1;
        }

        auto const dot = DotProduct(direction, vector);

        if (dot != 0) {
            return (0 < dot) ? -1 : +1;
        }

        return 0;
    }

    auto static constexpr CreateComparatorByAngle(Vector const& center = Zero(), Vector const& direction = UnitAxisX())
    {
        return [=](Vector const& lhs, Vector const& rhs) {
            auto const lhsSemiPlane = CalculateSemiPlane(direction, lhs - center);
            auto const rhsSemiPlane = CalculateSemiPlane(direction, rhs - center);

            if (lhsSemiPlane != rhsSemiPlane) {
                return lhsSemiPlane < rhsSemiPlane;
            }

            auto const cross = CrossProduct(lhs, rhs);

            if (cross != 0) {
                return 0 < cross;
            }

            return SquaredLength(lhs) < SquaredLength(rhs);
        };
    }
};

}  // namespace Geometry

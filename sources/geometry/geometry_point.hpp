#pragma once

namespace Geometry {
/// @brief Generic `Point` structure
/// @tparam Coordinate
template <typename Coordinate>
struct Point
{
    Coordinate x = Coordinate{0};
    Coordinate y = Coordinate{0};

    constexpr Point(Coordinate x, Coordinate y)
        : x(x)
        , y(y)
    {
    }

    constexpr auto operator+=(Point const& rhs) -> Point&
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    constexpr friend auto operator+(Point lhs, Point const& rhs) -> Point
    {
        lhs += rhs;
        return lhs;
    }

    constexpr auto operator-=(Point const& rhs) -> Point&
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    friend constexpr auto operator-(Point lhs, Point const& rhs) -> Point
    {
        lhs -= rhs;
        return lhs;
    }

    constexpr auto operator*=(Coordinate const& rhs) -> Point&
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    friend constexpr auto operator*(Point lhs, Coordinate const& rhs) -> Point
    {
        lhs *= rhs;
        return lhs;
    }

    friend constexpr auto operator*(Coordinate const& lhs, Point rhs) -> Point
    {
        rhs *= lhs;
        return rhs;
    }

    constexpr auto operator/=(Coordinate const& rhs) -> Point&
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    friend constexpr auto operator/(Point lhs, Coordinate const& rhs) -> Point
    {
        lhs /= rhs;
        return lhs;
    }

    friend constexpr auto operator==(Point const& lhs, Point const& rhs) -> bool
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    friend constexpr auto operator<(Point const& lhs, Point const& rhs) -> bool
    {
        return (lhs.x == rhs.x) ? (lhs.y < rhs.y) : (lhs.x < rhs.x);
    }
};

}  // namespace Geometry

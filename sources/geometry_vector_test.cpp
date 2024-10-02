
#include "geometry_vector.hpp"

#include <gtest/gtest.h>

template struct Geometry::Vector<int>;

TEST(GeometryVector, CheckVectorConstructor)
{
    auto const vector = Geometry::Vector<int>(1, 2);
    EXPECT_EQ(vector.x, 1);
    EXPECT_EQ(vector.y, 2);
}

TEST(GeometryVector, CheckVectorEqual)
{
    auto const lhs = Geometry::Vector<int>{1, 1};
    auto const rhs = Geometry::Vector<int>{1, 1};
    EXPECT_EQ(lhs, rhs);
}

TEST(GeometryVector, CheckLessDifferentX)
{
    auto const lhs = Geometry::Vector<int>{1, 1};
    auto const rhs = Geometry::Vector<int>{2, 0};
    EXPECT_TRUE(lhs < rhs);
    EXPECT_TRUE(!(rhs < lhs));
}

TEST(GeometryVector, CheckLessSameX)
{
    auto const lhs = Geometry::Vector<int>{1, 0};
    auto const rhs = Geometry::Vector<int>{1, 1};
    EXPECT_TRUE(lhs < rhs);
    EXPECT_TRUE(!(rhs < lhs));
}

TEST(GeometryVector, CheckVectorAdd)
{
    auto const lhs   = Geometry::Vector<int>{1, 1};
    auto const rhs   = Geometry::Vector<int>{1, 1};
    auto const added = Geometry::Vector<int>{2, 2};
    EXPECT_EQ(lhs + rhs, added);
}

TEST(GeometryVector, CheckVectorSubstract)
{
    auto const lhs         = Geometry::Vector<int>{1, 1};
    auto const rhs         = Geometry::Vector<int>{1, 1};
    auto const substracted = Geometry::Vector<int>{0, 0};
    EXPECT_EQ(lhs - rhs, substracted);
}

TEST(GeometryVector, CheckVectorScaleUp)
{
    auto const lhs    = Geometry::Vector<int>{1, 2};
    auto const rhs    = 2;
    auto const scaled = Geometry::Vector<int>{2, 4};
    EXPECT_EQ(lhs * rhs, scaled);
    EXPECT_EQ(rhs * lhs, scaled);
}

TEST(GeometryVector, CheckVectorScaleDown)
{
    auto const lhs    = Geometry::Vector<int>{2, 4};
    auto const rhs    = 2;
    auto const scaled = Geometry::Vector<int>{1, 2};
    EXPECT_EQ(lhs / rhs, scaled);
}

TEST(GeometryVector, CheckDotProduct)
{
    auto const lhs = Geometry::Vector<int>{+1, +1};
    auto const rhs = Geometry::Vector<int>{+1, -1};
    auto const dot = Geometry::Vector<int>::DotProduct(lhs, rhs);
    EXPECT_EQ(dot, 0);
}

TEST(GeometryVector, CheckCrossProduct)
{
    auto const lhs   = Geometry::Vector<int>{1, 2};
    auto const rhs   = Geometry::Vector<int>{2, 4};
    auto const cross = Geometry::Vector<int>::CrossProduct(lhs, rhs);
    EXPECT_EQ(cross, 0);
}

TEST(GeometryVector, CheckVectorComparatorXY)
{
    using Vector = Geometry::Vector<int>;

    // compare points with different x
    EXPECT_TRUE(Vector::CompareXThenY(Vector{0, 0}, Vector{1, -1}));
    EXPECT_FALSE(Vector::CompareXThenY(Vector{1, -1}, Vector{0, 0}));

    // compare points with same x
    EXPECT_TRUE(Vector::CompareXThenY(Vector{0, 0}, Vector{0, +1}));
    EXPECT_FALSE(Vector::CompareXThenY(Vector{0, +1}, Vector{0, 0}));

    // compare equal points
    EXPECT_FALSE(Vector::CompareXThenY(Vector{0, 0}, Vector{0, 0}));
}

TEST(GeometryVector, CheckVectorComparatorYX)
{
    using Vector = Geometry::Vector<int>;

    // compare points with different y
    EXPECT_TRUE(Vector::CompareYThenX(Vector{0, 0}, Vector{-1, +1}));
    EXPECT_FALSE(Vector::CompareYThenX(Vector{-1, +1}, Vector{0, 0}));

    // compare points with same y
    EXPECT_TRUE(Vector::CompareYThenX(Vector{0, 0}, Vector{+1, 0}));
    EXPECT_FALSE(Vector::CompareYThenX(Vector{+1, 0}, Vector{0, 0}));

    // compare equal points
    EXPECT_FALSE(Vector::CompareYThenX(Vector{0, 0}, Vector{0, 0}));
}

TEST(GeometryVector, CalculateSemiPlane)
{
    using Vector = Geometry::Vector<int>;

    EXPECT_EQ(Vector::CalculateSemiPlane(Vector{0, 0}), 0);

    EXPECT_EQ(Vector::CalculateSemiPlane(Vector{+1, 0}), -1);

    EXPECT_EQ(Vector::CalculateSemiPlane(Vector{0, +1}), -1);

    EXPECT_EQ(Vector::CalculateSemiPlane(Vector{-1, 0}), +1);

    EXPECT_EQ(Vector::CalculateSemiPlane(Vector{0, -1}), +1);
}

TEST(GeometryVector, DefaultComparatorAngle)
{
    using Vector = Geometry::Vector<int>;

    auto const comparator_angle = Vector::CreateComparatorByAngle();

    // compare points on different half planes
    EXPECT_TRUE(comparator_angle(Vector{0, 1}, Vector{0, -1}));
    EXPECT_FALSE(comparator_angle(Vector{0, -1}, Vector{0, 1}));

    // compare points on the same half plane
    EXPECT_TRUE(comparator_angle(Vector{1, 0}, Vector{0, 1}));
    EXPECT_FALSE(comparator_angle(Vector{0, 1}, Vector{1, 0}));
    EXPECT_TRUE(comparator_angle(Vector{-1, 0}, Vector{0, -1}));
    EXPECT_FALSE(comparator_angle(Vector{0, -1}, Vector{-1, 0}));

    // compare collinear points with the "origin" of the comparator
    EXPECT_TRUE(comparator_angle(Vector{0, 1}, Vector{0, 2}));
    EXPECT_FALSE(comparator_angle(Vector{0, 2}, Vector{0, 1}));
}

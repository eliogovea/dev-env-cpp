#include <gtest/gtest.h>

#include "geometry_value_sign.hpp"

template auto Geometry::GetValueSign(int&& value) -> Geometry::ValueSign;

TEST(GeometryValue, CheckSign)
{
    EXPECT_EQ(Geometry::GetValueSign(-1), Geometry::ValueSign::Negative);
    EXPECT_EQ(Geometry::GetValueSign(0), Geometry::ValueSign::Zero);
    EXPECT_EQ(Geometry::GetValueSign(+1), Geometry::ValueSign::Positive);
}

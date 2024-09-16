#include <gtest/gtest.h>
#include <fmt/core.h>

#include "math/linalg/point.hpp"
#include "math/linalg/fmt.hpp"

TEST(point, instantiation_indexing)
{
    using namespace math;
    point1f pt1f0{ -2 };
    ASSERT_FLOAT_EQ(pt1f0[0], -2);

    point2f pt2f0{ 1.5, 2.4 };
    ASSERT_FLOAT_EQ(pt2f0[0], 1.5);
    ASSERT_FLOAT_EQ(pt2f0[1], 2.4);

    point3f pt3f0{ 2.0, 3.0, 1.2 };
    ASSERT_FLOAT_EQ(pt3f0[0], 2.0);
    ASSERT_FLOAT_EQ(pt3f0[1], 3.0);
    ASSERT_FLOAT_EQ(pt3f0[2], 1.2);

    point4f pt4f0{ 0, 1, 2, 3 };
    ASSERT_FLOAT_EQ(pt4f0[0], 0);
    ASSERT_FLOAT_EQ(pt4f0[1], 1);
    ASSERT_FLOAT_EQ(pt4f0[2], 2);
    ASSERT_FLOAT_EQ(pt4f0[3], 3);

    point4f pt4f1;
    ASSERT_FLOAT_EQ(pt4f1[0], 0);
    ASSERT_FLOAT_EQ(pt4f1[1], 0);
    ASSERT_FLOAT_EQ(pt4f1[2], 0);
    ASSERT_FLOAT_EQ(pt4f1[3], 0);
}

TEST(point, operations)
{
    using namespace math;

    point4f pt0{ 2, 3, 4, 3 };
    vec4f vec0{ 2, 2, 2, 2 };

    auto add = pt0 + vec0;
    ASSERT_FLOAT_EQ(add[0], 4);
    ASSERT_FLOAT_EQ(add[1], 5);
    ASSERT_FLOAT_EQ(add[2], 6);
    ASSERT_FLOAT_EQ(add[3], 5);

    auto sub = pt0 - vec0;
    ASSERT_FLOAT_EQ(sub[0], 0);
    ASSERT_FLOAT_EQ(sub[1], 1);
    ASSERT_FLOAT_EQ(sub[2], 2);
    ASSERT_FLOAT_EQ(sub[3], 1);

    auto diff = add - pt0;
    ASSERT_FLOAT_EQ(diff[0], vec0[0]);
    ASSERT_FLOAT_EQ(diff[1], vec0[1]);
    ASSERT_FLOAT_EQ(diff[2], vec0[2]);
    ASSERT_FLOAT_EQ(diff[3], vec0[3]);

}
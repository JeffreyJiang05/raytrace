#include <gtest/gtest.h>
#include <fmt/format.h>

#include <bitset>

#include "math/floats.hpp"

using namespace math::utility;

TEST(float_utility, NaN_test)
{
    ASSERT_FALSE(is_nan(0));
    ASSERT_FALSE(is_nan(0.f));
    ASSERT_FALSE(is_nan(0.));
    ASSERT_FALSE(is_nan(1.f/0));
    ASSERT_FALSE(is_nan(-1./0));
    ASSERT_TRUE(is_nan(0./0));
    ASSERT_TRUE(is_nan(0.f/0));
}

TEST(float_utility, inf_test)
{
    ASSERT_FALSE(is_inf(0));
    ASSERT_FALSE(is_inf(0.f));
    ASSERT_FALSE(is_inf(0.));
    ASSERT_TRUE(is_inf(1.f/0));
    ASSERT_TRUE(is_inf(-1./0));
    ASSERT_FALSE(is_inf(0.f/0));
    ASSERT_FALSE(is_inf(0./0));
}

TEST(float_utility, fma_test)
{
    ASSERT_EQ(fma(3, 5, 3), 18);
    ASSERT_NE(fma(3, 5, 3), 14);
    ASSERT_FLOAT_EQ(fma(4.f, 2.1f, 4.2f), 12.6f);
    ASSERT_DOUBLE_EQ(fma(4.0, 2.1, 4.2), 12.6);
}
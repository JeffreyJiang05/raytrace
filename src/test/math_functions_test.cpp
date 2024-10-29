#include <gtest/gtest.h>
#include <fmt/core.h>

#include "math/functions.hpp"
#include <cmath>

TEST(math_functions, maclaurin_series)
{
    constexpr std::floating_point auto PI = math::pi<double>;
    constexpr std::floating_point auto TAU = math::tau<double>;
    constexpr auto PRECISION = 0.000000001;

    EXPECT_NEAR(math::sin(PI / 3), sin(PI / 3), PRECISION);
    EXPECT_NEAR(math::cos(PI / 3), cos(PI / 3), PRECISION);
    EXPECT_NEAR(math::tan(PI / 3), tan(PI / 3), PRECISION);

    EXPECT_NEAR(math::sin(PI / 6), sin(PI / 6), PRECISION);
    EXPECT_NEAR(math::cos(PI / 6), cos(PI / 6), PRECISION);
    EXPECT_NEAR(math::tan(PI / 6), tan(PI / 6), PRECISION);

    EXPECT_NEAR(math::sin(TAU / 3), sin(TAU / 3), PRECISION);
    EXPECT_NEAR(math::cos(TAU / 3), cos(TAU / 3), PRECISION);
    EXPECT_NEAR(math::tan(TAU / 3), tan(TAU / 3), PRECISION);

    EXPECT_NEAR(math::csc(PI / 3), 1 / sin(PI / 3), PRECISION);
    EXPECT_NEAR(math::sec(PI / 3), 1 / cos(PI / 3), PRECISION);
    EXPECT_NEAR(math::cot(PI / 3), 1 / tan(PI / 3), PRECISION);

    EXPECT_NEAR(math::csc(PI / 4), 1 / sin(PI / 4), PRECISION);
    EXPECT_NEAR(math::sec(PI / 4), 1 / cos(PI / 4), PRECISION);
    EXPECT_NEAR(math::cot(PI / 4), 1 / tan(PI / 4), PRECISION);

    EXPECT_NEAR(math::arcsin(0.5), asin(0.5), PRECISION);
    EXPECT_NEAR(math::arccos(0.5), acos(0.5), PRECISION);
    EXPECT_NEAR(math::arctan(0.5), atan(0.5), PRECISION);

    EXPECT_NEAR(math::arcsin(0.25), asin(0.25), PRECISION);
    EXPECT_NEAR(math::arccos(0.25), acos(0.25), PRECISION);
    EXPECT_NEAR(math::arctan(0.25), atan(0.25), PRECISION);

}


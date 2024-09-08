#include <gtest/gtest.h>
#include <fmt/core.h>

#include "math/linalg/vec.hpp"
#include "math/linalg/fmt.hpp"

TEST(vector, instantiation)
{
    using namespace math;
    vector v0{ 10, 2 };
    vector v1{ 10.f, 2.f };
    vector v2{ 2., 2. };
    vector v3{ v0, v0 };

    EXPECT_EQ(v0, v1);
    EXPECT_EQ(v2, v0.yy);
    EXPECT_NE(v2, v0.xx);
    EXPECT_EQ(v3, (vector<float, 4>{ 10.f, 2.f, 10.f, 2.f }));
}

TEST(vector, vec_function)
{
    using namespace math;
    vector v0{ 1, 2, 3 };
    vector v1{ 2.0, 3.0, 4.5 };
    vector v2{ 1.f, 2.f, 3.f };

    EXPECT_EQ(dot(v0, v1), 2. + 2 * 3. + 3 * 4.5);
    EXPECT_EQ(cross(v0, v1), (vector{ 0., 1.5, -1. }));
    EXPECT_EQ(cross(v1, v0), (vector{ 0., -1.5, 1. }));
    EXPECT_EQ(magnitude(v1), sqrt(2.0 * 2.0 + 3.0 * 3.0 + 4.5 * 4.5));
    EXPECT_EQ(normalize<double>(v1), v1 / magnitude(v1));
    EXPECT_EQ(lerp(vec_cast<double>(v0), v1, 0.5), (vector{ 1.5, 2.5, 3.75 }));
    EXPECT_EQ(fma(v0, v0, v0), (vector{ 2, 6, 12 }));
    EXPECT_EQ(fma(v1, v1, v1), (vector{ 6., 12., 4.5 * 5.5 }));
    EXPECT_EQ(fma(v2, v2, v2), (vector{ 2, 6, 12 }));

    auto [a, b] = coordinate_system(normalize<double>(v0));
    fmt::println("{} {} {}", dot(a, b), dot(b, v0), dot(a, v0));
}


#ifndef GPU_RAYTRACE_SAMPLING_INL
#define GPU_RAYTRACE_SAMPLING_INL

#include "math/sampling.hpp"
#include "math/floats.hpp"

namespace math
{

    template<std::floating_point T>
    constexpr CPU_GPU int sample_discrete(std::span<const T> weights, T u)
    {
        if (weights.empty()) return -1;

        T weight_sum = 0;
        for (auto&& weight : weights) weight_sum += weight;

        T uu = weight_sum * u;
        if (uu == weight_sum) uu = math::next_floating_down(uu);

        T sum = 0;
        int i = 0;
        while (sum + weights[i] <= uu) sum += weights[i++];

        return i;
    }

    template<std::floating_point T>
    constexpr CPU_GPU T linear_pdf(T u, T a, T b)
    {
        if (u < 0 || u > 1) return 0;
        return 2 * std::lerp(a, b, x) / (a + b);
    }

    template<std::floating_point T>
    constexpr CPU_GPU T sample_linear(T u, T a, T b)
    {
        if (u == 0 && a == 0) return 0;
        T uu = u * (a + b) / (a + std::sqrt(std::lerp(u, a * a, b * b)));
        return std::min(uu, math::one_minus_epsilon<T>);
    }

    template<std::floating_point T>
    constexpr CPU_GPU T invert_sample_linear(T u, T a, T b)
    {
        return u * (a * (2 - u) + b * u) / (a + b);
    }

    template<std::floating_point T>
    constexpr CPU_GPU point<T, 2> sample_disk(point<T, 2> u)
    {
        return {
            u[0] >= 0 && u[0] < 1 ? math::sqrt(u[0]) : 0,
            u[1] >= 0 && u[1] < 1 ? 2 * math::pi<T> * u[1] : 0
        };
    }

    template<std::floating_point T>
    constexpr CPU_GPU point<T, 3> sample_sphere_surface(point<T, 2> u)
    {
        return {
            1,
            u[0] >= 0 && u[0] < 1 ? 2 * math::pi<T> * u[0] : 0,
            u[1] >= 0 && u[1] < 1 ? math::arccos(1 - 2 * u[1]) : 0
        };
    }

    template<std::floating_point T>
    constexpr CPU_GPU point<T, 3> sample_sphere(point<T, 3> u)
    {
        return {
            u[0] >= 0 && u[1] < 1 ? math::pow(u[0], static_cast<T>(1) / 3) : 0,
            u[1] >= 0 && u[1] < 1 ? 2 * math::pi<T> * u[1] : 0,
            u[2] >= 0 && u[1] < 1 ? math::arccos(1 - 2 * u[2]) : 0
        };
    }

}

#endif //GPU_RAYTRACE_SAMPLING_INL

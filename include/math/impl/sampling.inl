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
    constexpr CPU_GPU T linear_pdf(T x, T a, T b)
    {
        if (x < 0 || x > 1) return 0;
        return 2 * std::lerp(a, b, x) / (a + b);
    }

    template<std::floating_point T>
    constexpr CPU_GPU T sample_linear(T u, T a, T b)
    {
        if (u == 0 && a == 0) return 0;
        T x = u * (a + b) / (a + std::sqrt(std::lerp(u, a * a, b * b)));
        return std::min(x, math::one_minus_epsilon<T>);
    }

    template<std::floating_point T>
    constexpr CPU_GPU T invert_sample_linear(T x, T a, T b)
    {
        return x * (a * (2 - x) + b * x) / (a + b);
    }

}

#endif //GPU_RAYTRACE_SAMPLING_INL

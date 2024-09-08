#ifndef GPU_RAYTRACE_SAMPLING_HPP
#define GPU_RAYTRACE_SAMPLING_HPP

#include <concepts>
#include <span>

#include "gpu/gpu.hpp"

namespace math::sampling
{

    /**
     * performs a discrete sampling based on a provided PMF via the inversion method.
     * The PMF does not need to be normalized.
     * @tparam T floating point type used in calculations
     * @param weights weights that form PMF to sample from
     * @param u uniform random value in interval [0,1)
     * @return index of weight sampled if successful. -1 on empty PMF.
     */
    template<std::floating_point T>
    constexpr CPU_GPU int sample_discrete(std::span<const T> weights, T u);

    /**
     * calculates the value of value of the probability density function
     * on a linear interpolation function (a+x(b-a)) at the value of x.
     * @tparam T floating point type
     * @param x variable from [0,1]
     * @param a lower bound of the interpolation
     * @param b upper bound of the interpolation
     * @return probability at x
     */
    template<std::floating_point T>
    constexpr CPU_GPU T linear_pdf(T x, T a, T b);

    /**
     * performs a continuous sampling based on a linear probability density function
     * between values the values a and b.
     * @tparam T floating point type
     * @param u uniform random value in interval [0,1)
     * @param a lower bound
     * @param b upper bound
     * @return linearly sampled value between a and b
     */
    template<std::floating_point T>
    constexpr CPU_GPU T sample_linear(T u, T a, T b);

    /**
     * inverts a linear sampling between the values a and b.
     * @tparam T floating point type
     * @param x sampled value in interval [a,b]
     * @param a lower bound
     * @param b upper bound
     * @return the value mapping to x in the range [0,1)
     */
    template<std::floating_point T>
    constexpr CPU_GPU T invert_sample_linear(T x, T a, T b);

}

#include "impl/sampling.inl"

#endif //GPU_RAYTRACE_SAMPLING_HPP

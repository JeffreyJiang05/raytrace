#ifndef GPU_RAYTRACE_FUNCTIONS_INL
#define GPU_RAYTRACE_FUNCTIONS_INL

#include "math/functions.hpp"

#include <cmath>

namespace math
{

    template<std::size_t Iterations, typename T>
    constexpr CPU_GPU T sqrt(T value)
    {
        if (std::is_constant_evaluated())
        {
            T n = 1;
            for (std::size_t i = 0; i < Iterations; ++i)
            {
                n = (n + value / n) / 2;
            }
            return n;
        }
        else return std::sqrt(value);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T sin(T theta)
    {
        if (std::is_constant_evaluated())
        {
            T n = 0;
            T theta_squared = theta * theta; T delta = theta;
            for (std::size_t i = 0; i < Iterations; ++i)
            {
                n += delta;
                delta *= -theta_squared / (6 + i * (10 + i * 4));
            }
            return n;
        }
        else return std::sin(theta);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T cos(T theta)
    {
        if (std::is_constant_evaluated())
        {
            T n = 0;
            T theta_squared = theta * theta; T delta = 1;
            for (std::size_t i = 0; i < Iterations; ++i)
            {
                n += delta;
                delta *= -theta_squared / (2 + i * (6 + 4 * i));
            }
            return n;
        }
        else return std::cos(theta);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T tan(T theta)
    {
        if (std::is_constant_evaluated()) return sin<Iterations>(theta) / cos<Iterations>(theta);
        else return std::tan(theta);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T csc(T theta)
    {
        return 1 / sin<Iterations>(theta);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T sec(T theta)
    {
        return 1 / cos<Iterations>(theta);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T cot(T theta)
    {
        return 1 / tan<Iterations>(theta);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T arcsin(T ratio)
    {
        if (std::is_constant_evaluated())
        {
            T n = 0;
            T ratio_squared = ratio * ratio; T delta = ratio;
            for (std::size_t i = 0; i < Iterations; ++i)
            {
                n += delta;
                delta *= ratio_squared * (1 + i * (5 + i * (8 + i * 4))) / (6 + i * (16 + i * (14 + i * 4)));
            }
            return n;
        }
        else return std::asin(ratio);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T arccos(T ratio)
    {
        if (std::is_constant_evaluated()) return pi<T> / 2 - arcsin<Iterations>(ratio);
        else return std::acos(ratio);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T arctan(T ratio)
    {
        if (std::is_constant_evaluated())
        {
            T n = 0;
            T ratio_squared = ratio * ratio; T delta = ratio;
            for (std::size_t i = 0; i < Iterations; ++i)
            {
                n += delta;
                delta *= -ratio_squared * (2 * i + 1) / (2 * i + 3);
            }
            return n;
        }
        else return std::atan(ratio);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T arccsc(T ratio)
    {
        return arcsin<Iterations>(1 / ratio);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T arcsec(T ratio)
    {
        return arccos<Iterations>(1 / ratio);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T arccot(T ratio)
    {
        return arctan<Iterations>(1 / ratio);
    }

    template<arithmetic T>
    constexpr CPU_GPU T copysign(T value, T sign)
    {
        if (std::is_constant_evaluated())
        {
            if ((sign < 0) ^ (value < 0)) return -value;
            return value;
        }
        return std::copysign(value, sign);
    }

}

#endif //GPU_RAYTRACE_FUNCTIONS_INL

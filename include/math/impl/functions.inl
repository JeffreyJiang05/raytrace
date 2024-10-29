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

    // x in (0, 2]
    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T bounded_log(T x)
    {
        T n = 0;
        T diff = x - 1, delta = diff;
        for (std::size_t i = 1; i < Iterations; ++i)
        {
            n += delta;
            delta *= -diff * i / (i + 1);
        }
        return n;
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T log(T x)
    {
        if (std::is_constant_evaluated())
        {
            if (x <= 0) return INFINITY;
            if (x <= 2) return bounded_log<Iterations>(x);
            else
            {
                auto exponent = exponent_bits(x) - (1l << (exponent_size<T> - 1)) + 1;
                return bounded_log<Iterations>(x / (1l << exponent)) + exponent * ln2<T>;
            }
        }
        return std::log(x);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T exp(T x)
    {
        if (std::is_constant_evaluated())
        {
            T val = x < 0 ? -x : x;
            T n = 0;
            T delta = 1;
            for (std::size_t i = 0; i < Iterations; ++i)
            {
                n += delta;
                delta *= val / (i + 1);
            }
            return x < 0 ? 1 / n : n;
        }
        return std::exp(x);
    }

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T pow(T a, T b)
    {
        if (std::is_constant_evaluated())
        {
            if (b < 0)
            {
                return 1 / exp<Iterations>(-b * log<Iterations>(a));
            }
            else return exp<Iterations>(b * log<Iterations>(a));
        }
        return std::pow(a, b);
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

    template<std::size_t Iterations, std::floating_point T>
    constexpr CPU_GPU T arctan2(T y, T x)
    {
        if (x > 0) return arctan<Iterations>(y / x);
        else if (x < 0 && y >= 0) return arctan<Iterations>(y / x) + pi<T>;
        else if (x < 0 && y < 0) return arctan<Iterations>(y / x) - pi<T>;
        else if (x == 0 && y > 0) return pi<T> / 2;
        else if (x == 0 && y < 0) return -pi<T> / 2;
        else return static_cast<T>(NAN);
    }

}

#endif //GPU_RAYTRACE_FUNCTIONS_INL

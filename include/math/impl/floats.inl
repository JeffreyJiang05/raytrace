#ifndef GPU_RAYTRACE_FLOATS_INL
#define GPU_RAYTRACE_FLOATS_INL

#include "math/floats.hpp"

#include <cmath>
#include <numeric>

namespace math
{

    template<std::floating_point T>
    constexpr CPU_GPU bool is_nan(T a)
    {
        // NaN is unordered meaning it is the only value such that a != a
        return a != a;
    }

    template<std::integral T>
    constexpr CPU_GPU bool is_nan(T a)
    {
        return false;
    }

    template<std::floating_point T>
    constexpr CPU_GPU bool is_inf(T a)
    {
        return a == INFINITY || a == -INFINITY;
    }

    template<std::integral T>
    constexpr CPU_GPU bool is_inf(T a)
    {
        return false;
    }

    template<std::floating_point T>
    constexpr CPU_GPU T fma(T a, T b, T c)
    {
        if (std::is_constant_evaluated()) return a * b + c;
        else return std::fma(a, b, c);
    }

    template<std::integral T>
    constexpr CPU_GPU T fma(T a, T b, T c)
    {
        return a * b + c;
    }

    template<typename T> requires(std::is_arithmetic_v<T>)
    constexpr CPU_GPU _impl::to_uint_t<T> to_bits(T value)
    {
        return std::bit_cast<_impl::to_uint_t<T>>(value);
    }

    template<typename T> requires(std::is_arithmetic_v<T>)
    constexpr CPU_GPU _impl::to_float_t<T> to_floating(T value)
    {
        return std::bit_cast<_impl::to_float_t<T>>(value);
    }

    template<std::floating_point T>
    constexpr CPU_GPU _impl::to_uint_t<T> mantissa_bits(T value)
    {
        using ret = _impl::to_uint_t<T>;
        constexpr auto mask = (1ul << (mantissa_size<T> + 1)) - 1;
        return static_cast<ret>( to_bits(value) & mask );
    }

    template<std::floating_point T>
    constexpr CPU_GPU _impl::to_uint_t<T> exponent_bits(T value)
    {
        using ret = _impl::to_uint_t<T>;
        constexpr auto mask = (1ul << (exponent_size<T> + 1)) - 1;
        return static_cast<ret>( to_bits(value) >> mantissa_size<T> & mask );
    }

    template<typename T> requires (std::is_arithmetic_v<T>)
    constexpr CPU_GPU bool sign_bit(T value)
    {
        return value < 0;
    }

    template<std::floating_point T>
    constexpr CPU_GPU T next_floating_up(T value)
    {
        if (is_inf(value) && value > 0) return value;
        if (-value == 0) value = static_cast<T>(0);

        auto bits = to_bits(value);
        if (value >= 0) ++bits;
        else --bits;
        return to_floating(bits);
    }

    template<std::floating_point T>
    constexpr CPU_GPU T next_floating_down(T value)
    {
        if (is_inf(value) && value < 0) return value;
        if (value == 0) value = -static_cast<T>(0);

        auto bits = to_bits(value);
        if (value <= 0) ++bits;
        else --bits;
        return to_floating(bits);
    }

}

#endif //GPU_RAYTRACE_FLOATS_INL

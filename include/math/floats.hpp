#ifndef GPU_RAYTRACE_FLOATS_HPP
#define GPU_RAYTRACE_FLOATS_HPP

#include <concepts>
#include <cstdint>

#include "gpu/gpu.hpp"

namespace math
{

    template<std::floating_point T> constexpr inline T one_minus_epsilon;
    template<> constexpr CONSTANT inline float one_minus_epsilon<float> = 0x1.fffffep-1;
    template<> constexpr CONSTANT inline double one_minus_epsilon<double> = 0x1.fffffffffffffp-1;

    /**
     * generic function to test if a floating point value is "not a number" (NaN)
     * @tparam T floating point type
     * @param a
     * @return true if `a` is NaN. false otherwise.
     */
    template<std::floating_point T>
    constexpr CPU_GPU bool is_nan(T a);

    /**
     * generic function to test if a floating point value is "not a number" (NaN)
     * @tparam T integral type
     * @param a
     * @return false for all integral values.
     */
    template<std::integral T>
    constexpr CPU_GPU bool is_nan(T a);

    /**
     * generic function to test if a floating point value is infinite
     * @tparam T floating point type
     * @param a
     * @return true if `a` is inf. false otherwise.
     */
    template<std::floating_point T>
    constexpr CPU_GPU bool is_inf(T a);

    /**
     * generic function to test if a floating point value is infinite
     * @tparam T integral type
     * @param a
     * @return false for all integral values.
     */
    template<std::integral T>
    constexpr CPU_GPU bool is_inf(T a);

    /**
     * generic fused multiply and add function utilizing built-in intrinsics if possible
     * @tparam T floating point type
     * @param a
     * @param b
     * @param c
     * @return the value a * b + c
     */
    template<std::floating_point T>
    constexpr CPU_GPU T fma(T a, T b, T c);

    /**
     * generic fused multiply and add function utilizing built-in intrinsics if possible
     * @tparam T integral type
     * @param a
     * @param b
     * @param c
     * @return the value a * b + c
     */
    template<std::integral T>
    constexpr CPU_GPU T fma(T a, T b, T c);

    namespace _impl
    {
        template<std::size_t N> struct bit_cast_conv;
        template<> struct bit_cast_conv<4>
        {
            using uint_type = uint32_t;
            using float_type = float;
        };
        template<> struct bit_cast_conv<8>
        {
            using uint_type = uint64_t;
            using float_type = double;
        };
        template<typename From> using to_uint_t = bit_cast_conv<sizeof(From)>::uint_type;
        template<typename From> using to_float_t = bit_cast_conv<sizeof(From)>::float_type;
    }

    /**
     * generic function to cast a value to their bit representations as an unsigned integer.
     * @tparam T arithmetic type
     * @param value
     * @return unsigned integral representation of value
     */
    template<typename T> requires(std::is_arithmetic_v<T>)
    constexpr CPU_GPU _impl::to_uint_t<T> to_bits(T value);

    /**
     * generic function to cast a value from their bits as an unsigned integer to a floating point.
     * @tparam T arithmetic type
     * @param value
     * @return floating point representation of value
     */
    template<typename T> requires(std::is_arithmetic_v<T>)
    constexpr CPU_GPU _impl::to_float_t<T> to_floating(T value);

    namespace _impl
    {
        template<std::size_t N> struct float_point_helper;
        template<> struct float_point_helper<4>
        {
            constexpr static uint8_t mantissa = 23;
            constexpr static uint8_t exponent = 8;
            constexpr static uint8_t sign = 1;
        };
        template<> struct float_point_helper<8>
        {
            constexpr static uint8_t mantissa = 52;
            constexpr static uint8_t exponent = 11;
            constexpr static uint8_t sign = 1;
        };
    }

    /**
     * data containing floating point format;
     * @tparam T floating point type
     */
    template<std::floating_point T>
    struct floating_point_format : public _impl::float_point_helper<sizeof(T)> {};

    template<std::floating_point T>
    constexpr inline uint8_t mantissa_size = floating_point_format<T>::mantissa;

    template<std::floating_point T>
    constexpr inline uint8_t exponent_size = floating_point_format<T>::exponent;

    /**
     * generic function to get the mantissa of a floating point value.
     * @tparam T floating point type
     * @param value
     * @return mantissa bits of value
     */
    template<std::floating_point T>
    constexpr CPU_GPU _impl::to_uint_t<T> mantissa_bits(T value);

    /**
     * generic function to get the exponent bits of a floating point value.
     * @tparam T floating point type
     * @param value
     * @return exponent bits of value
     */
    template<std::floating_point T>
    constexpr CPU_GPU _impl::to_uint_t<T> exponent_bits(T value);

    /**
     * generic function to get the sign bit of a value
     * @tparam T arithmetic type
     * @param value value to get the sign bit of
     * @return sign bit of value
     */
    template<typename T> requires (std::is_arithmetic_v<T>)
    constexpr CPU_GPU bool sign_bit(T value);

    /**
     * generic function to get the next floating point value above the provided value
     * @tparam T floating point type
     * @param value
     * @return next floating point value above value
     */
    template<std::floating_point T>
    constexpr CPU_GPU T next_floating_up(T value);

    /**
     * generic function to get the next floating point value below the provided value
     * @tparam T floating point type
     * @param value
     * @return next floating point value below value
     */
    template<std::floating_point T>
    constexpr CPU_GPU T next_floating_down(T value);

}

#include "impl/floats.inl"

#endif //GPU_RAYTRACE_FLOATS_HPP

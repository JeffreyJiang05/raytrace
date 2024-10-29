#ifndef GPU_RAYTRACE_FUNCTIONS_HPP
#define GPU_RAYTRACE_FUNCTIONS_HPP

#include <cstddef>
#include <concepts>

#include "floats.hpp"

namespace math
{

    template<typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    template<std::floating_point T> constexpr inline T pi;
    template<> constexpr CONSTANT inline float pi<float> = 3.14159265358979323846f;
    template<> constexpr CONSTANT inline double pi<double> = 3.14159265358979323846;

    template<std::floating_point T> constexpr inline T ln2;
    template<> constexpr CONSTANT inline float ln2<float> = 0.69314718055994530941f;
    template<> constexpr CONSTANT inline double ln2<double> = 0.69314718055994530941;

    template<std::floating_point T> constexpr CONSTANT inline T tau = 2 * pi<T>;

    constexpr inline std::size_t MACLAURIN_DEFAULT_ITERATION = 50;

    template<std::size_t Iterations = 16, typename T>
    constexpr CPU_GPU T sqrt(T value);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T sin(T theta);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T cos(T theta);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T tan(T theta);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T csc(T theta);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T sec(T theta);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T cot(T theta);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T arcsin(T ratio);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T arccos(T ratio);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T arctan(T ratio);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T arccsc(T ratio);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T arcsec(T ratio);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T arccot(T ratio);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T log(T x);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T exp(T x);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T pow(T a, T b);

    template<arithmetic T>
    constexpr CPU_GPU T copysign(T value, T sign);

    template<std::size_t Iterations = MACLAURIN_DEFAULT_ITERATION, std::floating_point T>
    constexpr CPU_GPU T arctan2(T y, T x);

}

#include "impl/functions.inl"

#endif //GPU_RAYTRACE_FUNCTIONS_HPP

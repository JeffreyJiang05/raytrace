#ifndef GPU_RAYTRACE_VEC_BASE_INL
#define GPU_RAYTRACE_VEC_BASE_INL

#include "math/geometry/vec.hpp"

namespace math
{

    // SEE HEADER FILE
    /*template<typename Derived>
    CPU_GPU Derived& vector_base<Derived>::operator=(const Derived& cpy) requires std::is_copy_assignable_v<typename Derived::value_type>
    {
        if (this == &cpy) return *this;
        for (std::size_t i = 0; i < Derived::size; ++i) as_derived()[i] = cpy[i];
        return *this;
    }

    template<typename Derived>
    CPU_GPU Derived& vector_base<Derived>::operator=(Derived&& mv) requires std::is_move_assignable_v<typename Derived::value_type>
    {
        if (this == &mv) return *this;
        for (std::size_t i = 0; i < Derived::size; ++i) as_derived()[i] = std::move(mv[i]);
        return *this;
    }*/

    template<typename Derived>
    template<vector_like Other>
    CPU_GPU Derived& vector_base<Derived>::operator=(Other&& vec) requires requires() {
        requires Derived::value == Other::value;
        requires std::is_convertible_v<typename Other::value_type, typename Derived::value_type>;
    }
    {
        if (this == &vec) return *this;
        for (std::size_t i = 0; i < Derived::size; ++i) as_derived()[i] = vec[i];
        return *this;
    }

    template<typename Derived>
    template<vector_like Other>
    CPU_GPU Derived& vector_base<Derived>::operator+=(const Other& value) requires requires(typename Derived::value_type a, typename Other::value_type b)
    {
        requires Derived::size == Other::size;
        { a + b } -> std::convertible_to<typename Derived::value_type>;
    }
    {
        Derived& derived = as_derived();
        for (std::size_t i = 0; i < Derived::size; ++i) derived[i] = static_cast<Derived::value_type>(derived[i] + value[i]);
        return derived;
    }

    template<typename Derived>
    template<vector_like Other>
    CPU_GPU Derived& vector_base<Derived>::operator-=(const Derived& value) requires requires(typename Derived::value_type a, typename Other::value_type b)
    {
        requires Derived::size == Other::size;
        { a - b } -> std::convertible_to<typename Derived::value_type>;
    }
    {
        Derived& derived = as_derived();
        for (std::size_t i = 0; i < Derived::size; ++i) derived[i] = static_cast<Derived::value_type>(derived[i] - value[i]);
        return derived;
    }

    template<typename Derived>
    template<vector_like Other>
    CPU_GPU Derived& vector_base<Derived>::operator*=(const Derived& value) requires requires(typename Derived::value_type a, typename Other::value_type b)
    {
        requires Derived::size == Other::size;
        { a * b } -> std::convertible_to<typename Derived::value_type>;
    }
    {
        Derived& derived = as_derived();
        for (std::size_t i = 0; i < Derived::size; ++i) derived[i] = static_cast<Derived::value_type>(derived[i] * value[i]);
        return derived;
    }

    template<typename Derived>
    template<typename T>
    CPU_GPU Derived& vector_base<Derived>::operator*=(T value) requires requires(typename Derived::value_type a, T b) { { a * b } -> std::convertible_to<typename Derived::value_type>; }
    {
        Derived& derived = as_derived();
        for (std::size_t i = 0; i < Derived::size; ++i) derived[i] = static_cast<Derived::value_type>(derived[i] * value);
        return derived;
    }

    template<typename Derived>
    template<vector_like Other>
    CPU_GPU Derived& vector_base<Derived>::operator/=(const Derived& value) requires requires(typename Derived::value_type a, typename Other::value_type b)
    {
        requires Derived::size == Other::size;
        { a / b } -> std::convertible_to<typename Derived::value_type>;
    }
    {
        Derived& derived = as_derived();
        for (std::size_t i = 0; i < Derived::size; ++i) derived[i] = static_cast<Derived::value_type>(derived[i] / value[i]);
        return derived;
    }

    template<typename Derived>
    CPU_GPU Derived vector_base<Derived>::operator-() requires requires(typename Derived::value_type a) { { -a } -> std::same_as<typename Derived::value_type>; }
    {
        Derived& derived = as_derived();
        Derived ret;
        for (std::size_t i = 0; i < Derived::size; ++i) ret[i] = -derived[i];
        return ret;
    }

}

#endif //GPU_RAYTRACE_VEC_BASE_INL

#ifndef GPU_RAYTRACE_POINT_BASE_INL
#define GPU_RAYTRACE_POINT_BASE_INL

#include "math/linalg/point.hpp"

namespace math
{

    template<typename Derived>
    template<vector_like Vec>
    CPU_GPU Derived& point_base<Derived>::operator+=(const Vec& vec) requires requires(typename Derived::value_type a, typename Vec::value_type b) { a += b; }
    {
        for (std::size_t i = 0; i < Derived::size; ++i)
        {
            as_derived()[i] += vec[i];
        }
        return *this;
    }

    template<typename Derived>
    template<vector_like Vec>
    CPU_GPU Derived& point_base<Derived>::operator-=(const Vec& vec) requires requires(typename Derived::value_type a, typename Vec::value_type b) { a -= b; }
    {
        for (std::size_t i = 0; i < Derived::size; ++i)
        {
            as_derived()[i] -= vec[i];
        }
        return *this;
    }

    template<typename Derived>
    template<vector_like Vec>
    CPU_GPU Derived& point_base<Derived>::operator*=(const Vec& vec) requires requires(typename Derived::value_type a, typename Vec::value_type b) { a *= b; }
    {
        for (std::size_t i = 0; i < Derived::size; ++i)
        {
            as_derived()[i] *= vec[i];
        }
        return *this;
    }

    template<typename Derived>
    template<vector_like Vec>
    CPU_GPU Derived& point_base<Derived>::operator/=(const Vec& vec) requires requires(typename Derived::value_type a, typename Vec::value_type b) { a /= b; }
    {
        for (std::size_t i = 0; i < Derived::size; ++i)
        {
            as_derived()[i] /= vec[i];
        }
        return *this;
    }

}

#endif //GPU_RAYTRACE_POINT_BASE_INL

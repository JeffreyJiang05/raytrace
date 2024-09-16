#ifndef GPU_RAYTRACE_SWIZZLE_VEC_INL
#define GPU_RAYTRACE_SWIZZLE_VEC_INL

#include "math/linalg/vec.hpp"

namespace math
{

    // swizzle_vector<X>

    template<typename T, std::size_t N, std::size_t X>
    constexpr CPU_GPU swizzle_vector<T, N, X>::swizzle_vector(const container<value_type, N>& data)
    : _components{ data } {}

    template<typename T, std::size_t N, std::size_t X>
    constexpr CPU_GPU T& swizzle_vector<T, N, X>::operator[](int)
    {
        return _components.buffer[X];
    }

    template<typename T, std::size_t N, std::size_t X>
    constexpr CPU_GPU T swizzle_vector<T, N, X>::operator[](int) const
    {
        return _components.buffer[X];
    }

    // swizzle_vector<X, Y>

    template<typename T, std::size_t N, std::size_t X, std::size_t Y>
    constexpr CPU_GPU swizzle_vector<T, N, X, Y>::swizzle_vector(const container<value_type, N>& data)
    : _components{ data } {}

    template<typename T, std::size_t N, std::size_t X, std::size_t Y>
    constexpr CPU_GPU T& swizzle_vector<T, N, X, Y>::operator[](int idx)
    {
        switch (idx)
        {
            case 0: return _components.buffer[X];
            default: return _components.buffer[Y];
        }
    }

    template<typename T, std::size_t N, std::size_t X, std::size_t Y>
    constexpr CPU_GPU T swizzle_vector<T, N, X, Y>::operator[](int idx) const
    {
        switch (idx)
        {
            case 0: return _components.buffer[X];
            default: return _components.buffer[Y];
        }
    }

    // swizzle_vector<X, Y, Z>

    template<typename T, std::size_t N, std::size_t X, std::size_t Y, std::size_t Z>
    constexpr CPU_GPU swizzle_vector<T, N, X, Y, Z>::swizzle_vector(const container<value_type, N>& data)
    : _components{ data } {}

    template<typename T, std::size_t N, std::size_t X, std::size_t Y, std::size_t Z>
    constexpr CPU_GPU T& swizzle_vector<T, N, X, Y, Z>::operator[](int idx)
    {
        switch (idx)
        {
            case 0: return _components.buffer[X];
            case 1: return _components.buffer[Y];
            default: return _components.buffer[Z];
        }
    }

    template<typename T, std::size_t N, std::size_t X, std::size_t Y, std::size_t Z>
    constexpr CPU_GPU T swizzle_vector<T, N, X, Y, Z>::operator[](int idx) const
    {
        switch (idx)
        {
            case 0: return _components.buffer[X];
            case 1: return _components.buffer[Y];
            default: return _components.buffer[Z];
        }
    }

    // swizzle_vector<X, Y, Z, W>

    template<typename T, std::size_t N, std::size_t X, std::size_t Y, std::size_t Z, std::size_t W>
    constexpr CPU_GPU swizzle_vector<T, N, X, Y, Z, W>::swizzle_vector(const container<value_type, N>& data)
    : _components{ data } {}

    template<typename T, std::size_t N, std::size_t X, std::size_t Y, std::size_t Z, std::size_t W>
    constexpr CPU_GPU T& swizzle_vector<T, N, X, Y, Z, W>::operator[](int idx)
    {
        switch (idx)
        {
            case 0: return _components.buffer[X];
            case 1: return _components.buffer[Y];
            case 2: return _components.buffer[Z];
            default: return _components.buffer[W];
        }
    }

    template<typename T, std::size_t N, std::size_t X, std::size_t Y, std::size_t Z, std::size_t W>
    constexpr CPU_GPU T swizzle_vector<T, N, X, Y, Z, W>::operator[](int idx) const
    {
        switch (idx)
        {
            case 0: return _components.buffer[X];
            case 1: return _components.buffer[Y];
            case 2: return _components.buffer[Z];
            default: return _components.buffer[W];
        }
    }

    // for constexpr swizzling
    template<typename T, std::size_t N, std::size_t... Swizzles> requires ((Swizzles < N) && ...)
    constexpr CPU_GPU auto swizzle(const vector<T, N>& vec, swizzle_tag<Swizzles...>) -> swizzle_vector<T, N, Swizzles...>
    {
        return swizzle_vector<T, N, Swizzles...>{ vec.data };
    }

}

#endif //GPU_RAYTRACE_SWIZZLE_VEC_INL

#ifndef GPU_RAYTRACE_NORMAL_INL
#define GPU_RAYTRACE_NORMAL_INL

#include "math/geometry/normal.hpp"

namespace math
{
    // normal<T, 1> implementations

    template<typename T>
    constexpr CPU_GPU normal<T, 1>::normal() requires std::is_default_constructible_v<value_type>
            : _data{{}}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 1>::normal(T x) : _data{{x}}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 1>::normal(const normal& cpy) requires std::is_copy_constructible_v<value_type>
            : _data{cpy.data}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 1>::normal(normal&& mv) requires std::is_move_constructible_v<value_type>
            : _data{std::move(mv.data)}
    {}

    template<typename T>
    CPU_GPU normal<T, 1>& normal<T, 1>::operator=(const normal& cpy) requires std::is_copy_assignable_v<value_type>
    {
        if (this != &cpy) {
            for (std::size_t i = 0; i < size; ++i) _data.buffer[i] = cpy[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU normal<T, 1>& normal<T, 1>::operator=(normal&& mv) requires std::is_move_assignable_v<value_type>
    {
        if (this != &mv) {
            for (std::size_t i = 0; i < size; ++i) _data.buffer[i] = std::move(mv[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU normal<T, 1>::operator value_type() const
    {
        return _data.buffer[0];
    }

    template<typename T>
    constexpr CPU_GPU T& normal<T, 1>::operator[](int idx)
    {
        return _data.buffer[idx];
    }

    template<typename T>
    constexpr CPU_GPU T normal<T, 1>::operator[](int idx) const
    {
        return _data.buffer[idx];
    }

    // normal<T, 2> implementations

    template<typename T>
    constexpr CPU_GPU normal<T, 2>::normal() requires std::is_default_constructible_v<value_type>
            : _data{{}}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 2>::normal(T x, T y) : _data{{x, y}}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 2>::normal(const normal& cpy) requires std::is_copy_constructible_v<value_type>
            : _data{cpy.data}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 2>::normal(normal&& mv) requires std::is_move_constructible_v<value_type>
            : _data{std::move(mv.data)}
    {}

    template<typename T>
    CPU_GPU normal<T, 2>& normal<T, 2>::operator=(const normal& cpy) requires std::is_copy_assignable_v<value_type>
    {
        if (this != &cpy) {
            for (std::size_t i = 0; i < size; ++i) _data.buffer[i] = cpy[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU normal<T, 2>& normal<T, 2>::operator=(normal&& mv) requires std::is_move_assignable_v<value_type>
    {
        if (this != &mv) {
            for (std::size_t i = 0; i < size; ++i) _data.buffer[i] = std::move(mv[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU T& normal<T, 2>::operator[](int idx)
    {
        return _data.buffer[idx];
    }

    template<typename T>
    constexpr CPU_GPU T normal<T, 2>::operator[](int idx) const
    {
        return _data.buffer[idx];
    }

    // normal<T, 3> implementations

    template<typename T>
    constexpr CPU_GPU normal<T, 3>::normal() requires std::is_default_constructible_v<value_type>
            : _data{{}}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 3>::normal(T x, T y, T z) : _data{{x, y, z}}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 3>::normal(const normal& cpy) requires std::is_copy_constructible_v<value_type>
            : _data{cpy.data}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 3>::normal(normal&& mv) requires std::is_move_constructible_v<value_type>
            : _data{std::move(mv.data)}
    {}

    template<typename T>
    CPU_GPU normal<T, 3>& normal<T, 3>::operator=(const normal& cpy) requires std::is_copy_assignable_v<value_type>
    {
        if (this != &cpy) {
            for (std::size_t i = 0; i < size; ++i) _data.buffer[i] = cpy[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU normal<T, 3>& normal<T, 3>::operator=(normal&& mv) requires std::is_move_assignable_v<value_type>
    {
        if (this != &mv) {
            for (std::size_t i = 0; i < size; ++i) _data.buffer[i] = std::move(mv[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU T& normal<T, 3>::operator[](int idx)
    {
        return _data.buffer[idx];
    }

    template<typename T>
    constexpr CPU_GPU T normal<T, 3>::operator[](int idx) const
    {
        return _data.buffer[idx];
    }

    // normal<T, 4> implementations

    template<typename T>
    constexpr CPU_GPU normal<T, 4>::normal() requires std::is_default_constructible_v<value_type>
            : _data{{}}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 4>::normal(T x, T y, T z, T w) : _data{{x, y, z, w}}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 4>::normal(const normal& cpy) requires std::is_copy_constructible_v<value_type>
            : _data{cpy.data}
    {}

    template<typename T>
    constexpr CPU_GPU normal<T, 4>::normal(normal&& mv) requires std::is_move_constructible_v<value_type>
            : _data{std::move(mv)}
    {}

    template<typename T>
    CPU_GPU normal<T, 4>& normal<T, 4>::operator=(const normal& cpy) requires std::is_copy_assignable_v<value_type>
    {
        if (this != &cpy) {
            for (std::size_t i = 0; i < size; ++i) _data.buffer[i] = cpy[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU normal<T, 4>& normal<T, 4>::operator=(normal&& mv) requires std::is_move_assignable_v<value_type>
    {
        if (this != &mv) {
            for (std::size_t i = 0; i < size; ++i) _data.buffer[i] = std::move(mv[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU T& normal<T, 4>::operator[](int idx)
    {
        return _data.buffer[idx];
    }

    template<typename T>
    constexpr CPU_GPU T normal<T, 4>::operator[](int idx) const
    {
        return _data.buffer[idx];
    }

    // functions

    template<vector_like Vector>
    requires (Vector::size == 3)
    constexpr CPU_GPU normal<typename Vector::value_type, 3>
    face_forward(const normal<typename Vector::value_type, 3>& n, const vector<typename Vector::value_type, 3>& v)
    {
        return dot(n, v) < 0.0f ? -n : n;
    }

}

#endif //GPU_RAYTRACE_NORMAL_INL

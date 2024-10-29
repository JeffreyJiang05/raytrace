#ifndef GPU_RAYTRACE_POINT_INL
#define GPU_RAYTRACE_POINT_INL

#include "math/geometry/point.hpp"

namespace math
{
    // point<T, 1>

    template<typename T>
    constexpr CPU_GPU point<T, 1>::point() requires std::is_default_constructible_v<value_type> : _data{ {} } {}

    template<typename T>
    constexpr CPU_GPU point<T, 1>::point(value_type _x) : _data{ { _x } } {}

    template<typename T>
    constexpr CPU_GPU point<T, 1>::point(const point& cpy) requires std::is_copy_constructible_v<value_type> : _data{ cpy._data } {}

    template<typename T>
    constexpr CPU_GPU point<T, 1>::point(point&& mv) requires std::is_move_constructible_v<value_type> : _data{ std::move(mv._data) } {}

    template<typename T>
    CPU_GPU point<T, 1>& point<T, 1>::operator=(const point& cpy) requires std::is_copy_assignable_v<value_type>
    {
        if (this == &cpy) return *this;
        for (std::size_t i = 0; i < size; ++i)
        {
            _data.buffer[i] = cpy._data.buffer[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU point<T, 1>& point<T, 1>::operator=(point&& mv) requires std::is_move_assignable_v<value_type>
    {
        if (this == &mv) return *this;
        for (std::size_t i = 0; i < size; ++i)
        {
            _data.buffer[i] = std::move(mv._data.buffer[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU point<T, 1>::operator value_type()
    {
        return _data.buffer[0];
    }

    template<typename T>
    constexpr CPU_GPU auto point<T, 1>::operator[](int N) -> value_type&
    {
        return _data.buffer[N];
    }

    template<typename T>
    constexpr CPU_GPU auto point<T, 1>::operator[](int N) const -> const value_type&
    {
        return _data.buffer[N];
    }

    // point<T, 2>

    template<typename T>
    constexpr CPU_GPU point<T, 2>::point() requires std::is_default_constructible_v<value_type> : _data{ {} } {}

    template<typename T>
    constexpr CPU_GPU point<T, 2>::point(value_type _x, value_type _y) : _data{ { _x, _y } } {}

    template<typename T>
    constexpr CPU_GPU point<T, 2>::point(const point& cpy) requires std::is_copy_constructible_v<value_type> : _data{ cpy._data } {}

    template<typename T>
    constexpr CPU_GPU point<T, 2>::point(point&& mv) requires std::is_move_constructible_v<value_type> : _data{ std::move(mv._data) } {}

    template<typename T>
    CPU_GPU point<T, 2>& point<T, 2>::operator=(const point& cpy) requires std::is_copy_assignable_v<value_type>
    {
        if (this == &cpy) return *this;
        for (std::size_t i = 0; i < size; ++i)
        {
            _data.buffer[i] = cpy._data.buffer[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU point<T, 2>& point<T, 2>::operator=(point&& mv) requires std::is_move_assignable_v<value_type>
    {
        if (this == &mv) return *this;
        for (std::size_t i = 0; i < size; ++i)
        {
            _data.buffer[i] = std::move(mv._data.buffer[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU auto point<T, 2>::operator[](int N) -> value_type&
    {
        return _data.buffer[N];
    }

    template<typename T>
    constexpr CPU_GPU auto point<T, 2>::operator[](int N) const -> const value_type&
    {
        return _data.buffer[N];
    }

    // point<T, 3>


    template<typename T>
    constexpr CPU_GPU point<T, 3>::point() requires std::is_default_constructible_v<value_type> : _data{ {} } {}

    template<typename T>
    constexpr CPU_GPU point<T, 3>::point(value_type _x, value_type _y, value_type _z) : _data{ { _x, _y, _z } } {}

    template<typename T>
    constexpr CPU_GPU point<T, 3>::point(const point& cpy) requires std::is_copy_constructible_v<value_type> : _data{ cpy._data } {}

    template<typename T>
    constexpr CPU_GPU point<T, 3>::point(point&& mv) requires std::is_move_constructible_v<value_type> : _data{ std::move(mv._data) } {}

    template<typename T>
    CPU_GPU point<T, 3>& point<T, 3>::operator=(const point& cpy) requires std::is_copy_assignable_v<value_type>
    {
        if (this == &cpy) return *this;
        for (std::size_t i = 0; i < size; ++i)
        {
            _data.buffer[i] = cpy._data.buffer[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU point<T, 3>& point<T, 3>::operator=(point&& mv) requires std::is_move_assignable_v<value_type>
    {
        if (this == &mv) return *this;
        for (std::size_t i = 0; i < size; ++i)
        {
            _data.buffer[i] = std::move(mv._data.buffer[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU auto point<T, 3>::operator[](int N) -> value_type&
    {
        return _data.buffer[N];
    }

    template<typename T>
    constexpr CPU_GPU auto point<T, 3>::operator[](int N) const -> const value_type&
    {
        return _data.buffer[N];
    }

    // point<T, 4>

    template<typename T>
    constexpr CPU_GPU point<T, 4>::point() requires std::is_default_constructible_v<value_type> : _data{ {} } {}

    template<typename T>
    constexpr CPU_GPU point<T, 4>::point(value_type _x, value_type _y, value_type _z, value_type _w) : _data{ { _x, _y, _z, _w } } {}

    template<typename T>
    constexpr CPU_GPU point<T, 4>::point(const point& cpy) requires std::is_copy_constructible_v<value_type> : _data{ cpy._data } {}

    template<typename T>
    constexpr CPU_GPU point<T, 4>::point(point&& mv) requires std::is_move_constructible_v<value_type> : _data{ std::move(mv._data) } {}

    template<typename T>
    CPU_GPU point<T, 4>& point<T, 4>::operator=(const point& cpy) requires std::is_copy_assignable_v<value_type>
    {
        if (this == &cpy) return *this;
        for (std::size_t i = 0; i < size; ++i)
        {
            _data.buffer[i] = cpy._data.buffer[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU point<T, 4>& point<T, 4>::operator=(point&& mv) requires std::is_move_assignable_v<value_type>
    {
        if (this == &mv) return *this;
        for (std::size_t i = 0; i < size; ++i)
        {
            _data.buffer[i] = std::move(mv._data.buffer[i]);
        }
        return *this;
    }


    template<typename T>
    constexpr CPU_GPU auto point<T, 4>::operator[](int N) -> value_type&
    {
        return _data.buffer[N];
    }

    template<typename T>
    constexpr CPU_GPU auto point<T, 4>::operator[](int N) const -> const value_type&
    {
        return _data.buffer[N];
    }
}

#endif //GPU_RAYTRACE_POINT_INL

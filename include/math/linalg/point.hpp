#ifndef GPU_RAYTRACE_POINT_HPP
#define GPU_RAYTRACE_POINT_HPP

#include "vec.hpp"

namespace math
{
    template<typename T, std::size_t N>
    class point;

    template<typename Derived>
    class point_base
    {
    private:
        CPU_GPU Derived& as_derived() { return *static_cast<Derived*>(this); }
        CPU_GPU const Derived& as_derived() const { return *static_cast<const Derived*>(this); }
    public:
        template<vector_like Vec>
        CPU_GPU Derived& operator+=(const Vec& vec) requires requires(typename Derived::value_type a, typename Vec::value_type b) { a += b; };

        template<vector_like Vec>
        CPU_GPU Derived& operator-=(const Vec& vec) requires requires(typename Derived::value_type a, typename Vec::value_type b) { a -= b; };

        template<vector_like Vec>
        CPU_GPU Derived& operator*=(const Vec& vec) requires requires(typename Derived::value_type a, typename Vec::value_type b) { a *= b; };

        template<vector_like Vec>
        CPU_GPU Derived& operator/=(const Vec& vec) requires requires(typename Derived::value_type a, typename Vec::value_type b) { a /= b; };
    };


    template<typename T>
    class point<T, 1> : public point_base<point<T, 1>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 1;
    private:
        container<value_type, size> _data;
    public:
        constexpr CPU_GPU point() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU point(value_type _x);
        constexpr CPU_GPU point(const point& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU point(point&& mv) requires std::is_move_constructible_v<value_type>;
        CPU_GPU point& operator=(const point& cpy) requires std::is_copy_assignable_v<value_type>;
        CPU_GPU point& operator=(point&& mv) requires std::is_move_assignable_v<value_type>;

        constexpr CPU_GPU operator value_type();

        constexpr CPU_GPU value_type& operator[](int N);
        constexpr CPU_GPU const value_type& operator[](int N) const;
    };

    template<typename T>
    class point<T, 2> : public point_base<point<T, 2>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 2;
    private:
        container<value_type, size> _data;
    public:
        constexpr CPU_GPU point() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU point(value_type _x, value_type _y);
        constexpr CPU_GPU point(const point& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU point(point&& mv) requires std::is_move_constructible_v<value_type>;
        CPU_GPU point& operator=(const point& cpy) requires std::is_copy_assignable_v<value_type>;
        CPU_GPU point& operator=(point&& mv) requires std::is_move_assignable_v<value_type>;

        constexpr CPU_GPU value_type& operator[](int N);
        constexpr CPU_GPU const value_type& operator[](int N) const;
    };

    template<typename T>
    class point<T, 3> : public point_base<point<T, 3>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 3;
    private:
        container<value_type, size> _data;
    public:
        constexpr CPU_GPU point() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU point(value_type _x, value_type _y, value_type _z);
        constexpr CPU_GPU point(const point& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU point(point&& mv) requires std::is_move_constructible_v<value_type>;
        CPU_GPU point& operator=(const point& cpy) requires std::is_copy_assignable_v<value_type>;
        CPU_GPU point& operator=(point&& mv) requires std::is_move_assignable_v<value_type>;

        constexpr CPU_GPU value_type& operator[](int N);
        constexpr CPU_GPU const value_type& operator[](int N) const;
    };

    template<typename T>
    class point<T, 4> : public point_base<point<T, 4>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 4;
    private:
        container<value_type, size> _data;
    public:
        constexpr CPU_GPU point() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU point(value_type _x, value_type _y, value_type _z, value_type _w);
        constexpr CPU_GPU point(const point& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU point(point&& mv) requires std::is_move_constructible_v<value_type>;
        CPU_GPU point& operator=(const point& cpy) requires std::is_copy_assignable_v<value_type>;
        CPU_GPU point& operator=(point&& mv) requires std::is_move_assignable_v<value_type>;

        constexpr CPU_GPU value_type& operator[](int N);
        constexpr CPU_GPU const value_type& operator[](int N) const;
    };

    using point1f = point<float, 1>;
    using point2f = point<float, 2>;
    using point3f = point<float, 3>;
    using point4f = point<float, 4>;

    template<typename T, std::size_t N, vector_like Vector>
    constexpr CPU_GPU auto operator+(const point<T, N>& pt, const Vector& vec) requires (N <= std::remove_cvref_t<Vector>::size && std::is_same_v<typename std::remove_cvref_t<Vector>::value_type, T>);

    template<typename T, std::size_t N, vector_like Vector>
    constexpr CPU_GPU auto operator-(const point<T, N>& pt, const Vector& vec) requires (N <= std::remove_cvref_t<Vector>::size && std::is_same_v<typename std::remove_cvref_t<Vector>::value_type, T>);

    template<typename T, std::size_t N0, typename U, std::size_t N1>
    constexpr CPU_GPU auto operator-(const point<T, N0>& pt0, const point<U, N1>& pt1) requires (N0 == N1) && requires(T a, U b) { a - b; };

}

#include "impl/point_base.inl"
#include "impl/point.inl"
#include "impl/point_func.inl"

#endif //GPU_RAYTRACE_POINT_HPP

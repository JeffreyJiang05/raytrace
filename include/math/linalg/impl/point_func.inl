#ifndef GPU_RAYTRACE_POINT_FUNC_INL
#define GPU_RAYTRACE_POINT_FUNC_INL

#include "math/linalg/point.hpp"

namespace math
{

    namespace impl
    {
        template<typename Point, vector_like Vector>
        using point_vec_add_t = point<decltype(std::declval<typename Point::value_type>() + std::declval<typename std::remove_cvref_t<Vector>::value_type>()), Point::size>;

        template<typename Point, vector_like Vector, std::size_t... Ns>
        constexpr CPU_GPU point_vec_add_t<Point, Vector> point_vec_add(const Point& point, const Vector& vector, std::index_sequence<Ns...>)
        {
            return point_vec_add_t<Point, Vector>{ (point[Ns] + vector[Ns])... };
        }

        template<typename Point, vector_like Vector>
        using point_vec_sub_t = point<decltype(std::declval<typename Point::value_type>() - std::declval<typename std::remove_cvref_t<Vector>::value_type>()), Point::size>;

        template<typename Point, vector_like Vector, std::size_t... Ns>
        constexpr CPU_GPU point_vec_sub_t<Point, Vector> point_vec_sub(const Point& point, const Vector& vector, std::index_sequence<Ns...>)
        {
            return point_vec_sub_t<Point, Vector>{ (point[Ns] - vector[Ns])... };
        }

        template<typename Point0, typename Point1>
        using point_point_sub_t = point<decltype(std::declval<typename Point0::value_type>() + std::declval<typename Point1::value_type>()), Point0::size>;

        template<typename Point0, typename Point1, std::size_t... Ns>
        constexpr CPU_GPU point_point_sub_t<Point0, Point1> point_point_sub(const Point0& p0, const Point1& p1, std::index_sequence<Ns...>)
        {
            return point_point_sub_t<Point0, Point1>{ (p0[Ns] - p1[Ns])... };
        }

    }

    template<typename T, std::size_t N, vector_like Vector>
    constexpr CPU_GPU auto operator+(const point<T, N>& pt, const Vector& vec) requires (N <= std::remove_cvref_t<Vector>::size && std::is_same_v<typename std::remove_cvref_t<Vector>::value_type, T>)
    {
        return impl::point_vec_add<point<T, N>, Vector>(pt, vec, std::make_index_sequence<N>{});
    }

    template<typename T, std::size_t N, vector_like Vector>
    constexpr CPU_GPU auto operator-(const point<T, N>& pt, const Vector& vec) requires (N <= std::remove_cvref_t<Vector>::size && std::is_same_v<typename std::remove_cvref_t<Vector>::value_type, T>)
    {
        return impl::point_vec_sub<point<T, N>, Vector>(pt, vec, std::make_index_sequence<N>{});
    }

    template<typename T, std::size_t N0, typename U, std::size_t N1>
    constexpr CPU_GPU auto operator-(const point<T, N0>& pt0, const point<U, N1>& pt1) requires (N0 == N1) && requires(T a, U b) { a - b; }
    {
        return impl::point_point_sub<point<T, N0>, point<T, N1>>(pt0, pt1, std::make_index_sequence<N0>{});
    }

}

#endif //GPU_RAYTRACE_POINT_FUNC_INL

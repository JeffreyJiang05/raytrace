#ifndef GPU_RAYTRACE_VEC_FUNC_INL
#define GPU_RAYTRACE_VEC_FUNC_INL

#include "math/geometry/vec.hpp"

#include <algorithm>
#include <utility>

#include "math/floats.hpp"
#include "math/functions.hpp"

namespace math
{
    template<std::size_t N, vector_like Vec>
    constexpr decltype(auto) get(Vec&& v)
    {
        return v[N];
    }

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a == b; }
    constexpr CPU_GPU bool operator==(const Vector0& v0, const Vector1& v1)
    {
        for (std::size_t i = 0; i < std::remove_cvref_t<Vector0>::size; ++i)
        {
            if (!(v0[i] == v1[i])) return false;
        }
        return true;
    }

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a != b; }
    constexpr CPU_GPU bool operator!=(const Vector0& v0, const Vector1& v1)
    {
        for (std::size_t i = 0; i < std::remove_cvref_t<Vector0>::size; ++i)
        {
            if (v0[i] != v1[i]) return true;
        }
        return false;
    }

    namespace impl
    {
        template<vector_like Vector0, vector_like Vector1>
        using add_ret_t = vector<decltype(std::declval<typename std::remove_cvref_t<Vector0>::value_type>() + std::declval<typename std::remove_cvref_t<Vector1>::value_type>()), std::remove_cvref_t<Vector0>::size>;

        template<vector_like Vector0, vector_like Vector1, std::size_t... Indices>
        constexpr CPU_GPU add_ret_t<Vector0, Vector1> add(const Vector0& v0, const Vector1& v1, std::index_sequence<Indices...>)
        {
            return add_ret_t<Vector0, Vector1>{ (v0[Indices] + v1[Indices])... };
        }

        template<vector_like Vector0, vector_like Vector1>
        using sub_ret_t = vector<decltype(std::declval<typename std::remove_cvref_t<Vector0>::value_type>() - std::declval<typename std::remove_cvref_t<Vector1>::value_type>()), std::remove_cvref_t<Vector0>::size>;

        template<vector_like Vector0, vector_like Vector1, std::size_t... Indices>
        constexpr CPU_GPU sub_ret_t<Vector0, Vector1> sub(const Vector0& v0, const Vector1& v1, std::index_sequence<Indices...>)
        {
            return sub_ret_t<Vector0, Vector1>{ (v0[Indices] - v1[Indices])... };
        }

        template<vector_like Vector0, vector_like Vector1>
        using mul_ret_t = vector<decltype(std::declval<typename std::remove_cvref_t<Vector0>::value_type>() * std::declval<typename std::remove_cvref_t<Vector1>::value_type>()), std::remove_cvref_t<Vector0>::size>;

        template<vector_like Vector0, vector_like Vector1, std::size_t... Indices>
        constexpr CPU_GPU mul_ret_t<Vector0, Vector1> mul(const Vector0& v0, const Vector1& v1, std::index_sequence<Indices...>)
        {
            return mul_ret_t<Vector0, Vector1>{ (v0[Indices] * v1[Indices])... };
        }

        template<vector_like Vector, typename T>
        using rscale_ret_t = vector<decltype( std::declval<typename std::remove_cvref_t<Vector>::value_type>() * std::declval<std::remove_cvref_t<T>>() ), std::remove_cvref_t<Vector>::size>;

        template<vector_like Vector, typename T>
        using lscale_ret_t = vector<decltype( std::declval<std::remove_cvref_t<T>>() * std::declval<typename std::remove_cvref_t<Vector>::value_type>() ), std::remove_cvref_t<Vector>::size>;

        template<vector_like Vector, typename T, std::size_t... Indices>
        constexpr CPU_GPU rscale_ret_t<Vector, T> rscale(const Vector& v, const T& t, std::index_sequence<Indices...>)
        {
            return rscale_ret_t<Vector, T>{ (v[Indices] * t)... };
        }

        template<typename T, vector_like Vector, std::size_t... Indices>
        constexpr CPU_GPU lscale_ret_t<T, Vector> lscale(const T& t, const Vector& v, std::index_sequence<Indices...>)
        {
            return lscale_ret_t<T, Vector>{ (t * v[Indices])... };
        }

        template<vector_like Vector0, vector_like Vector1>
        using div_ret_t = vector<decltype(std::declval<typename std::remove_cvref_t<Vector0>::value_type>() / std::declval<typename std::remove_cvref_t<Vector1>::value_type>()), std::remove_cvref_t<Vector0>::size>;

        template<vector_like Vector0, vector_like Vector1, std::size_t... Indices>
        constexpr CPU_GPU div_ret_t<Vector0, Vector1> div(const Vector0& v0, const Vector1& v1, std::index_sequence<Indices...>)
        {
            return div_ret_t<Vector0, Vector1>{ (v0[Indices] / v1[Indices])... };
        }

        template<vector_like Vector, typename T>
        using rdscale_ret_t = vector<decltype( std::declval<typename std::remove_cvref_t<Vector>::value_type>() / std::declval<std::remove_cvref_t<T>>() ), std::remove_cvref_t<Vector>::size>;

        template<vector_like Vector, typename T>
        using ldscale_ret_t = vector<decltype( std::declval<std::remove_cvref_t<T>>() / std::declval<typename std::remove_cvref_t<Vector>::value_type>() ), std::remove_cvref_t<Vector>::size>;

        template<vector_like Vector, typename T, std::size_t... Indices>
        constexpr CPU_GPU rdscale_ret_t<Vector, T> rdscale(const Vector& v, const T& t, std::index_sequence<Indices...>)
        {
            return rscale_ret_t<Vector, T>{ (v[Indices] / t)... };
        }

        template<typename T, vector_like Vector, std::size_t... Indices>
        constexpr CPU_GPU ldscale_ret_t<T, Vector> ldscale(const T& t, const Vector& v, std::index_sequence<Indices...>)
        {
            return lscale_ret_t<T, Vector>{ (t / v[Indices])... };
        }

        template<vector_like Vector0, vector_like Vector1, std::size_t... Indices>
        constexpr CPU_GPU auto dot(const Vector0& v0, const Vector1& v1, std::index_sequence<Indices...>)
        {
            return ( (v0[Indices] * v1[Indices]) + ... );
        }

        template<vector_like Vector0, vector_like Vector1>
        using cross_ret_t = vector<decltype(std::declval<typename std::remove_cvref_t<Vector0>::value_type>() * std::declval<typename std::remove_cvref_t<Vector1>::value_type>() - std::declval<typename std::remove_cvref_t<Vector0>::value_type>() * std::declval<typename std::remove_cvref_t<Vector1>::value_type>()), 3>;

        template<vector_like Vector, std::floating_point T>
        using lerp_ret_t = vector<decltype(std::declval<typename std::remove_cvref_t<Vector>::value_type>() * std::declval<T>() + std::declval<typename std::remove_cvref_t<Vector>::value_type>() * std::declval<T>()), std::remove_cvref_t<Vector>::size>;

        template<vector_like Vector, std::floating_point T, std::size_t... Indices>
        constexpr CPU_GPU lerp_ret_t<Vector, T> lerp(const Vector& a, const Vector& b, T t, std::index_sequence<Indices...>)
        {
            return lerp_ret_t<Vector, T>{ ((1 - t) * a[Indices] + t * b[Indices])... };
        }

        template<vector_like Vector0, vector_like Vector1, vector_like Vector2>
        using fma_ret_t = vector<decltype(std::declval<typename std::remove_cvref_t<Vector0>::value_type>() * std::declval<typename std::remove_cvref_t<Vector1>::value_type>() + std::declval<typename std::remove_cvref_t<Vector2>::value_type>()), std::remove_cvref_t<Vector0>::size>;

        template<vector_like Vector0, vector_like Vector1, vector_like Vector2, std::size_t... Indices>
        constexpr CPU_GPU fma_ret_t<Vector0, Vector1, Vector2> fma(const Vector0& v0, const Vector1& v1, const Vector2& v2, std::index_sequence<Indices...>)
        {
            if constexpr(std::is_arithmetic_v<Vector0> && std::is_arithmetic_v<Vector1> && std::is_arithmetic_v<Vector2>)
                return fma_ret_t<Vector0, Vector1, Vector2>{ (math::fma(v0[Indices], v1[Indices], v2[Indices]))... };
            else return fma_ret_t<Vector0, Vector1, Vector2>{ (v0[Indices] * v1[Indices] + v2[Indices])... };
        }

        template<vector_like Vector, std::size_t... Indices>
        constexpr CPU_GPU std::remove_cvref_t<Vector> min(const Vector& v0, const Vector& v1, std::index_sequence<Indices...>)
        {
            return std::remove_cvref_t<Vector>{ std::min(v0[Indices], v1[Indices])... };
        }

        template<vector_like Vector, std::size_t... Indices>
        constexpr CPU_GPU std::remove_cvref_t<Vector> max(const Vector& v0, const Vector& v1, std::index_sequence<Indices...>)
        {
            return std::remove_cvref_t<Vector>{ std::max(v0[Indices], v1[Indices])... };
        }

        template<typename To, vector_like Vector>
        using cast_ret_t = vector<std::remove_cvref_t<To>, std::remove_cvref_t<Vector>::size>;

        template<typename To, vector_like Vector, std::size_t... Indices>
        constexpr CPU_GPU cast_ret_t<To, Vector> vec_cast(const Vector& v, std::index_sequence<Indices...>)
        {
            return cast_ret_t<To, Vector>{ static_cast<std::remove_cvref_t<To>>(v[Indices])... };
        }

        template<vector_like Vector0, vector_like Vector1>
        using ortho_vec_t = vector<decltype(std::declval<typename std::remove_cvref_t<Vector0>::value_type>() - dot(std::declval<std::remove_cvref_t<Vector0>>(), std::declval<std::remove_cvref_t<Vector1>>()) * std::declval<typename std::remove_cvref_t<Vector1>::value_type>()), 3>;

    }

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a + b; }
    constexpr CPU_GPU auto operator+(const Vector0& v0, const Vector1& v1)
    {
        return impl::add(v0, v1, std::make_index_sequence<std::remove_cvref_t<Vector0>::size>{});
    }

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a - b; }
    constexpr CPU_GPU auto operator-(const Vector0& v0, const Vector1& v1)
    {
        return impl::sub(v0, v1, std::make_index_sequence<std::remove_cvref_t<Vector0>::size>{});
    }

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a * b; }
    constexpr CPU_GPU auto operator*(const Vector0& v0, const Vector1& v1)
    {
        return impl::mul(v0, v1, std::make_index_sequence<std::remove_cvref_t<Vector0>::size>{});
    }

    template<vector_like Vector, typename T> requires requires(typename std::remove_cvref_t<Vector>::value_type a, std::remove_cvref_t<T> b) { a * b; }
    constexpr CPU_GPU auto operator*(const Vector& v, T t)
    {
        return impl::rscale(v, t, std::make_index_sequence<std::remove_cvref_t<Vector>::size>{});
    }

    template<typename T, vector_like Vector> requires requires(std::remove_cvref_t<T> a, typename std::remove_cvref_t<Vector>::value_type b) { a * b; }
    constexpr CPU_GPU auto operator*(T t, const Vector& v)
    {
        return impl::lscale(v, t, std::make_index_sequence<std::remove_cvref_t<Vector>::size>{});
    }

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a / b; }
    constexpr CPU_GPU auto operator/(const Vector0& v0, const Vector1& v1)
    {
        return impl::div(v0, v1, std::make_index_sequence<std::remove_cvref_t<Vector0>::size>{});
    }

    template<vector_like Vector, typename T> requires requires(typename std::remove_cvref_t<Vector>::value_type a, std::remove_cvref_t<T> b) { a / b; }
    constexpr CPU_GPU auto operator/(const Vector& v, T t)
    {
        return impl::rdscale(v, t, std::make_index_sequence<std::remove_cvref_t<Vector>::size>{});
    }

    template<typename T, vector_like Vector> requires requires(std::remove_cvref_t<T> a, typename std::remove_cvref_t<Vector>::value_type b) { a / b; }
    constexpr CPU_GPU auto operator/(T t, const Vector& v)
    {
        return impl::ldscale(v, t, std::make_index_sequence<std::remove_cvref_t<Vector>::size>{});
    }

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a * b + a * b; }
    constexpr CPU_GPU auto dot(const Vector0& v0, const Vector1& v1)
    {
        return impl::dot(v0, v1, std::make_index_sequence<std::remove_cvref_t<Vector0>::size>{});
    }

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size && std::remove_cvref_t<Vector1>::size == 3) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a * b - a * b; }
    constexpr CPU_GPU auto cross(const Vector0& v0, const Vector1& v1)
    {
        return impl::cross_ret_t<Vector0, Vector1>{
            v0[1] * v1[2] - v0[2] * v1[1],
            v0[2] * v1[0] - v0[0] * v1[2],
            v0[0] * v1[1] - v0[1] * v1[0]
        };
    }

    template<vector_like Vector>
    constexpr CPU_GPU auto magnitude(const Vector& v)
    {
        return math::sqrt(dot(v, v));
    }

    template<std::floating_point T, vector_like Vector>
    constexpr CPU_GPU auto normalize(const Vector& v0)
    {
        return vec_cast<T>(v0) / magnitude(vec_cast<T>(v0));
    }

    template<vector_like Vector, std::floating_point T>
    constexpr CPU_GPU auto lerp(const Vector& v0, const Vector& v1, T t)
    {
        return impl::lerp(v0, v1, t, std::make_index_sequence<std::remove_cvref_t<Vector>::size>{});
    }

    template<vector_like Vector0, vector_like Vector1, vector_like Vector2> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size && std::remove_cvref_t<Vector1>::size == std::remove_cvref_t<Vector2>::size)
    constexpr CPU_GPU auto fma(const Vector0& v0, const Vector1& v1, const Vector2& v2)
    {
        return impl::fma(v0, v1, v2, std::make_index_sequence<std::remove_cvref_t<Vector0>::size>{});
    }

    template<vector_like Vector> requires requires(typename std::remove_cvref_t<Vector>::value_type a) { { a < a } -> std::same_as<bool>; }
    constexpr CPU_GPU auto min(const Vector& v0, const Vector& v1)
    {
        return impl::min(v0, v1, std::make_index_sequence<std::remove_cvref_t<Vector>::size>{});
    }

    template<vector_like Vector> requires requires(typename std::remove_cvref_t<Vector>::value_type a) { { a < a } -> std::same_as<bool>; }
    constexpr CPU_GPU auto max(const Vector& v0, const Vector& v1)
    {
        return impl::max(v0, v1, std::make_index_sequence<std::remove_cvref_t<Vector>::size>{});
    }

    template<typename To, vector_like Vector> requires std::is_convertible_v<typename std::remove_cvref_t<Vector>::value_type, To>
    constexpr CPU_GPU auto vec_cast(const Vector& vec)
    {
        return impl::vec_cast<To>(vec, std::make_index_sequence<std::remove_cvref_t<Vector>::size>{});
    }

    template<std::floating_point AngleT, vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && std::floating_point<typename std::remove_cvref_t<Vector0>::value_type> && std::floating_point<typename std::remove_cvref_t<Vector1>::value_type>
    constexpr CPU_GPU AngleT angle_between(const Vector0& v0, const Vector1& v1)
    {
        using mag_t = decltype(magnitude(v0 + v1));
        if (dot(v0, v1) < 0)
            return static_cast<AngleT>(pi<AngleT> - 2 * arcsin(std::clamp(magnitude(v0 + v1) / 2, static_cast<mag_t>(-1), static_cast<mag_t>(1))));
        else
            return static_cast<AngleT>(2 * arcsin(std::clamp(magnitude(v1 - v0) / 2, static_cast<mag_t>(-1), static_cast<mag_t>(1))));
    }

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == 3 && std::remove_cvref_t<Vector1>::size == 3)
    constexpr CPU_GPU auto normal_to_ortho(const Vector0& v0, const Vector1& v1)
    {
        return impl::ortho_vec_t<Vector0, Vector1>{ v0 - dot(v0, v1) * v1 };
    }

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == 3 && std::remove_cvref_t<Vector1>::size == 3)
    constexpr CPU_GPU auto nonnormal_to_ortho(const Vector0& v0, const Vector1& v1)
    {
        return impl::ortho_vec_t<Vector0, Vector1>{ v0 - dot(v0, v1) / dot(v1, v1) * v1 };
    }

    template<vector_like Vector> requires (std::remove_cvref_t<Vector>::size == 3) && std::is_arithmetic_v<typename std::remove_cvref_t<Vector>::value_type>
    constexpr CPU_GPU auto coordinate_system(const Vector& v)
    {
        using vec_t = vector< typename std::remove_cvref_t<Vector>::value_type, 3 >;
        using value_type = typename vec_t::value_type;
        value_type sign = copysign(static_cast<value_type>(1), v[2]);
        auto a = -1 / (sign + v[2]);
        auto b = v[0] * v[1] * a;
        return std::pair{
                vec_t{ 1 + sign * v[0] * v[0] * a, sign * b, -sign * v[0] },
                vec_t{ b, sign + v[1] * v[1] * a, -v[1] }
        };
    }

    namespace impl
    {
        template<coordinate_system_type From, coordinate_system_type To>
        struct coordinate_system_convert;

        template<coordinate_system_type Same>
        struct coordinate_system_convert<Same, Same>
        {
            // (x, y) -> (r, theta)
            template<std::floating_point T>
            constexpr CPU_GPU static vector<T, 2> convert(const vector<T, 2>& p)
            {
                return p;
            }
        };

        template<>
        struct coordinate_system_convert<cartesian_coordinate<2>, polar_cooordinate>
        {
            // (x, y) -> (r, theta)
            template<std::floating_point T>
            constexpr CPU_GPU static vector<T, 2> convert(const vector<T, 2>& p)
            {
                T r = math::sqrt(p[0] * p[0] + p[1] * p[1]);
                T theta = math::arctan2(p[1], p[0]);
                return { r, theta };
            }
        };

        template<>
        struct coordinate_system_convert<polar_cooordinate, cartesian_coordinate<2>>
        {
            // (r, theta) -> (x, y)
            template<std::floating_point T>
            constexpr CPU_GPU static vector<T, 2> convert(const vector<T, 2>& p)
            {
                T x = p[0] * math::cos(p[1]);
                T y = p[1] * math::sin(p[1]);
                return { x, y };
            }
        };

        template<>
        struct coordinate_system_convert<cartesian_coordinate<3>, cylindrical_coordinate>
        {
            // (x, y, z) -> (r, theta, z)
            template<std::floating_point T>
            constexpr CPU_GPU static vector<T, 3> convert(const vector<T, 3>& p)
            {
                T r = math::sqrt(p[0] * p[0] + p[1] * p[1]);
                T theta = math::arctan2(p[1], p[0]);
                T z = p[2];
                return { r, theta, z };
            }
        };

        template<>
        struct coordinate_system_convert<cartesian_coordinate<3>, spherical_coordinate>
        {
            // (x, y, z) -> (rho, theta, phi)
            template<std::floating_point T>
            constexpr CPU_GPU static vector<T, 3> convert(const vector<T, 3>& p)
            {
                T rho = math::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
                T theta = math::arctan2(p[1], p[0]);
                T phi = math::arccos(p[2] / rho);
                return { rho, theta, phi };
            }
        };

        template<>
        struct coordinate_system_convert<cylindrical_coordinate, spherical_coordinate>
        {
            // (r, theta, z) -> (rho, theta, phi)
            template<std::floating_point T>
            constexpr CPU_GPU static vector<T, 3> convert(const vector<T, 3>& p)
            {
                T rho = math::sqrt(p[0] * p[0] + p[1] * p[1]);
                T theta = p[1];
                T phi = math::arccos(p[1] / rho);
                return { rho, theta, phi };
            }
        };

        template<>
        struct coordinate_system_convert<cylindrical_coordinate, cartesian_coordinate<3>>
        {
            // (r, theta, z) -> (x, y, z)
            template<std::floating_point T>
            constexpr CPU_GPU static vector<T, 3> convert(const vector<T, 3>& p)
            {
                T x = p[0] * math::cos(p[1]);
                T y = p[0] * math::sin(p[1]);
                T z = z;
                return { x, y, z };
            }
        };

        template<>
        struct coordinate_system_convert<spherical_coordinate, cylindrical_coordinate>
        {
            // (rho, theta, phi) -> (r, theta, z)
            template<std::floating_point T>
            constexpr CPU_GPU static vector<T, 3> convert(const vector<T, 3>& p)
            {
                T r = p[0] * math::sin(p[2]);
                T theta = p[1];
                T z = p[0] * math::cos(p[2]);
                return { r, theta, z };
            }
        };

        template<>
        struct coordinate_system_convert<spherical_coordinate, cartesian_coordinate<3>>
        {
            // (rho, theta, phi) -> (x, y, z)
            template<std::floating_point T>
            constexpr CPU_GPU static vector<T, 3> convert(const vector<T, 3>& p)
            {
                T x = p[0] * math::cos(p[1]) * math::sin(p[2]);
                T y = p[0] * math::sin(p[1]) * math::sin(p[2]);
                T z = p[0] * math::cos(p[2]);
                return { x, y, z };
            }
        };
    }

    template<coordinate_system_type From, coordinate_system_type To, std::floating_point T>
    constexpr CPU_GPU vector<T, coordinate_system_trait<To>::dimension> coordinate_cast(const vector<T, coordinate_system_trait<From>::dimension>& value) requires (coordinate_system_trait<To>::dimension == coordinate_system_trait<From>::dimension)
    {
        return impl::coordinate_system_convert<From, To>::convert(value);
    }

}

#endif //GPU_RAYTRACE_VEC_FUNC_INL

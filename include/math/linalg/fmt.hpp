#ifndef GPU_RAYTRACE_MATH_LINALG_FMT_HPP
#define GPU_RAYTRACE_MATH_LINALG_FMT_HPP

#include <fmt/core.h>

#include "vec.hpp"
#include "point.hpp"

template<math::vector_like Vector>
class fmt::formatter<Vector>
{
public:
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename Context>
    constexpr auto format(const Vector& vec, Context& ctx) const
    {
        constexpr auto size = std::remove_cvref_t<Vector>::size;
        if constexpr(size == 1)
            return format_to(ctx.out(), "[{}]", vec[0]);
        else if constexpr(size == 2)
            return format_to(ctx.out(), "[{}, {}]", vec[0], vec[1]);
        else if constexpr(size == 3)
            return format_to(ctx.out(), "[{}, {}, {}]", vec[0], vec[1], vec[2]);
        else if constexpr(size == 4)
            return format_to(ctx.out(), "[{}, {}, {}, {}]", vec[0], vec[1], vec[2], vec[3]);
        else
            return format_to(ctx.out(), "[NOT IMPLEMENTED]");
    }
};

template<typename T, std::size_t N>
class fmt::formatter<math::point<T, N>>
{
public:
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename Context>
    constexpr auto format(const math::point<T, N>& pt, Context& ctx) const
    {
        if constexpr(N == 1)
            return format_to(ctx.out(), "[{}]", pt[0]);
        else if constexpr(N == 2)
            return format_to(ctx.out(), "[{}, {}]", pt[0], pt[1]);
        else if constexpr(N == 3)
            return format_to(ctx.out(), "[{}, {}, {}]", pt[0], pt[1], pt[2]);
        else if constexpr(N == 4)
            return format_to(ctx.out(), "[{}, {}, {}, {}]", pt[0], pt[1], pt[2], pt[3]);
        else
            return format_to(ctx.out(), "[NOT IMPLEMENTED]");
    }
};

#endif //GPU_RAYTRACE_MATH_LINALG_FMT_HPP

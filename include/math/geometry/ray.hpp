#ifndef GPU_RAYTRACE_RAY_HPP
#define GPU_RAYTRACE_RAY_HPP

#include <concepts>

#include "point.hpp"
#include "vec.hpp"

namespace math
{

    template<typename T, std::size_t N>
    class ray
    {
    private:
        point<T, N> _point;
        vector<T, N> _vec;
    public:
        constexpr CPU_GPU ray() = default;
        constexpr CPU_GPU ray(point<T, N> origin, vector<T, N> direction);
        constexpr CPU_GPU ray(const ray&) = default;
        constexpr CPU_GPU ray(ray&&) = default;
        CPU_GPU ray& operator=(const ray&) = default;
        CPU_GPU ray& operator=(ray&&) = default;

        constexpr CPU_GPU point<T, N>& get_origin();
        constexpr CPU_GPU const point<T, N>& get_origin() const;
        constexpr CPU_GPU vector<T, N>& get_direction();
        constexpr CPU_GPU const vector<T, N>& get_direction() const;

        template<std::floating_point FloatT>
        constexpr CPU_GPU auto at(FloatT t) const;

        template<std::floating_point FloatT>
        constexpr CPU_GPU auto operator()(FloatT t) const;
    };

    template<std::floating_point T, std::size_t N>
    class tracked_ray : public ray<T, N>
    {
    private:
        T _time;
    public:
        constexpr CPU_GPU tracked_ray() = default;
        constexpr CPU_GPU tracked_ray(point<T, N> origin, vector<T, N> direction, T time = 0);
        constexpr CPU_GPU tracked_ray(const tracked_ray&) = default;
        constexpr CPU_GPU tracked_ray(tracked_ray&&) = default;
        CPU_GPU tracked_ray& operator=(const tracked_ray&) = default;
        CPU_GPU tracked_ray& operator=(tracked_ray&&) = default;

        constexpr CPU_GPU T get_time() const;
        constexpr CPU_GPU void set_time(T new_time);
        constexpr CPU_GPU void increment_time(T delta);

        constexpr CPU_GPU auto at() const;
        constexpr CPU_GPU auto operator()() const;
    };

}

#endif //GPU_RAYTRACE_RAY_HPP

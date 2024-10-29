#ifndef GPU_RAYTRACE_RAY_INL
#define GPU_RAYTRACE_RAY_INL

#include "math/geometry/ray.hpp"

namespace math
{

    template<typename T, std::size_t N>
    constexpr CPU_GPU ray<T, N>::ray(point<T, N> origin, vector<T, N> direction) : _point{ origin }, _vec{ direction } {}

    template<typename T, std::size_t N>
    constexpr CPU_GPU point<T, N>& ray<T, N>::get_origin()
    {
        return _point;
    }

    template<typename T, std::size_t N>
    constexpr CPU_GPU const point<T, N>& ray<T, N>::get_origin() const
    {
        return _point;
    }

    template<typename T, std::size_t N>
    constexpr CPU_GPU vector<T, N>& ray<T, N>::get_direction()
    {
        return _vec;
    }

    template<typename T, std::size_t N>
    constexpr CPU_GPU const vector<T, N>& ray<T, N>::get_direction() const
    {
        return _vec;
    }

    template<typename T, std::size_t N>
    template<std::floating_point FloatT>
    constexpr CPU_GPU auto ray<T, N>::at(FloatT t) const
    {
        return _point + t * _vec;
    }

    template<typename T, std::size_t N>
    template<std::floating_point FloatT>
    constexpr CPU_GPU auto ray<T, N>::operator()(FloatT t) const
    {
        return at(t);
    }

    template<std::floating_point T, std::size_t N>
    constexpr CPU_GPU tracked_ray<T, N>::tracked_ray(point<T, N> origin, vector<T, N> direction, T time) : ray<T, N>{ origin, direction }, _time{ time } {}

    template<std::floating_point T, std::size_t N>
    constexpr CPU_GPU T tracked_ray<T, N>::get_time() const
    {
        return time;
    }

    template<std::floating_point T, std::size_t N>
    constexpr CPU_GPU void tracked_ray<T, N>::set_time(T new_time)
    {
        _time = new_time;
    }

    template<std::floating_point T, std::size_t N>
    constexpr CPU_GPU void tracked_ray<T, N>::increment_time(T delta)
    {
        _time += delta;
    }

    template<std::floating_point T, std::size_t N>
    constexpr CPU_GPU auto tracked_ray<T, N>::at() const
    {
        return ray<T, N>::at(_time);
    }

    template<std::floating_point T, std::size_t N>
    constexpr CPU_GPU auto tracked_ray<T, N>::operator()() const
    {
        return ray<T, N>::at(_time);
    }

}

#endif //GPU_RAYTRACE_RAY_INL

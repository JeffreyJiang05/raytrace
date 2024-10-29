#ifndef GPU_RAYTRACE_NORMAL_HPP
#define GPU_RAYTRACE_NORMAL_HPP

#include "vec.hpp"

namespace math
{

    template<typename T, std::size_t N>
    class normal;

    template<typename T>
    class normal<T, 1> : public vector_base<normal<T, 1>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 1;
    private:
        container<value_type, size> _data;
    public:
        constexpr CPU_GPU normal() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU normal(value_type _x);
        constexpr CPU_GPU normal(const normal& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU normal(normal&& mv) requires std::is_move_constructible_v<value_type>;
        CPU_GPU normal& operator=(const normal& cpy) requires std::is_copy_assignable_v<value_type>;
        CPU_GPU normal& operator=(normal&& mv) requires std::is_move_assignable_v<value_type>;

        constexpr CPU_GPU operator value_type() const;

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T>
    normal(T x) -> normal<T, 1>;

    template<typename T>
    class normal<T, 2> : public vector_base<normal<T, 2>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 2;
    private:
        container<value_type, size> _data;
    public:
        constexpr CPU_GPU normal() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU normal(value_type _x, value_type _y);
        constexpr CPU_GPU normal(const normal& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU normal(normal&& mv) requires std::is_move_constructible_v<value_type>;
        CPU_GPU normal& operator=(const normal& cpy) requires std::is_copy_assignable_v<value_type>;
        CPU_GPU normal& operator=(normal&& mv) requires std::is_move_assignable_v<value_type>;

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T>
    normal(T x, T y) -> normal<T, 2>;

    template<typename T>
    class normal<T, 3> : public vector_base<normal<T, 3>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 3;
    private:
        container<value_type, size> _data;
    public:
        constexpr CPU_GPU normal() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU normal(value_type _x, value_type _y, value_type _z);
        constexpr CPU_GPU normal(const normal& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU normal(normal&& mv) requires std::is_move_constructible_v<value_type>;
        CPU_GPU normal& operator=(const normal& cpy) requires std::is_copy_assignable_v<value_type>;
        CPU_GPU normal& operator=(normal&& mv) requires std::is_move_assignable_v<value_type>;

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T>
    normal(T x, T y, T z) -> normal<T, 3>;

    template<typename T>
    class normal<T, 4> : public vector_base<normal<T, 4>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 4;
    private:
        container<value_type, size> _data;
    public:
        constexpr CPU_GPU normal() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU normal(value_type _x, value_type _y, value_type _z, value_type _w);
        constexpr CPU_GPU normal(const normal& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU normal(normal&& mv) requires std::is_move_constructible_v<value_type>;
        CPU_GPU normal& operator=(const normal& cpy) requires std::is_copy_assignable_v<value_type>;
        CPU_GPU normal& operator=(normal&& mv) requires std::is_move_assignable_v<value_type>;

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T>
    normal(T x, T y, T z, T w) -> normal<T, 4>;

    using normal1f = normal<float, 1>;
    using normal2f = normal<float, 2>;
    using normal3f = normal<float, 3>;
    using normal4f = normal<float, 4>;

    template<vector_like Vector> requires (Vector::size == 3)
    constexpr CPU_GPU normal<typename Vector::value_type, 3> face_forward(const normal<typename Vector::value_type, 3>& n, const vector<typename Vector::value_type, 3>& v);

}

#include "impl/normal.inl"

#endif //GPU_RAYTRACE_NORMAL_HPP

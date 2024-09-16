#ifndef GPU_RAYTRACE_VEC_HPP
#define GPU_RAYTRACE_VEC_HPP

#include <cstdint>
#include <concepts>
#include <cmath>

#include "math/functions.hpp"

// macros to allow the swizzling of vectors
#define PRIMITIVE_CAT_IDENTIFIER_2(id0, id1) id0 ## id1
#define PRIMITIVE_CAT_IDENTIFIER_3(id0, id1, id2) id0 ## id1 ## id2
#define PRIMITIVE_CAT_IDENTIFIER_4(id0, id1, id2, id3) id0 ## id1 ## id2 ## id3

#define CAT_IDENTIFIER_1(id0) id0
#define CAT_IDENTIFIER_2(id0, id1) PRIMITIVE_CAT_IDENTIFIER_2(id0, id1)
#define CAT_IDENTIFIER_3(id0, id1, id2) PRIMITIVE_CAT_IDENTIFIER_3(id0, id1, id2)
#define CAT_IDENTIFIER_4(id0, id1, id2, id3) PRIMITIVE_CAT_IDENTIFIER_4(id0, id1, id2, id3)

#define EXPAND_COMPONENT(value) COMPONENT ## value

#define CAT_COMPONENT_1(v0) CAT_IDENTIFIER_1(EXPAND_COMPONENT(v0))
#define CAT_COMPONENT_2(v0, v1) CAT_IDENTIFIER_2(EXPAND_COMPONENT(v0), EXPAND_COMPONENT(v1))
#define CAT_COMPONENT_3(v0, v1, v2) CAT_IDENTIFIER_3(EXPAND_COMPONENT(v0), EXPAND_COMPONENT(v1), EXPAND_COMPONENT(v2))
#define CAT_COMPONENT_4(v0, v1, v2, v3) CAT_IDENTIFIER_4(EXPAND_COMPONENT(v0), EXPAND_COMPONENT(v1), EXPAND_COMPONENT(v2), EXPAND_COMPONENT(v3))

#define DEFINE_SWIZZLE_1(vec_template, type, size, v0) const vec_template<type, size, v0> CAT_COMPONENT_1(v0)
#define DEFINE_SWIZZLE_2(vec_template, type, size, v0, v1) const vec_template<type, size, v0, v1> CAT_COMPONENT_2(v0, v1)
#define DEFINE_SWIZZLE_3(vec_template, type, size, v0, v1, v2) const vec_template<type, size, v0, v1, v2> CAT_COMPONENT_3(v0, v1, v2)
#define DEFINE_SWIZZLE_4(vec_template, type, size, v0, v1, v2, v3) const vec_template<type, size, v0, v1, v2, v3> CAT_COMPONENT_4(v0, v1, v2, v3)

#define OVERLOAD_7MACRO(_1, _2, _3, _4, _5, _6, _7, NAME, ...) NAME
#define PRIMITIVE_DEFINE_SWIZZLE(...) OVERLOAD_7MACRO(__VA_ARGS__, DEFINE_SWIZZLE_4, DEFINE_SWIZZLE_3, DEFINE_SWIZZLE_2, DEFINE_SWIZZLE_1)(__VA_ARGS__)
#define DEFINE_SWIZZLE(...) PRIMITIVE_DEFINE_SWIZZLE(VEC_TEMPLATE, TYPE, SIZE, __VA_ARGS__)

#define DEFINE_TAG_1(vec_template, v0) constexpr inline vec_template<v0> CAT_COMPONENT_1(v0)
#define DEFINE_TAG_2(vec_template, v0, v1) constexpr inline vec_template<v0, v1> CAT_COMPONENT_2(v0, v1)
#define DEFINE_TAG_3(vec_template, v0, v1, v2) constexpr inline vec_template<v0, v1, v2> CAT_COMPONENT_3(v0, v1, v2)
#define DEFINE_TAG_4(vec_template, v0, v1, v2, v3) constexpr inline vec_template<v0, v1, v2, v3> CAT_COMPONENT_4(v0, v1, v2, v3)

#define OVERLOAD_5MACRO(_1, _2, _3, _4, _5, NAME, ...) NAME
#define PRIMITIVE_DEFINE_TAG(...) OVERLOAD_5MACRO(__VA_ARGS__, DEFINE_TAG_4, DEFINE_TAG_3, DEFINE_TAG_2, DEFINE_TAG_1)(__VA_ARGS__)
#define DEFINE_TAG(...) PRIMITIVE_DEFINE_TAG(VEC_TEMPLATE, __VA_ARGS__)

#define COMPONENT0 x
#define COMPONENT1 y
#define COMPONENT2 z
#define COMPONENT3 w
#define TYPE value_type

namespace math
{

    template<typename T, std::size_t N> struct container { T buffer[N]; };

    template<typename Derived>
    class vector_base;

    /**
     * concept for vector-like classes.
     * @tparam T vector type
     */
    template<typename T>
    concept vector_like = requires(std::remove_cvref_t<T> vec, const std::remove_cvref_t<T> cvec)
    {
        typename std::remove_cvref_t<T>::value_type;
        { std::remove_cvref_t<T>::size } -> std::same_as<const std::size_t&>;
        { vec[0] } -> std::same_as<typename std::remove_cvref_t<T>::value_type&>;
        { cvec[0] } -> std::same_as<typename std::remove_cvref_t<T>::value_type>;
        requires std::is_base_of_v<vector_base<std::remove_cvref_t<T>>, std::remove_cvref_t<T>>;
    };

    /**
     * CRTP base class
     * @tparam Derived vector-like class which will inherit vector_base
     */
    template<typename Derived>
    class vector_base
    {
    private:
        CPU_GPU Derived& as_derived() { return *static_cast<Derived*>(this); }
        CPU_GPU const Derived& as_derived() const { return *static_cast<const Derived*>(this); }

        /*template<vector_like Other>
        constexpr static bool has_same_dim = Derived::size == Other::size;*/
    public:

        // unfortunately, g++ has a internal compiler error here
        // see here for the likely culprit
        // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91930

        /*CPU_GPU Derived& operator=(const Derived& cpy) requires std::is_copy_assignable_v<typename Derived::value_type>;
        CPU_GPU Derived& operator=(Derived&& mv) requires std::is_move_assignable_v<typename Derived::value_type>;*/

        template<vector_like Other>
        CPU_GPU Derived& operator=(Other&& vec) requires requires() {
            requires Derived::value == Other::value;
            requires std::is_convertible_v<typename Other::value_type, typename Derived::value_type>;
        };

        template<vector_like Other>
        CPU_GPU Derived& operator+=(const Other& value) requires requires(typename Derived::value_type a, typename Other::value_type b)
        {
            requires Derived::size == Other::size;
            { a + b } -> std::convertible_to<typename Derived::value_type>;
        };

        template<vector_like Other>
        CPU_GPU Derived& operator-=(const Derived& value) requires requires(typename Derived::value_type a, typename Other::value_type b)
        {
            requires Derived::size == Other::size;
            { a - b } -> std::convertible_to<typename Derived::value_type>;
        };

        template<vector_like Other>
        CPU_GPU Derived& operator*=(const Derived& value) requires requires(typename Derived::value_type a, typename Other::value_type b)
        {
            requires Derived::size == Other::size;
            { a * b } -> std::convertible_to<typename Derived::value_type>;
        };

        template<typename T>
        CPU_GPU Derived& operator*=(T value) requires requires(typename Derived::value_type a, T b) { { a * b } -> std::convertible_to<typename Derived::value_type>; };

        template<vector_like Other>
        CPU_GPU Derived& operator/=(const Derived& value) requires requires(typename Derived::value_type a, typename Other::value_type b)
        {
            requires Derived::size == Other::size;
            { a / b } -> std::convertible_to<typename Derived::value_type>;
        };

        CPU_GPU Derived operator-() requires requires(typename Derived::value_type a) { { -a } -> std::same_as<typename Derived::value_type>; };

    };

    /**
     * swizzle vectors are simply used for permutations of a normal vector type.
     * consequently, they only provide a const interface.
     */
    template<typename T, std::size_t N, std::size_t... Refs>
    class swizzle_vector {};

    template<typename T, std::size_t N, std::size_t X>
    class swizzle_vector<T, N, X> : public vector_base<swizzle_vector<T, N, X>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 1;
    private:
        container<value_type, N> _components;
    public:
        constexpr CPU_GPU swizzle_vector(const container<value_type, N>& data);

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T, std::size_t N, std::size_t X, std::size_t Y>
    class swizzle_vector<T, N, X, Y> : public vector_base<swizzle_vector<T, N, X, Y>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 2;
    private:
        container<value_type, N> _components;
    public:
        constexpr CPU_GPU swizzle_vector(const container<value_type, N>& data);

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T, std::size_t N, std::size_t X, std::size_t Y, std::size_t Z>
    class swizzle_vector<T, N, X, Y, Z> : public vector_base<swizzle_vector<T, N, X, Y, Z>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 3;
    private:
        container<value_type, N> _components;
    public:
        constexpr CPU_GPU swizzle_vector(const container<value_type, N>& data);

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T, std::size_t N, std::size_t X, std::size_t Y, std::size_t Z, std::size_t W>
    class swizzle_vector<T, N, X, Y, Z, W> : public vector_base<swizzle_vector<T, N, X, Y, Z, W>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 4;
    private:
        container<value_type, N> _components;
    public:
        constexpr CPU_GPU swizzle_vector(const container<value_type, N>& data);

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<std::size_t... Swizzles>
    struct swizzle_tag {};

#define VEC_TEMPLATE swizzle_tag

    DEFINE_TAG(0); DEFINE_TAG(1); DEFINE_TAG(2); DEFINE_TAG(3);
    DEFINE_TAG(0, 0); DEFINE_TAG(0, 1); DEFINE_TAG(0, 2); DEFINE_TAG(0, 3);
    DEFINE_TAG(1, 0); DEFINE_TAG(1, 1); DEFINE_TAG(1, 2); DEFINE_TAG(1, 3);
    DEFINE_TAG(2, 0); DEFINE_TAG(2, 1); DEFINE_TAG(2, 2); DEFINE_TAG(2, 3);
    DEFINE_TAG(3, 0); DEFINE_TAG(3, 1); DEFINE_TAG(3, 2); DEFINE_TAG(3, 3);
    DEFINE_TAG(0, 0, 0); DEFINE_TAG(0, 0, 1); DEFINE_TAG(0, 0, 2); DEFINE_TAG(0, 0, 3);
    DEFINE_TAG(0, 1, 0); DEFINE_TAG(0, 1, 1); DEFINE_TAG(0, 1, 2); DEFINE_TAG(0, 1, 3);
    DEFINE_TAG(0, 2, 0); DEFINE_TAG(0, 2, 1); DEFINE_TAG(0, 2, 2); DEFINE_TAG(0, 2, 3);
    DEFINE_TAG(0, 3, 0); DEFINE_TAG(0, 3, 1); DEFINE_TAG(0, 3, 2); DEFINE_TAG(0, 3, 3);
    DEFINE_TAG(1, 0, 0); DEFINE_TAG(1, 0, 1); DEFINE_TAG(1, 0, 2); DEFINE_TAG(1, 0, 3);
    DEFINE_TAG(1, 1, 0); DEFINE_TAG(1, 1, 1); DEFINE_TAG(1, 1, 2); DEFINE_TAG(1, 1, 3);
    DEFINE_TAG(1, 2, 0); DEFINE_TAG(1, 2, 1); DEFINE_TAG(1, 2, 2); DEFINE_TAG(1, 2, 3);
    DEFINE_TAG(1, 3, 0); DEFINE_TAG(1, 3, 1); DEFINE_TAG(1, 3, 2); DEFINE_TAG(1, 3, 3);
    DEFINE_TAG(2, 0, 0); DEFINE_TAG(2, 0, 1); DEFINE_TAG(2, 0, 2); DEFINE_TAG(2, 0, 3);
    DEFINE_TAG(2, 1, 0); DEFINE_TAG(2, 1, 1); DEFINE_TAG(2, 1, 2); DEFINE_TAG(2, 1, 3);
    DEFINE_TAG(2, 2, 0); DEFINE_TAG(2, 2, 1); DEFINE_TAG(2, 2, 2); DEFINE_TAG(2, 2, 3);
    DEFINE_TAG(2, 3, 0); DEFINE_TAG(2, 3, 1); DEFINE_TAG(2, 3, 2); DEFINE_TAG(2, 3, 3);
    DEFINE_TAG(3, 0, 0); DEFINE_TAG(3, 0, 1); DEFINE_TAG(3, 0, 2); DEFINE_TAG(3, 0, 3);
    DEFINE_TAG(3, 1, 0); DEFINE_TAG(3, 1, 1); DEFINE_TAG(3, 1, 2); DEFINE_TAG(3, 1, 3);
    DEFINE_TAG(3, 2, 0); DEFINE_TAG(3, 2, 1); DEFINE_TAG(3, 2, 2); DEFINE_TAG(3, 2, 3);
    DEFINE_TAG(3, 3, 0); DEFINE_TAG(3, 3, 1); DEFINE_TAG(3, 3, 2); DEFINE_TAG(3, 3, 3);
    DEFINE_TAG(0, 0, 0, 0); DEFINE_TAG(0, 0, 0, 1); DEFINE_TAG(0, 0, 0, 2); DEFINE_TAG(0, 0, 0, 3);
    DEFINE_TAG(0, 0, 1, 0); DEFINE_TAG(0, 0, 1, 1); DEFINE_TAG(0, 0, 1, 2); DEFINE_TAG(0, 0, 1, 3);
    DEFINE_TAG(0, 0, 2, 0); DEFINE_TAG(0, 0, 2, 1); DEFINE_TAG(0, 0, 2, 2); DEFINE_TAG(0, 0, 2, 3);
    DEFINE_TAG(0, 0, 3, 0); DEFINE_TAG(0, 0, 3, 1); DEFINE_TAG(0, 0, 3, 2); DEFINE_TAG(0, 0, 3, 3);
    DEFINE_TAG(0, 1, 0, 0); DEFINE_TAG(0, 1, 0, 1); DEFINE_TAG(0, 1, 0, 2); DEFINE_TAG(0, 1, 0, 3);
    DEFINE_TAG(0, 1, 1, 0); DEFINE_TAG(0, 1, 1, 1); DEFINE_TAG(0, 1, 1, 2); DEFINE_TAG(0, 1, 1, 3);
    DEFINE_TAG(0, 1, 2, 0); DEFINE_TAG(0, 1, 2, 1); DEFINE_TAG(0, 1, 2, 2); DEFINE_TAG(0, 1, 2, 3);
    DEFINE_TAG(0, 1, 3, 0); DEFINE_TAG(0, 1, 3, 1); DEFINE_TAG(0, 1, 3, 2); DEFINE_TAG(0, 1, 3, 3);
    DEFINE_TAG(0, 2, 0, 0); DEFINE_TAG(0, 2, 0, 1); DEFINE_TAG(0, 2, 0, 2); DEFINE_TAG(0, 2, 0, 3);
    DEFINE_TAG(0, 2, 1, 0); DEFINE_TAG(0, 2, 1, 1); DEFINE_TAG(0, 2, 1, 2); DEFINE_TAG(0, 2, 1, 3);
    DEFINE_TAG(0, 2, 2, 0); DEFINE_TAG(0, 2, 2, 1); DEFINE_TAG(0, 2, 2, 2); DEFINE_TAG(0, 2, 2, 3);
    DEFINE_TAG(0, 2, 3, 0); DEFINE_TAG(0, 2, 3, 1); DEFINE_TAG(0, 2, 3, 2); DEFINE_TAG(0, 2, 3, 3);
    DEFINE_TAG(0, 3, 0, 0); DEFINE_TAG(0, 3, 0, 1); DEFINE_TAG(0, 3, 0, 2); DEFINE_TAG(0, 3, 0, 3);
    DEFINE_TAG(0, 3, 1, 0); DEFINE_TAG(0, 3, 1, 1); DEFINE_TAG(0, 3, 1, 2); DEFINE_TAG(0, 3, 1, 3);
    DEFINE_TAG(0, 3, 2, 0); DEFINE_TAG(0, 3, 2, 1); DEFINE_TAG(0, 3, 2, 2); DEFINE_TAG(0, 3, 2, 3);
    DEFINE_TAG(0, 3, 3, 0); DEFINE_TAG(0, 3, 3, 1); DEFINE_TAG(0, 3, 3, 2); DEFINE_TAG(0, 3, 3, 3);
    DEFINE_TAG(1, 0, 0, 0); DEFINE_TAG(1, 0, 0, 1); DEFINE_TAG(1, 0, 0, 2); DEFINE_TAG(1, 0, 0, 3);
    DEFINE_TAG(1, 0, 1, 0); DEFINE_TAG(1, 0, 1, 1); DEFINE_TAG(1, 0, 1, 2); DEFINE_TAG(1, 0, 1, 3);
    DEFINE_TAG(1, 0, 2, 0); DEFINE_TAG(1, 0, 2, 1); DEFINE_TAG(1, 0, 2, 2); DEFINE_TAG(1, 0, 2, 3);
    DEFINE_TAG(1, 0, 3, 0); DEFINE_TAG(1, 0, 3, 1); DEFINE_TAG(1, 0, 3, 2); DEFINE_TAG(1, 0, 3, 3);
    DEFINE_TAG(1, 1, 0, 0); DEFINE_TAG(1, 1, 0, 1); DEFINE_TAG(1, 1, 0, 2); DEFINE_TAG(1, 1, 0, 3);
    DEFINE_TAG(1, 1, 1, 0); DEFINE_TAG(1, 1, 1, 1); DEFINE_TAG(1, 1, 1, 2); DEFINE_TAG(1, 1, 1, 3);
    DEFINE_TAG(1, 1, 2, 0); DEFINE_TAG(1, 1, 2, 1); DEFINE_TAG(1, 1, 2, 2); DEFINE_TAG(1, 1, 2, 3);
    DEFINE_TAG(1, 1, 3, 0); DEFINE_TAG(1, 1, 3, 1); DEFINE_TAG(1, 1, 3, 2); DEFINE_TAG(1, 1, 3, 3);
    DEFINE_TAG(1, 2, 0, 0); DEFINE_TAG(1, 2, 0, 1); DEFINE_TAG(1, 2, 0, 2); DEFINE_TAG(1, 2, 0, 3);
    DEFINE_TAG(1, 2, 1, 0); DEFINE_TAG(1, 2, 1, 1); DEFINE_TAG(1, 2, 1, 2); DEFINE_TAG(1, 2, 1, 3);
    DEFINE_TAG(1, 2, 2, 0); DEFINE_TAG(1, 2, 2, 1); DEFINE_TAG(1, 2, 2, 2); DEFINE_TAG(1, 2, 2, 3);
    DEFINE_TAG(1, 2, 3, 0); DEFINE_TAG(1, 2, 3, 1); DEFINE_TAG(1, 2, 3, 2); DEFINE_TAG(1, 2, 3, 3);
    DEFINE_TAG(1, 3, 0, 0); DEFINE_TAG(1, 3, 0, 1); DEFINE_TAG(1, 3, 0, 2); DEFINE_TAG(1, 3, 0, 3);
    DEFINE_TAG(1, 3, 1, 0); DEFINE_TAG(1, 3, 1, 1); DEFINE_TAG(1, 3, 1, 2); DEFINE_TAG(1, 3, 1, 3);
    DEFINE_TAG(1, 3, 2, 0); DEFINE_TAG(1, 3, 2, 1); DEFINE_TAG(1, 3, 2, 2); DEFINE_TAG(1, 3, 2, 3);
    DEFINE_TAG(1, 3, 3, 0); DEFINE_TAG(1, 3, 3, 1); DEFINE_TAG(1, 3, 3, 2); DEFINE_TAG(1, 3, 3, 3);
    DEFINE_TAG(2, 0, 0, 0); DEFINE_TAG(2, 0, 0, 1); DEFINE_TAG(2, 0, 0, 2); DEFINE_TAG(2, 0, 0, 3);
    DEFINE_TAG(2, 0, 1, 0); DEFINE_TAG(2, 0, 1, 1); DEFINE_TAG(2, 0, 1, 2); DEFINE_TAG(2, 0, 1, 3);
    DEFINE_TAG(2, 0, 2, 0); DEFINE_TAG(2, 0, 2, 1); DEFINE_TAG(2, 0, 2, 2); DEFINE_TAG(2, 0, 2, 3);
    DEFINE_TAG(2, 0, 3, 0); DEFINE_TAG(2, 0, 3, 1); DEFINE_TAG(2, 0, 3, 2); DEFINE_TAG(2, 0, 3, 3);
    DEFINE_TAG(2, 1, 0, 0); DEFINE_TAG(2, 1, 0, 1); DEFINE_TAG(2, 1, 0, 2); DEFINE_TAG(2, 1, 0, 3);
    DEFINE_TAG(2, 1, 1, 0); DEFINE_TAG(2, 1, 1, 1); DEFINE_TAG(2, 1, 1, 2); DEFINE_TAG(2, 1, 1, 3);
    DEFINE_TAG(2, 1, 2, 0); DEFINE_TAG(2, 1, 2, 1); DEFINE_TAG(2, 1, 2, 2); DEFINE_TAG(2, 1, 2, 3);
    DEFINE_TAG(2, 1, 3, 0); DEFINE_TAG(2, 1, 3, 1); DEFINE_TAG(2, 1, 3, 2); DEFINE_TAG(2, 1, 3, 3);
    DEFINE_TAG(2, 2, 0, 0); DEFINE_TAG(2, 2, 0, 1); DEFINE_TAG(2, 2, 0, 2); DEFINE_TAG(2, 2, 0, 3);
    DEFINE_TAG(2, 2, 1, 0); DEFINE_TAG(2, 2, 1, 1); DEFINE_TAG(2, 2, 1, 2); DEFINE_TAG(2, 2, 1, 3);
    DEFINE_TAG(2, 2, 2, 0); DEFINE_TAG(2, 2, 2, 1); DEFINE_TAG(2, 2, 2, 2); DEFINE_TAG(2, 2, 2, 3);
    DEFINE_TAG(2, 2, 3, 0); DEFINE_TAG(2, 2, 3, 1); DEFINE_TAG(2, 2, 3, 2); DEFINE_TAG(2, 2, 3, 3);
    DEFINE_TAG(2, 3, 0, 0); DEFINE_TAG(2, 3, 0, 1); DEFINE_TAG(2, 3, 0, 2); DEFINE_TAG(2, 3, 0, 3);
    DEFINE_TAG(2, 3, 1, 0); DEFINE_TAG(2, 3, 1, 1); DEFINE_TAG(2, 3, 1, 2); DEFINE_TAG(2, 3, 1, 3);
    DEFINE_TAG(2, 3, 2, 0); DEFINE_TAG(2, 3, 2, 1); DEFINE_TAG(2, 3, 2, 2); DEFINE_TAG(2, 3, 2, 3);
    DEFINE_TAG(2, 3, 3, 0); DEFINE_TAG(2, 3, 3, 1); DEFINE_TAG(2, 3, 3, 2); DEFINE_TAG(2, 3, 3, 3);
    DEFINE_TAG(3, 0, 0, 0); DEFINE_TAG(3, 0, 0, 1); DEFINE_TAG(3, 0, 0, 2); DEFINE_TAG(3, 0, 0, 3);
    DEFINE_TAG(3, 0, 1, 0); DEFINE_TAG(3, 0, 1, 1); DEFINE_TAG(3, 0, 1, 2); DEFINE_TAG(3, 0, 1, 3);
    DEFINE_TAG(3, 0, 2, 0); DEFINE_TAG(3, 0, 2, 1); DEFINE_TAG(3, 0, 2, 2); DEFINE_TAG(3, 0, 2, 3);
    DEFINE_TAG(3, 0, 3, 0); DEFINE_TAG(3, 0, 3, 1); DEFINE_TAG(3, 0, 3, 2); DEFINE_TAG(3, 0, 3, 3);
    DEFINE_TAG(3, 1, 0, 0); DEFINE_TAG(3, 1, 0, 1); DEFINE_TAG(3, 1, 0, 2); DEFINE_TAG(3, 1, 0, 3);
    DEFINE_TAG(3, 1, 1, 0); DEFINE_TAG(3, 1, 1, 1); DEFINE_TAG(3, 1, 1, 2); DEFINE_TAG(3, 1, 1, 3);
    DEFINE_TAG(3, 1, 2, 0); DEFINE_TAG(3, 1, 2, 1); DEFINE_TAG(3, 1, 2, 2); DEFINE_TAG(3, 1, 2, 3);
    DEFINE_TAG(3, 1, 3, 0); DEFINE_TAG(3, 1, 3, 1); DEFINE_TAG(3, 1, 3, 2); DEFINE_TAG(3, 1, 3, 3);
    DEFINE_TAG(3, 2, 0, 0); DEFINE_TAG(3, 2, 0, 1); DEFINE_TAG(3, 2, 0, 2); DEFINE_TAG(3, 2, 0, 3);
    DEFINE_TAG(3, 2, 1, 0); DEFINE_TAG(3, 2, 1, 1); DEFINE_TAG(3, 2, 1, 2); DEFINE_TAG(3, 2, 1, 3);
    DEFINE_TAG(3, 2, 2, 0); DEFINE_TAG(3, 2, 2, 1); DEFINE_TAG(3, 2, 2, 2); DEFINE_TAG(3, 2, 2, 3);
    DEFINE_TAG(3, 2, 3, 0); DEFINE_TAG(3, 2, 3, 1); DEFINE_TAG(3, 2, 3, 2); DEFINE_TAG(3, 2, 3, 3);
    DEFINE_TAG(3, 3, 0, 0); DEFINE_TAG(3, 3, 0, 1); DEFINE_TAG(3, 3, 0, 2); DEFINE_TAG(3, 3, 0, 3);
    DEFINE_TAG(3, 3, 1, 0); DEFINE_TAG(3, 3, 1, 1); DEFINE_TAG(3, 3, 1, 2); DEFINE_TAG(3, 3, 1, 3);
    DEFINE_TAG(3, 3, 2, 0); DEFINE_TAG(3, 3, 2, 1); DEFINE_TAG(3, 3, 2, 2); DEFINE_TAG(3, 3, 2, 3);
    DEFINE_TAG(3, 3, 3, 0); DEFINE_TAG(3, 3, 3, 1); DEFINE_TAG(3, 3, 3, 2); DEFINE_TAG(3, 3, 3, 3);

    // -------------------------------------------------------------------------------------------------------------------------------------- //

    template<typename T, std::size_t N>
    class vector {};

#undef VEC_TEMPLATE
#define VEC_TEMPLATE swizzle_vector

#define SIZE 1

    template<typename T>
    class vector<T, 1> : public vector_base<vector<T, 1>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 1;

        union
        {
            container<value_type, size> data;

            DEFINE_SWIZZLE(0);
            DEFINE_SWIZZLE(0, 0);
            DEFINE_SWIZZLE(0, 0, 0);
            DEFINE_SWIZZLE(0, 0, 0, 0);
        };

        constexpr CPU_GPU vector() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU vector(T x);
        constexpr CPU_GPU vector(const vector& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU vector(vector&& mv) requires std::is_move_constructible_v<value_type>;

        template<vector_like... Vectors>
        constexpr CPU_GPU vector(const Vectors&... vecs) requires ((... + std::remove_cvref_t<Vectors>::size) == size) && std::conjunction_v<std::is_same<value_type, typename std::remove_cvref_t<Vectors>::value_type>...>;

        using vector_base<vector>::operator=;

        CPU_GPU vector& operator=(const vector& cpy);
        CPU_GPU vector& operator=(vector&& mv);

        constexpr CPU_GPU operator T() const; // trivially convertible to just T;

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T> requires (!vector_like<T>)
    vector(T x) -> vector<T, 1>;

#undef SIZE
#define SIZE 2

    template<typename T>
    class vector<T, 2> : public vector_base<vector<T, 2>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 2;

        union
        {
            container<value_type, size> data;

            DEFINE_SWIZZLE(0); DEFINE_SWIZZLE(1);

            DEFINE_SWIZZLE(0, 0); DEFINE_SWIZZLE(0, 1);
            DEFINE_SWIZZLE(1, 0); DEFINE_SWIZZLE(1, 1);

            DEFINE_SWIZZLE(0, 0, 0); DEFINE_SWIZZLE(0, 0, 1);
            DEFINE_SWIZZLE(0, 1, 0); DEFINE_SWIZZLE(0, 1, 1);
            DEFINE_SWIZZLE(1, 0, 0); DEFINE_SWIZZLE(1, 0, 1);
            DEFINE_SWIZZLE(1, 1, 0); DEFINE_SWIZZLE(1, 1, 1);

            DEFINE_SWIZZLE(0, 0, 0, 0); DEFINE_SWIZZLE(0, 0, 0, 1);
            DEFINE_SWIZZLE(0, 0, 1, 0); DEFINE_SWIZZLE(0, 0, 1, 1);
            DEFINE_SWIZZLE(0, 1, 0, 0); DEFINE_SWIZZLE(0, 1, 0, 1);
            DEFINE_SWIZZLE(0, 1, 1, 0); DEFINE_SWIZZLE(0, 1, 1, 1);
            DEFINE_SWIZZLE(1, 0, 0, 0); DEFINE_SWIZZLE(1, 0, 0, 1);
            DEFINE_SWIZZLE(1, 0, 1, 0); DEFINE_SWIZZLE(1, 0, 1, 1);
            DEFINE_SWIZZLE(1, 1, 0, 0); DEFINE_SWIZZLE(1, 1, 0, 1);
            DEFINE_SWIZZLE(1, 1, 1, 0); DEFINE_SWIZZLE(1, 1, 1, 1);
        };

        constexpr CPU_GPU vector() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU vector(T x, T y);
        constexpr CPU_GPU vector(const vector& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU vector(vector&& mv) requires std::is_move_constructible_v<value_type>;

        template<vector_like... Vectors>
        constexpr CPU_GPU vector(const Vectors&... vecs) requires ((... + std::remove_cvref_t<Vectors>::size) == size) && std::conjunction_v<std::is_same<value_type, typename std::remove_cvref_t<Vectors>::value_type>...>;

        using vector_base<vector>::operator=;

        CPU_GPU vector& operator=(const vector& cpy);
        CPU_GPU vector& operator=(vector&& mv);

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T> requires (!vector_like<T>)
    vector(T x, T y) -> vector<T, 2>;

#undef SIZE
#define SIZE 3

    template<typename T>
    class vector<T, 3> : public vector_base<vector<T, 3>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 3;

        union
        {
            container<value_type, size> data;

            DEFINE_SWIZZLE(0); DEFINE_SWIZZLE(1); DEFINE_SWIZZLE(2);

            DEFINE_SWIZZLE(0, 0); DEFINE_SWIZZLE(0, 1); DEFINE_SWIZZLE(0, 2);
            DEFINE_SWIZZLE(1, 0); DEFINE_SWIZZLE(1, 1); DEFINE_SWIZZLE(1, 2);
            DEFINE_SWIZZLE(2, 0); DEFINE_SWIZZLE(2, 1); DEFINE_SWIZZLE(2, 2);

            DEFINE_SWIZZLE(0, 0, 0); DEFINE_SWIZZLE(0, 0, 1); DEFINE_SWIZZLE(0, 0, 2);
            DEFINE_SWIZZLE(0, 1, 0); DEFINE_SWIZZLE(0, 1, 1); DEFINE_SWIZZLE(0, 1, 2);
            DEFINE_SWIZZLE(0, 2, 0); DEFINE_SWIZZLE(0, 2, 1); DEFINE_SWIZZLE(0, 2, 2);
            DEFINE_SWIZZLE(1, 0, 0); DEFINE_SWIZZLE(1, 0, 1); DEFINE_SWIZZLE(1, 0, 2);
            DEFINE_SWIZZLE(1, 1, 0); DEFINE_SWIZZLE(1, 1, 1); DEFINE_SWIZZLE(1, 1, 2);
            DEFINE_SWIZZLE(1, 2, 0); DEFINE_SWIZZLE(1, 2, 1); DEFINE_SWIZZLE(1, 2, 2);
            DEFINE_SWIZZLE(2, 0, 0); DEFINE_SWIZZLE(2, 0, 1); DEFINE_SWIZZLE(2, 0, 2);
            DEFINE_SWIZZLE(2, 1, 0); DEFINE_SWIZZLE(2, 1, 1); DEFINE_SWIZZLE(2, 1, 2);
            DEFINE_SWIZZLE(2, 2, 0); DEFINE_SWIZZLE(2, 2, 1); DEFINE_SWIZZLE(2, 2, 2);

            DEFINE_SWIZZLE(0, 0, 0, 0); DEFINE_SWIZZLE(0, 0, 0, 1); DEFINE_SWIZZLE(0, 0, 0, 2);
            DEFINE_SWIZZLE(0, 0, 1, 0); DEFINE_SWIZZLE(0, 0, 1, 1); DEFINE_SWIZZLE(0, 0, 1, 2);
            DEFINE_SWIZZLE(0, 0, 2, 0); DEFINE_SWIZZLE(0, 0, 2, 1); DEFINE_SWIZZLE(0, 0, 2, 2);
            DEFINE_SWIZZLE(0, 1, 0, 0); DEFINE_SWIZZLE(0, 1, 0, 1); DEFINE_SWIZZLE(0, 1, 0, 2);
            DEFINE_SWIZZLE(0, 1, 1, 0); DEFINE_SWIZZLE(0, 1, 1, 1); DEFINE_SWIZZLE(0, 1, 1, 2);
            DEFINE_SWIZZLE(0, 1, 2, 0); DEFINE_SWIZZLE(0, 1, 2, 1); DEFINE_SWIZZLE(0, 1, 2, 2);
            DEFINE_SWIZZLE(0, 2, 0, 0); DEFINE_SWIZZLE(0, 2, 0, 1); DEFINE_SWIZZLE(0, 2, 0, 2);
            DEFINE_SWIZZLE(0, 2, 1, 0); DEFINE_SWIZZLE(0, 2, 1, 1); DEFINE_SWIZZLE(0, 2, 1, 2);
            DEFINE_SWIZZLE(0, 2, 2, 0); DEFINE_SWIZZLE(0, 2, 2, 1); DEFINE_SWIZZLE(0, 2, 2, 2);
            DEFINE_SWIZZLE(1, 0, 0, 0); DEFINE_SWIZZLE(1, 0, 0, 1); DEFINE_SWIZZLE(1, 0, 0, 2);
            DEFINE_SWIZZLE(1, 0, 1, 0); DEFINE_SWIZZLE(1, 0, 1, 1); DEFINE_SWIZZLE(1, 0, 1, 2);
            DEFINE_SWIZZLE(1, 0, 2, 0); DEFINE_SWIZZLE(1, 0, 2, 1); DEFINE_SWIZZLE(1, 0, 2, 2);
            DEFINE_SWIZZLE(1, 1, 0, 0); DEFINE_SWIZZLE(1, 1, 0, 1); DEFINE_SWIZZLE(1, 1, 0, 2);
            DEFINE_SWIZZLE(1, 1, 1, 0); DEFINE_SWIZZLE(1, 1, 1, 1); DEFINE_SWIZZLE(1, 1, 1, 2);
            DEFINE_SWIZZLE(1, 1, 2, 0); DEFINE_SWIZZLE(1, 1, 2, 1); DEFINE_SWIZZLE(1, 1, 2, 2);
            DEFINE_SWIZZLE(1, 2, 0, 0); DEFINE_SWIZZLE(1, 2, 0, 1); DEFINE_SWIZZLE(1, 2, 0, 2);
            DEFINE_SWIZZLE(1, 2, 1, 0); DEFINE_SWIZZLE(1, 2, 1, 1); DEFINE_SWIZZLE(1, 2, 1, 2);
            DEFINE_SWIZZLE(1, 2, 2, 0); DEFINE_SWIZZLE(1, 2, 2, 1); DEFINE_SWIZZLE(1, 2, 2, 2);
            DEFINE_SWIZZLE(2, 0, 0, 0); DEFINE_SWIZZLE(2, 0, 0, 1); DEFINE_SWIZZLE(2, 0, 0, 2);
            DEFINE_SWIZZLE(2, 0, 1, 0); DEFINE_SWIZZLE(2, 0, 1, 1); DEFINE_SWIZZLE(2, 0, 1, 2);
            DEFINE_SWIZZLE(2, 0, 2, 0); DEFINE_SWIZZLE(2, 0, 2, 1); DEFINE_SWIZZLE(2, 0, 2, 2);
            DEFINE_SWIZZLE(2, 1, 0, 0); DEFINE_SWIZZLE(2, 1, 0, 1); DEFINE_SWIZZLE(2, 1, 0, 2);
            DEFINE_SWIZZLE(2, 1, 1, 0); DEFINE_SWIZZLE(2, 1, 1, 1); DEFINE_SWIZZLE(2, 1, 1, 2);
            DEFINE_SWIZZLE(2, 1, 2, 0); DEFINE_SWIZZLE(2, 1, 2, 1); DEFINE_SWIZZLE(2, 1, 2, 2);
            DEFINE_SWIZZLE(2, 2, 0, 0); DEFINE_SWIZZLE(2, 2, 0, 1); DEFINE_SWIZZLE(2, 2, 0, 2);
            DEFINE_SWIZZLE(2, 2, 1, 0); DEFINE_SWIZZLE(2, 2, 1, 1); DEFINE_SWIZZLE(2, 2, 1, 2);
            DEFINE_SWIZZLE(2, 2, 2, 0); DEFINE_SWIZZLE(2, 2, 2, 1); DEFINE_SWIZZLE(2, 2, 2, 2);
        };

        constexpr CPU_GPU vector() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU vector(T x, T y, T z);
        constexpr CPU_GPU vector(const vector& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU vector(vector&& mv) requires std::is_move_constructible_v<value_type>;

        template<vector_like... Vectors>
        constexpr CPU_GPU vector(const Vectors&... vecs) requires ((... + std::remove_cvref_t<Vectors>::size) == size) && std::conjunction_v<std::is_same<value_type, typename std::remove_cvref_t<Vectors>::value_type>...>;

        using vector_base<vector>::operator=;

        CPU_GPU vector& operator=(const vector& cpy);
        CPU_GPU vector& operator=(vector&& mv);

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T> requires (!vector_like<T>)
    vector(T x, T y, T z) -> vector<T, 3>;

#undef SIZE
#define SIZE 4

    template<typename T>
    class vector<T, 4> : public vector_base<vector<T, 4>>
    {
    public:
        using value_type = T;
        constexpr static std::size_t size = 4;

        union
        {
            container<value_type, size> data;

            DEFINE_SWIZZLE(0); DEFINE_SWIZZLE(1); DEFINE_SWIZZLE(2); DEFINE_SWIZZLE(3);

            DEFINE_SWIZZLE(0, 0); DEFINE_SWIZZLE(0, 1); DEFINE_SWIZZLE(0, 2); DEFINE_SWIZZLE(0, 3);
            DEFINE_SWIZZLE(1, 0); DEFINE_SWIZZLE(1, 1); DEFINE_SWIZZLE(1, 2); DEFINE_SWIZZLE(1, 3);
            DEFINE_SWIZZLE(2, 0); DEFINE_SWIZZLE(2, 1); DEFINE_SWIZZLE(2, 2); DEFINE_SWIZZLE(2, 3);
            DEFINE_SWIZZLE(3, 0); DEFINE_SWIZZLE(3, 1); DEFINE_SWIZZLE(3, 2); DEFINE_SWIZZLE(3, 3);

            DEFINE_SWIZZLE(0, 0, 0); DEFINE_SWIZZLE(0, 0, 1); DEFINE_SWIZZLE(0, 0, 2); DEFINE_SWIZZLE(0, 0, 3);
            DEFINE_SWIZZLE(0, 1, 0); DEFINE_SWIZZLE(0, 1, 1); DEFINE_SWIZZLE(0, 1, 2); DEFINE_SWIZZLE(0, 1, 3);
            DEFINE_SWIZZLE(0, 2, 0); DEFINE_SWIZZLE(0, 2, 1); DEFINE_SWIZZLE(0, 2, 2); DEFINE_SWIZZLE(0, 2, 3);
            DEFINE_SWIZZLE(0, 3, 0); DEFINE_SWIZZLE(0, 3, 1); DEFINE_SWIZZLE(0, 3, 2); DEFINE_SWIZZLE(0, 3, 3);
            DEFINE_SWIZZLE(1, 0, 0); DEFINE_SWIZZLE(1, 0, 1); DEFINE_SWIZZLE(1, 0, 2); DEFINE_SWIZZLE(1, 0, 3);
            DEFINE_SWIZZLE(1, 1, 0); DEFINE_SWIZZLE(1, 1, 1); DEFINE_SWIZZLE(1, 1, 2); DEFINE_SWIZZLE(1, 1, 3);
            DEFINE_SWIZZLE(1, 2, 0); DEFINE_SWIZZLE(1, 2, 1); DEFINE_SWIZZLE(1, 2, 2); DEFINE_SWIZZLE(1, 2, 3);
            DEFINE_SWIZZLE(1, 3, 0); DEFINE_SWIZZLE(1, 3, 1); DEFINE_SWIZZLE(1, 3, 2); DEFINE_SWIZZLE(1, 3, 3);
            DEFINE_SWIZZLE(2, 0, 0); DEFINE_SWIZZLE(2, 0, 1); DEFINE_SWIZZLE(2, 0, 2); DEFINE_SWIZZLE(2, 0, 3);
            DEFINE_SWIZZLE(2, 1, 0); DEFINE_SWIZZLE(2, 1, 1); DEFINE_SWIZZLE(2, 1, 2); DEFINE_SWIZZLE(2, 1, 3);
            DEFINE_SWIZZLE(2, 2, 0); DEFINE_SWIZZLE(2, 2, 1); DEFINE_SWIZZLE(2, 2, 2); DEFINE_SWIZZLE(2, 2, 3);
            DEFINE_SWIZZLE(2, 3, 0); DEFINE_SWIZZLE(2, 3, 1); DEFINE_SWIZZLE(2, 3, 2); DEFINE_SWIZZLE(2, 3, 3);
            DEFINE_SWIZZLE(3, 0, 0); DEFINE_SWIZZLE(3, 0, 1); DEFINE_SWIZZLE(3, 0, 2); DEFINE_SWIZZLE(3, 0, 3);
            DEFINE_SWIZZLE(3, 1, 0); DEFINE_SWIZZLE(3, 1, 1); DEFINE_SWIZZLE(3, 1, 2); DEFINE_SWIZZLE(3, 1, 3);
            DEFINE_SWIZZLE(3, 2, 0); DEFINE_SWIZZLE(3, 2, 1); DEFINE_SWIZZLE(3, 2, 2); DEFINE_SWIZZLE(3, 2, 3);
            DEFINE_SWIZZLE(3, 3, 0); DEFINE_SWIZZLE(3, 3, 1); DEFINE_SWIZZLE(3, 3, 2); DEFINE_SWIZZLE(3, 3, 3);

            DEFINE_SWIZZLE(0, 0, 0, 0); DEFINE_SWIZZLE(0, 0, 0, 1); DEFINE_SWIZZLE(0, 0, 0, 2); DEFINE_SWIZZLE(0, 0, 0, 3);
            DEFINE_SWIZZLE(0, 0, 1, 0); DEFINE_SWIZZLE(0, 0, 1, 1); DEFINE_SWIZZLE(0, 0, 1, 2); DEFINE_SWIZZLE(0, 0, 1, 3);
            DEFINE_SWIZZLE(0, 0, 2, 0); DEFINE_SWIZZLE(0, 0, 2, 1); DEFINE_SWIZZLE(0, 0, 2, 2); DEFINE_SWIZZLE(0, 0, 2, 3);
            DEFINE_SWIZZLE(0, 0, 3, 0); DEFINE_SWIZZLE(0, 0, 3, 1); DEFINE_SWIZZLE(0, 0, 3, 2); DEFINE_SWIZZLE(0, 0, 3, 3);
            DEFINE_SWIZZLE(0, 1, 0, 0); DEFINE_SWIZZLE(0, 1, 0, 1); DEFINE_SWIZZLE(0, 1, 0, 2); DEFINE_SWIZZLE(0, 1, 0, 3);
            DEFINE_SWIZZLE(0, 1, 1, 0); DEFINE_SWIZZLE(0, 1, 1, 1); DEFINE_SWIZZLE(0, 1, 1, 2); DEFINE_SWIZZLE(0, 1, 1, 3);
            DEFINE_SWIZZLE(0, 1, 2, 0); DEFINE_SWIZZLE(0, 1, 2, 1); DEFINE_SWIZZLE(0, 1, 2, 2); DEFINE_SWIZZLE(0, 1, 2, 3);
            DEFINE_SWIZZLE(0, 1, 3, 0); DEFINE_SWIZZLE(0, 1, 3, 1); DEFINE_SWIZZLE(0, 1, 3, 2); DEFINE_SWIZZLE(0, 1, 3, 3);
            DEFINE_SWIZZLE(0, 2, 0, 0); DEFINE_SWIZZLE(0, 2, 0, 1); DEFINE_SWIZZLE(0, 2, 0, 2); DEFINE_SWIZZLE(0, 2, 0, 3);
            DEFINE_SWIZZLE(0, 2, 1, 0); DEFINE_SWIZZLE(0, 2, 1, 1); DEFINE_SWIZZLE(0, 2, 1, 2); DEFINE_SWIZZLE(0, 2, 1, 3);
            DEFINE_SWIZZLE(0, 2, 2, 0); DEFINE_SWIZZLE(0, 2, 2, 1); DEFINE_SWIZZLE(0, 2, 2, 2); DEFINE_SWIZZLE(0, 2, 2, 3);
            DEFINE_SWIZZLE(0, 2, 3, 0); DEFINE_SWIZZLE(0, 2, 3, 1); DEFINE_SWIZZLE(0, 2, 3, 2); DEFINE_SWIZZLE(0, 2, 3, 3);
            DEFINE_SWIZZLE(0, 3, 0, 0); DEFINE_SWIZZLE(0, 3, 0, 1); DEFINE_SWIZZLE(0, 3, 0, 2); DEFINE_SWIZZLE(0, 3, 0, 3);
            DEFINE_SWIZZLE(0, 3, 1, 0); DEFINE_SWIZZLE(0, 3, 1, 1); DEFINE_SWIZZLE(0, 3, 1, 2); DEFINE_SWIZZLE(0, 3, 1, 3);
            DEFINE_SWIZZLE(0, 3, 2, 0); DEFINE_SWIZZLE(0, 3, 2, 1); DEFINE_SWIZZLE(0, 3, 2, 2); DEFINE_SWIZZLE(0, 3, 2, 3);
            DEFINE_SWIZZLE(0, 3, 3, 0); DEFINE_SWIZZLE(0, 3, 3, 1); DEFINE_SWIZZLE(0, 3, 3, 2); DEFINE_SWIZZLE(0, 3, 3, 3);
            DEFINE_SWIZZLE(1, 0, 0, 0); DEFINE_SWIZZLE(1, 0, 0, 1); DEFINE_SWIZZLE(1, 0, 0, 2); DEFINE_SWIZZLE(1, 0, 0, 3);
            DEFINE_SWIZZLE(1, 0, 1, 0); DEFINE_SWIZZLE(1, 0, 1, 1); DEFINE_SWIZZLE(1, 0, 1, 2); DEFINE_SWIZZLE(1, 0, 1, 3);
            DEFINE_SWIZZLE(1, 0, 2, 0); DEFINE_SWIZZLE(1, 0, 2, 1); DEFINE_SWIZZLE(1, 0, 2, 2); DEFINE_SWIZZLE(1, 0, 2, 3);
            DEFINE_SWIZZLE(1, 0, 3, 0); DEFINE_SWIZZLE(1, 0, 3, 1); DEFINE_SWIZZLE(1, 0, 3, 2); DEFINE_SWIZZLE(1, 0, 3, 3);
            DEFINE_SWIZZLE(1, 1, 0, 0); DEFINE_SWIZZLE(1, 1, 0, 1); DEFINE_SWIZZLE(1, 1, 0, 2); DEFINE_SWIZZLE(1, 1, 0, 3);
            DEFINE_SWIZZLE(1, 1, 1, 0); DEFINE_SWIZZLE(1, 1, 1, 1); DEFINE_SWIZZLE(1, 1, 1, 2); DEFINE_SWIZZLE(1, 1, 1, 3);
            DEFINE_SWIZZLE(1, 1, 2, 0); DEFINE_SWIZZLE(1, 1, 2, 1); DEFINE_SWIZZLE(1, 1, 2, 2); DEFINE_SWIZZLE(1, 1, 2, 3);
            DEFINE_SWIZZLE(1, 1, 3, 0); DEFINE_SWIZZLE(1, 1, 3, 1); DEFINE_SWIZZLE(1, 1, 3, 2); DEFINE_SWIZZLE(1, 1, 3, 3);
            DEFINE_SWIZZLE(1, 2, 0, 0); DEFINE_SWIZZLE(1, 2, 0, 1); DEFINE_SWIZZLE(1, 2, 0, 2); DEFINE_SWIZZLE(1, 2, 0, 3);
            DEFINE_SWIZZLE(1, 2, 1, 0); DEFINE_SWIZZLE(1, 2, 1, 1); DEFINE_SWIZZLE(1, 2, 1, 2); DEFINE_SWIZZLE(1, 2, 1, 3);
            DEFINE_SWIZZLE(1, 2, 2, 0); DEFINE_SWIZZLE(1, 2, 2, 1); DEFINE_SWIZZLE(1, 2, 2, 2); DEFINE_SWIZZLE(1, 2, 2, 3);
            DEFINE_SWIZZLE(1, 2, 3, 0); DEFINE_SWIZZLE(1, 2, 3, 1); DEFINE_SWIZZLE(1, 2, 3, 2); DEFINE_SWIZZLE(1, 2, 3, 3);
            DEFINE_SWIZZLE(1, 3, 0, 0); DEFINE_SWIZZLE(1, 3, 0, 1); DEFINE_SWIZZLE(1, 3, 0, 2); DEFINE_SWIZZLE(1, 3, 0, 3);
            DEFINE_SWIZZLE(1, 3, 1, 0); DEFINE_SWIZZLE(1, 3, 1, 1); DEFINE_SWIZZLE(1, 3, 1, 2); DEFINE_SWIZZLE(1, 3, 1, 3);
            DEFINE_SWIZZLE(1, 3, 2, 0); DEFINE_SWIZZLE(1, 3, 2, 1); DEFINE_SWIZZLE(1, 3, 2, 2); DEFINE_SWIZZLE(1, 3, 2, 3);
            DEFINE_SWIZZLE(1, 3, 3, 0); DEFINE_SWIZZLE(1, 3, 3, 1); DEFINE_SWIZZLE(1, 3, 3, 2); DEFINE_SWIZZLE(1, 3, 3, 3);
            DEFINE_SWIZZLE(2, 0, 0, 0); DEFINE_SWIZZLE(2, 0, 0, 1); DEFINE_SWIZZLE(2, 0, 0, 2); DEFINE_SWIZZLE(2, 0, 0, 3);
            DEFINE_SWIZZLE(2, 0, 1, 0); DEFINE_SWIZZLE(2, 0, 1, 1); DEFINE_SWIZZLE(2, 0, 1, 2); DEFINE_SWIZZLE(2, 0, 1, 3);
            DEFINE_SWIZZLE(2, 0, 2, 0); DEFINE_SWIZZLE(2, 0, 2, 1); DEFINE_SWIZZLE(2, 0, 2, 2); DEFINE_SWIZZLE(2, 0, 2, 3);
            DEFINE_SWIZZLE(2, 0, 3, 0); DEFINE_SWIZZLE(2, 0, 3, 1); DEFINE_SWIZZLE(2, 0, 3, 2); DEFINE_SWIZZLE(2, 0, 3, 3);
            DEFINE_SWIZZLE(2, 1, 0, 0); DEFINE_SWIZZLE(2, 1, 0, 1); DEFINE_SWIZZLE(2, 1, 0, 2); DEFINE_SWIZZLE(2, 1, 0, 3);
            DEFINE_SWIZZLE(2, 1, 1, 0); DEFINE_SWIZZLE(2, 1, 1, 1); DEFINE_SWIZZLE(2, 1, 1, 2); DEFINE_SWIZZLE(2, 1, 1, 3);
            DEFINE_SWIZZLE(2, 1, 2, 0); DEFINE_SWIZZLE(2, 1, 2, 1); DEFINE_SWIZZLE(2, 1, 2, 2); DEFINE_SWIZZLE(2, 1, 2, 3);
            DEFINE_SWIZZLE(2, 1, 3, 0); DEFINE_SWIZZLE(2, 1, 3, 1); DEFINE_SWIZZLE(2, 1, 3, 2); DEFINE_SWIZZLE(2, 1, 3, 3);
            DEFINE_SWIZZLE(2, 2, 0, 0); DEFINE_SWIZZLE(2, 2, 0, 1); DEFINE_SWIZZLE(2, 2, 0, 2); DEFINE_SWIZZLE(2, 2, 0, 3);
            DEFINE_SWIZZLE(2, 2, 1, 0); DEFINE_SWIZZLE(2, 2, 1, 1); DEFINE_SWIZZLE(2, 2, 1, 2); DEFINE_SWIZZLE(2, 2, 1, 3);
            DEFINE_SWIZZLE(2, 2, 2, 0); DEFINE_SWIZZLE(2, 2, 2, 1); DEFINE_SWIZZLE(2, 2, 2, 2); DEFINE_SWIZZLE(2, 2, 2, 3);
            DEFINE_SWIZZLE(2, 2, 3, 0); DEFINE_SWIZZLE(2, 2, 3, 1); DEFINE_SWIZZLE(2, 2, 3, 2); DEFINE_SWIZZLE(2, 2, 3, 3);
            DEFINE_SWIZZLE(2, 3, 0, 0); DEFINE_SWIZZLE(2, 3, 0, 1); DEFINE_SWIZZLE(2, 3, 0, 2); DEFINE_SWIZZLE(2, 3, 0, 3);
            DEFINE_SWIZZLE(2, 3, 1, 0); DEFINE_SWIZZLE(2, 3, 1, 1); DEFINE_SWIZZLE(2, 3, 1, 2); DEFINE_SWIZZLE(2, 3, 1, 3);
            DEFINE_SWIZZLE(2, 3, 2, 0); DEFINE_SWIZZLE(2, 3, 2, 1); DEFINE_SWIZZLE(2, 3, 2, 2); DEFINE_SWIZZLE(2, 3, 2, 3);
            DEFINE_SWIZZLE(2, 3, 3, 0); DEFINE_SWIZZLE(2, 3, 3, 1); DEFINE_SWIZZLE(2, 3, 3, 2); DEFINE_SWIZZLE(2, 3, 3, 3);
            DEFINE_SWIZZLE(3, 0, 0, 0); DEFINE_SWIZZLE(3, 0, 0, 1); DEFINE_SWIZZLE(3, 0, 0, 2); DEFINE_SWIZZLE(3, 0, 0, 3);
            DEFINE_SWIZZLE(3, 0, 1, 0); DEFINE_SWIZZLE(3, 0, 1, 1); DEFINE_SWIZZLE(3, 0, 1, 2); DEFINE_SWIZZLE(3, 0, 1, 3);
            DEFINE_SWIZZLE(3, 0, 2, 0); DEFINE_SWIZZLE(3, 0, 2, 1); DEFINE_SWIZZLE(3, 0, 2, 2); DEFINE_SWIZZLE(3, 0, 2, 3);
            DEFINE_SWIZZLE(3, 0, 3, 0); DEFINE_SWIZZLE(3, 0, 3, 1); DEFINE_SWIZZLE(3, 0, 3, 2); DEFINE_SWIZZLE(3, 0, 3, 3);
            DEFINE_SWIZZLE(3, 1, 0, 0); DEFINE_SWIZZLE(3, 1, 0, 1); DEFINE_SWIZZLE(3, 1, 0, 2); DEFINE_SWIZZLE(3, 1, 0, 3);
            DEFINE_SWIZZLE(3, 1, 1, 0); DEFINE_SWIZZLE(3, 1, 1, 1); DEFINE_SWIZZLE(3, 1, 1, 2); DEFINE_SWIZZLE(3, 1, 1, 3);
            DEFINE_SWIZZLE(3, 1, 2, 0); DEFINE_SWIZZLE(3, 1, 2, 1); DEFINE_SWIZZLE(3, 1, 2, 2); DEFINE_SWIZZLE(3, 1, 2, 3);
            DEFINE_SWIZZLE(3, 1, 3, 0); DEFINE_SWIZZLE(3, 1, 3, 1); DEFINE_SWIZZLE(3, 1, 3, 2); DEFINE_SWIZZLE(3, 1, 3, 3);
            DEFINE_SWIZZLE(3, 2, 0, 0); DEFINE_SWIZZLE(3, 2, 0, 1); DEFINE_SWIZZLE(3, 2, 0, 2); DEFINE_SWIZZLE(3, 2, 0, 3);
            DEFINE_SWIZZLE(3, 2, 1, 0); DEFINE_SWIZZLE(3, 2, 1, 1); DEFINE_SWIZZLE(3, 2, 1, 2); DEFINE_SWIZZLE(3, 2, 1, 3);
            DEFINE_SWIZZLE(3, 2, 2, 0); DEFINE_SWIZZLE(3, 2, 2, 1); DEFINE_SWIZZLE(3, 2, 2, 2); DEFINE_SWIZZLE(3, 2, 2, 3);
            DEFINE_SWIZZLE(3, 2, 3, 0); DEFINE_SWIZZLE(3, 2, 3, 1); DEFINE_SWIZZLE(3, 2, 3, 2); DEFINE_SWIZZLE(3, 2, 3, 3);
            DEFINE_SWIZZLE(3, 3, 0, 0); DEFINE_SWIZZLE(3, 3, 0, 1); DEFINE_SWIZZLE(3, 3, 0, 2); DEFINE_SWIZZLE(3, 3, 0, 3);
            DEFINE_SWIZZLE(3, 3, 1, 0); DEFINE_SWIZZLE(3, 3, 1, 1); DEFINE_SWIZZLE(3, 3, 1, 2); DEFINE_SWIZZLE(3, 3, 1, 3);
            DEFINE_SWIZZLE(3, 3, 2, 0); DEFINE_SWIZZLE(3, 3, 2, 1); DEFINE_SWIZZLE(3, 3, 2, 2); DEFINE_SWIZZLE(3, 3, 2, 3);
            DEFINE_SWIZZLE(3, 3, 3, 0); DEFINE_SWIZZLE(3, 3, 3, 1); DEFINE_SWIZZLE(3, 3, 3, 2); DEFINE_SWIZZLE(3, 3, 3, 3);
        };

        constexpr CPU_GPU vector() requires std::is_default_constructible_v<value_type>;
        constexpr CPU_GPU vector(T x, T y, T z, T w);
        constexpr CPU_GPU vector(const vector& cpy) requires std::is_copy_constructible_v<value_type>;
        constexpr CPU_GPU vector(vector&& mv) requires std::is_move_constructible_v<value_type>;

        template<vector_like... Vectors>
        constexpr CPU_GPU vector(const Vectors&... vecs) requires ((... + std::remove_cvref_t<Vectors>::size) == size) && std::conjunction_v<std::is_same<value_type, typename std::remove_cvref_t<Vectors>::value_type>...>;

        using vector_base<vector>::operator=;

        CPU_GPU vector& operator=(const vector& cpy);
        CPU_GPU vector& operator=(vector&& mv);

        constexpr CPU_GPU T& operator[](int idx);
        constexpr CPU_GPU T operator[](int idx) const;
    };

    template<typename T> requires (!vector_like<T>)
    vector(T x, T y, T z, T w) -> vector<T, 4>;

    template<vector_like Vector, vector_like... Vectors> requires (std::conjunction_v<std::is_same<typename std::remove_cvref_t<Vector>::value_type, typename std::remove_cvref_t<Vectors>::value_type>...>)
    vector(const Vector& vec, const Vectors&... vecs) -> vector<typename std::remove_cvref_t<Vector>::value_type, (std::remove_cvref_t<Vector>::size + ... + std::remove_cvref_t<Vectors>::size)>;

    using vec1f = vector<float, 1>;
    using vec2f = vector<float, 2>;
    using vec3f = vector<float, 3>;
    using vec4f = vector<float, 4>;

    // for constexpr swizzling
    template<typename T, std::size_t N, std::size_t... Swizzles> requires ((Swizzles < N) && ...)
    constexpr CPU_GPU auto swizzle(const vector<T, N>& vec, swizzle_tag<Swizzles...>) -> swizzle_vector<T, N, Swizzles...>;

    // functions
    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a == b; }
    constexpr CPU_GPU bool operator==(const Vector0& v0, const Vector1& v1);

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a != b; }
    constexpr CPU_GPU bool operator!=(const Vector0& v0, const Vector1& v1);

    // CLion has static analysis bugs that makes including these declarations break other code. unsure why.
    /*
    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a + b; }
    constexpr auto operator+(const Vector0& v0, const Vector1& v1);

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a - b; }
    constexpr auto operator-(const Vector0& v0, const Vector1& v1);

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a * b; }
    constexpr auto operator*(const Vector0& v0, const Vector1& v1);

    template<vector_like Vector, typename T> requires requires(typename std::remove_cvref_t<Vector>::value_type a, std::remove_cvref_t<T> b) { a * b; }
    constexpr auto operator*(const Vector& v, T t);

    template<typename T, vector_like Vector> requires requires(std::remove_cvref_t<T> a, typename std::remove_cvref_t<Vector>::value_type b) { a * b; }
    constexpr auto operator*(T t, const Vector& v);

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a / b; }
    constexpr auto operator/(const Vector0& v0, const Vector1& v1);

    template<vector_like Vector, typename T> requires requires(typename std::remove_cvref_t<Vector>::value_type a, std::remove_cvref_t<T> b) { a / b; }
    constexpr auto operator/(const Vector& v, T t);

    template<typename T, vector_like Vector> requires requires(std::remove_cvref_t<T> a, typename std::remove_cvref_t<Vector>::value_type b) { a / b; }
    constexpr auto operator/(T t, const Vector& v);
    */

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a * b + a * b; }
    constexpr CPU_GPU auto dot(const Vector0& v0, const Vector1& v1);

    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size && std::remove_cvref_t<Vector1>::size == 3) && requires(typename std::remove_cvref_t<Vector0>::value_type a, typename std::remove_cvref_t<Vector1>::value_type b) { a * b - a * b; }
    constexpr CPU_GPU auto cross(const Vector0& v0, const Vector1& v1);

    template<vector_like Vector>
    constexpr CPU_GPU auto magnitude(const Vector& v0);

    template<std::floating_point T, vector_like Vector>
    constexpr CPU_GPU auto normalize(const Vector& v0);

    template<vector_like Vector, std::floating_point T>
    constexpr CPU_GPU auto lerp(const Vector& v0, const Vector& v1, T t);

    template<vector_like Vector0, vector_like Vector1, vector_like Vector2> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size && std::remove_cvref_t<Vector1>::size == std::remove_cvref_t<Vector2>::size)
    constexpr CPU_GPU auto fma(const Vector0& v0, const Vector1& v1, const Vector2& v2);

    template<vector_like Vector> requires requires(typename std::remove_cvref_t<Vector>::value_type a) { { a < a } -> std::same_as<bool>; }
    constexpr CPU_GPU auto min(const Vector& v0, const Vector& v1) -> std::remove_cvref_t<Vector>;

    template<vector_like Vector> requires requires(typename std::remove_cvref_t<Vector>::value_type a) { { a < a } -> std::same_as<bool>; }
    constexpr CPU_GPU auto max(const Vector& v0, const Vector& v1) -> std::remove_cvref_t<Vector>;

    template<typename To, vector_like Vector> requires std::is_convertible_v<typename std::remove_cvref_t<Vector>::value_type, To>
    constexpr CPU_GPU auto vec_cast(const Vector& vec);

    // requires normalized vectors
    template<std::floating_point AngleT, vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == std::remove_cvref_t<Vector1>::size) && std::floating_point<typename std::remove_cvref_t<Vector0>::value_type> && std::floating_point<typename std::remove_cvref_t<Vector1>::value_type>
    constexpr CPU_GPU AngleT angle_between(const Vector0& v0, const Vector1& v1);

    // requires v1 to be normalized
    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == 3 && std::remove_cvref_t<Vector1>::size == 3)
    constexpr CPU_GPU auto normal_to_ortho(const Vector0& v0, const Vector1& v1);

    // does not require normalized vectors
    template<vector_like Vector0, vector_like Vector1> requires (std::remove_cvref_t<Vector0>::size == 3 && std::remove_cvref_t<Vector1>::size == 3)
    constexpr CPU_GPU auto nonnormal_to_ortho(const Vector0& v0, const Vector1& v1);

    // requires normalized vector
    template<vector_like Vector> requires (std::remove_cvref_t<Vector>::size == 3) && std::is_arithmetic_v<typename std::remove_cvref_t<Vector>::value_type>
    constexpr CPU_GPU auto coordinate_system(const Vector& v);

}

#include "impl/vec_base.inl"
#include "impl/swizzle_vec.inl"
#include "impl/vec.inl"
#include "impl/vec_func.inl"

// undefine macros

#undef PRIMITIVE_CAT_IDENTIFIER_2
#undef PRIMITIVE_CAT_IDENTIFIER_3
#undef PRIMITIVE_CAT_IDENTIFIER_4
#undef CAT_IDENTIFIER_1
#undef CAT_IDENTIFIER_2
#undef CAT_IDENTIFIER_3
#undef CAT_IDENTIFIER_4
#undef COMPONENT0
#undef COMPONENT1
#undef COMPONENT2
#undef COMPONENT3
#undef EXPAND_COMPONENT
#undef CAT_COMPONENT_1
#undef CAT_COMPONENT_2
#undef CAT_COMPONENT_3
#undef CAT_COMPONENT_4
#undef DEFINE_SWIZZLE_1
#undef DEFINE_SWIZZLE_2
#undef DEFINE_SWIZZLE_3
#undef DEFINE_SWIZZLE_4
#undef OVERLOAD_7MACRO
#undef PRIMITIVE_DEFINE_SWIZZLE
#undef DEFINE_SWIZZLE
#undef DEFINE_TAG_1
#undef DEFINE_TAG_2
#undef DEFINE_TAG_3
#undef DEFINE_TAG_4
#undef OVERLOAD_5MACRO
#undef PRIMITIVE_DEFINE_TAG
#undef DEFINE_TAG

#undef VEC_TEMPLATE
#undef TYPE
#undef SIZE

#endif //GPU_RAYTRACE_VEC_HPP

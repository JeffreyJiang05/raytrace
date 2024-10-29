#ifndef GPU_RAYTRACE_VEC_INL
#define GPU_RAYTRACE_VEC_INL

#include <utility>

#include "math/geometry/vec.hpp"

namespace math
{

    namespace impl
    {

        template<typename OffsetList, vector_like... Vectors>
        struct make_offset_list_helper;

        template<std::size_t Prev, std::size_t... Values, vector_like Head, vector_like Second, vector_like... Tail>
        struct make_offset_list_helper<std::index_sequence<Prev, Values...>, Head, Second, Tail...>
        {
            using type = typename make_offset_list_helper<std::index_sequence<
                    Prev + Head::size, Prev, Values...>, Second, Tail...>::type;
        };

        template<std::size_t... Values, vector_like Last>
        struct make_offset_list_helper<std::index_sequence<Values...>, Last>
        {
            using type = std::index_sequence<Values...>;
        };

        template<typename List, std::size_t Value>
        struct push_sequence_list;

        template<std::size_t... Values, std::size_t Value>
        struct push_sequence_list<std::index_sequence<Values...>, Value>
        {
            using type = std::index_sequence<Value, Values...>;
        };

        template<typename List, std::size_t Value>
        using push_sequence_list_t = typename push_sequence_list<List, Value>::type;

        template<typename OffsetList, typename Other>
        struct reverse_offset_list;

        template<std::size_t Head, std::size_t... Tail, std::size_t... Values>
        struct reverse_offset_list<std::index_sequence<Head, Tail...>, std::index_sequence<Values...>>
        {
            using type = typename reverse_offset_list<std::index_sequence<Tail...>, std::index_sequence<Head, Values...>>::type;
        };

        template<std::size_t... Values>
        struct reverse_offset_list<std::index_sequence<>, std::index_sequence<Values...>>
        {
            using type = std::index_sequence<Values...>;
        };

        template<typename OffsetList, typename Other>
        using reverse_offset_list_t = typename reverse_offset_list<OffsetList, Other>::type;

        template<vector_like... Vectors>
        struct make_offset_list
        {
            using type = reverse_offset_list_t<
                    typename make_offset_list_helper<std::index_sequence<0>, std::remove_cvref_t<Vectors>...>::type,
                    std::index_sequence<>>;
        };

        template<vector_like... Vectors>
        using make_offset_list_t = typename make_offset_list<Vectors...>::type;

        template<typename T, std::size_t N, vector_like Vector>
        constexpr int set_component_datum(container<T, N>& data, const Vector& vector, std::size_t offset)
        {
            for (std::size_t idx = 0; idx < std::remove_cvref_t<Vector>::size; ++idx)
            {
                data.buffer[offset + idx] = vector[idx];
            }
            return 0;
        }

        template<typename T, std::size_t N, vector_like... Vectors, std::size_t... Offsets>
        constexpr container<T, N> set_component_data(std::index_sequence<Offsets...>, const Vectors&... vectors)
        {
            container<T, N> container;
            [[maybe_unused]] int sink[] = { set_component_datum(container, vectors, Offsets)... };
            return container;
        }

    }

    // vector<T, 1> implementations

    template<typename T>
    constexpr CPU_GPU vector<T, 1>::vector() requires std::is_default_constructible_v<value_type>
    : data{ {  } } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 1>::vector(T x) : data{ {x} } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 1>::vector(const vector& cpy) requires std::is_copy_constructible_v<value_type>
    : data{ cpy.data } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 1>::vector(vector&& mv) requires std::is_move_constructible_v<value_type>
    : data{ std::move(mv.data) } {}

    template<typename T>
    template<vector_like... Vectors>
    constexpr CPU_GPU vector<T, 1>::vector(const Vectors&... vecs) requires ((... + std::remove_cvref_t<Vectors>::size) == size) && std::conjunction_v<std::is_same<value_type, typename std::remove_cvref_t<Vectors>::value_type>...>
    : data{ impl::set_component_data<value_type, size>(impl::make_offset_list_t<Vectors...>{}, vecs...) } {}

    template<typename T>
    CPU_GPU vector<T, 1>& vector<T, 1>::operator=(const vector& cpy)
    {
        if (this != &cpy)
        {
            for (std::size_t i = 0; i < size; ++i) data.buffer[i] = cpy[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU vector<T, 1>& vector<T, 1>::operator=(vector&& mv)
    {
        if (this != &mv)
        {
            for (std::size_t i = 0; i < size; ++i) data.buffer[i] = std::move(mv[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU vector<T, 1>::operator T() const
    {
        return data.buffer[0];
    }

    template<typename T>
    constexpr CPU_GPU T& vector<T, 1>::operator[](int idx)
    {
        return data.buffer[idx];
    }

    template<typename T>
    constexpr CPU_GPU T vector<T, 1>::operator[](int idx) const
    {
        return data.buffer[idx];
    }

    // vector<T, 2> implementations

    template<typename T>
    constexpr CPU_GPU vector<T, 2>::vector() requires std::is_default_constructible_v<value_type>
    : data{ {  } } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 2>::vector(T x, T y) : data{ {x, y} } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 2>::vector(const vector& cpy) requires std::is_copy_constructible_v<value_type>
    : data{ cpy.data } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 2>::vector(vector&& mv) requires std::is_move_constructible_v<value_type>
    : data{ std::move(mv.data) } {}

    template<typename T>
    template<vector_like... Vectors>
    constexpr CPU_GPU vector<T, 2>::vector(const Vectors&... vecs) requires ((... + std::remove_cvref_t<Vectors>::size) == size) && std::conjunction_v<std::is_same<value_type, typename std::remove_cvref_t<Vectors>::value_type>...>
    : data{ impl::set_component_data<value_type, size>(impl::make_offset_list_t<Vectors...>{}, vecs...) } {}

    template<typename T>
    CPU_GPU vector<T, 2>& vector<T, 2>::operator=(const vector& cpy)
    {
        if (this != &cpy)
        {
            for (std::size_t i = 0; i < size; ++i) data.buffer[i] = cpy[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU vector<T, 2>& vector<T, 2>::operator=(vector&& mv)
    {
        if (this != &mv)
        {
            for (std::size_t i = 0; i < size; ++i) data.buffer[i] = std::move(mv[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU T& vector<T, 2>::operator[](int idx)
    {
        return data.buffer[idx];
    }

    template<typename T>
    constexpr CPU_GPU T vector<T, 2>::operator[](int idx) const
    {
        return data.buffer[idx];
    }

    // vector<T, 3> implementations

    template<typename T>
    constexpr CPU_GPU vector<T, 3>::vector() requires std::is_default_constructible_v<value_type>
    : data{ {  } } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 3>::vector(T x, T y, T z) : data{ {x, y, z } } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 3>::vector(const vector& cpy) requires std::is_copy_constructible_v<value_type>
    : data{ cpy.data } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 3>::vector(vector&& mv) requires std::is_move_constructible_v<value_type>
    : data{ std::move(mv.data) } {}

    template<typename T>
    template<vector_like... Vectors>
    constexpr CPU_GPU vector<T, 3>::vector(const Vectors&... vecs) requires ((... + std::remove_cvref_t<Vectors>::size) == size) && std::conjunction_v<std::is_same<value_type, typename std::remove_cvref_t<Vectors>::value_type>...>
    : data{ impl::set_component_data<value_type, size>(impl::make_offset_list_t<Vectors...>{}, vecs...) } {}

    template<typename T>
    CPU_GPU vector<T, 3>& vector<T, 3>::operator=(const vector& cpy)
    {
        if (this != &cpy)
        {
            for (std::size_t i = 0; i < size; ++i) data.buffer[i] = cpy[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU vector<T, 3>& vector<T, 3>::operator=(vector&& mv)
    {
        if (this != &mv)
        {
            for (std::size_t i = 0; i < size; ++i) data.buffer[i] = std::move(mv[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU T& vector<T, 3>::operator[](int idx)
    {
        return data.buffer[idx];
    }

    template<typename T>
    constexpr CPU_GPU T vector<T, 3>::operator[](int idx) const
    {
        return data.buffer[idx];
    }

    // vector<T, 4> implementations

    template<typename T>
    constexpr CPU_GPU vector<T, 4>::vector() requires std::is_default_constructible_v<value_type>
    : data{ {  } } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 4>::vector(T x, T y, T z, T w) : data{ {x, y, z, w} } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 4>::vector(const vector& cpy) requires std::is_copy_constructible_v<value_type>
    : data{ cpy.data } {}

    template<typename T>
    constexpr CPU_GPU vector<T, 4>::vector(vector&& mv) requires std::is_move_constructible_v<value_type>
    : data{ std::move(mv) } {}

    template<typename T>
    template<vector_like... Vectors>
    constexpr CPU_GPU vector<T, 4>::vector(const Vectors&... vecs) requires ((... + std::remove_cvref_t<Vectors>::size) == size) && std::conjunction_v<std::is_same<value_type, typename std::remove_cvref_t<Vectors>::value_type>...>
    : data{ impl::set_component_data<value_type, size>(impl::make_offset_list_t<Vectors...>{}, vecs...) } {}

    template<typename T>
    CPU_GPU vector<T, 4>& vector<T, 4>::operator=(const vector& cpy)
    {
        if (this != &cpy)
        {
            for (std::size_t i = 0; i < size; ++i) data.buffer[i] = cpy[i];
        }
        return *this;
    }

    template<typename T>
    CPU_GPU vector<T, 4>& vector<T, 4>::operator=(vector&& mv)
    {
        if (this != &mv)
        {
            for (std::size_t i = 0; i < size; ++i) data.buffer[i] = std::move(mv[i]);
        }
        return *this;
    }

    template<typename T>
    constexpr CPU_GPU T& vector<T, 4>::operator[](int idx)
    {
        return data.buffer[idx];
    }

    template<typename T>
    constexpr CPU_GPU T vector<T, 4>::operator[](int idx) const
    {
        return data.buffer[idx];
    }

}

#endif //GPU_RAYTRACE_VEC_INL

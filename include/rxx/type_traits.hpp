#ifndef __RXX_TYPE_TRAITS_HPP__
#define __RXX_TYPE_TRAITS_HPP__

#include <type_traits>
#include "rxx/algorithm.hpp"

namespace rxx {

#define RXX_REQUIRES(...) typename std::enable_if<__VA_ARGS__::value, bool>::type = false

template<typename... Ts> struct make_void { typedef void type;};
template<typename... Ts> using void_t = typename rxx::make_void<Ts...>::type;

template<typename... Args>
using common_type_t = typename std::common_type<Args...>::type;

template< bool B, class T = void >
using enable_if_t = typename std::enable_if<B,T>::type;

template<typename T>
using decay_t = typename std::decay<T>::type;

template<bool B>
using bool_constant = std::integral_constant<bool, B>;

template<bool B, class T, class F>
using conditional_t = typename std::conditional<B,T,F>::type;

template<class...> struct conjunction : std::true_type {};
template<class B1> struct conjunction<B1> : B1 { };
template<class B1, class... Bn>
struct conjunction<B1, Bn...> 
    : rxx::conditional_t<bool(B1::value), rxx::conjunction<Bn...>, B1> {};

template<class T>
using remove_reference_t = typename std::remove_reference<T>::type;

template<std::size_t Len, class... Types>
struct aligned_union
{
    static constexpr std::size_t alignment_value = rxx::max(alignof(Types)...);
 
    struct type
    {
        alignas(alignment_value) char _s[rxx::max(Len, sizeof(Types)...)];
    };
};

template<std::size_t Len, class... Types>
using aligned_union_t = typename rxx::aligned_union<Len, Types...>::type;

template<class T, T... Ints>
struct integer_sequence {
    typedef T value_type;
    static_assert(std::is_integral<T>::value, "T must be integral");

    static constexpr inline std::size_t size() noexcept {
        return sizeof...(Ints);
    }
};

template<std::size_t... Ints>
using index_sequence = rxx::integer_sequence<std::size_t, Ints...>;

namespace impl {
    
template<class T, T Int, class S>
struct push_to_integer_sequence;

template<class T, T Int, T... Ints>
struct push_to_integer_sequence<T, Int, rxx::integer_sequence<T, Ints...>>
    : rxx::integer_sequence<T, Ints..., Int> {};

template<class T, T N>
struct make_integer_sequence_helper
    : rxx::impl::push_to_integer_sequence<
        T,
        N - 1,
        typename rxx::impl::make_integer_sequence_helper<T, N - 2>::type>::type {};

template<> struct make_integer_sequence_helper<char, 1>
    : rxx::integer_sequence<char, 0> {};

template<> struct make_integer_sequence_helper<char, 0>
    : rxx::integer_sequence<char> {};

template<> struct make_integer_sequence_helper<signed char, 1>
    : rxx::integer_sequence<signed char, 0> {};

template<> struct make_integer_sequence_helper<signed char, 0>
    : rxx::integer_sequence<signed char> {};

template<> struct make_integer_sequence_helper<unsigned char, 1>
    : rxx::integer_sequence<unsigned char, 0> {};

template<> struct make_integer_sequence_helper<unsigned char, 0>
    : rxx::integer_sequence<unsigned char> {};

template<> struct make_integer_sequence_helper<signed short, 1>
    : rxx::integer_sequence<signed short, 0> {};

template<> struct make_integer_sequence_helper<signed short, 0>
    : rxx::integer_sequence<signed short> {};

template<> struct make_integer_sequence_helper<unsigned short, 1>
    : rxx::integer_sequence<unsigned short, 0> {};

template<> struct make_integer_sequence_helper<unsigned short, 0>
    : rxx::integer_sequence<unsigned short> {};

template<> struct make_integer_sequence_helper<signed int, 1>
    : rxx::integer_sequence<signed int, 0> {};

template<> struct make_integer_sequence_helper<signed int, 0>
    : rxx::integer_sequence<signed int> {};

template<> struct make_integer_sequence_helper<unsigned int, 1>
    : rxx::integer_sequence<unsigned int, 0> {};

template<> struct make_integer_sequence_helper<unsigned int, 0>
    : rxx::integer_sequence<unsigned int> {};

template<> struct make_integer_sequence_helper<signed long, 1>
    : rxx::integer_sequence<signed long, 0> {};

template<> struct make_integer_sequence_helper<signed long, 0>
    : rxx::integer_sequence<signed long> {};

template<> struct make_integer_sequence_helper<unsigned long, 1>
    : rxx::integer_sequence<unsigned long, 0> {};

template<> struct make_integer_sequence_helper<unsigned long, 0>
    : rxx::integer_sequence<unsigned long> {};

template<> struct make_integer_sequence_helper<signed long long, 1>
    : rxx::integer_sequence<signed long long, 0> {};

template<> struct make_integer_sequence_helper<signed long long, 0>
    : rxx::integer_sequence<signed long long> {};

template<> struct make_integer_sequence_helper<unsigned long long, 1>
    : rxx::integer_sequence<unsigned long long, 0> {};

template<> struct make_integer_sequence_helper<unsigned long long, 0>
    : rxx::integer_sequence<unsigned long long> {};

} /* impl */ 

template<class T, T N>
using make_integer_sequence = typename rxx::impl::make_integer_sequence_helper<T, N>::type;

template<std::size_t N>
using make_index_sequence = rxx::make_integer_sequence<std::size_t, N>;

}

#endif /* end of include guard: __RXX_TYPE_TRAITS_HPP__ */

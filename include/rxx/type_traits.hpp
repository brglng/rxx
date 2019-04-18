#ifndef __RXX_TYPE_TRAITS_HPP__
#define __RXX_TYPE_TRAITS_HPP__

#include <type_traits>
#include "rxx/algorithm.hpp"

namespace rxx {

#define RXX_REQUIRES(...) typename std::enable_if<__VA_ARGS__::value, bool>::type = false

template<typename... Ts> struct make_void { typedef void type;};
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

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
    : conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

template<std::size_t Len, class... Types>
struct aligned_union
{
    static constexpr std::size_t alignment_value = static_max({alignof(Types)...});
 
    struct type
    {
        alignas(alignment_value) char _s[static_max({Len, sizeof(Types)...})];
    };
};

template<std::size_t Len, class... Types>
using aligned_union_t = typename aligned_union<Len, Types...>::type;

}

#endif /* end of include guard: __RXX_TYPE_TRAITS_HPP__ */

#ifndef __RXX_UTILITY_HPP__
#define __RXX_UTILITY_HPP__

#include <cstdlib>
#include <utility>
#include "rxx/type_traits.hpp"

namespace rxx {

struct in_place_t {
    explicit in_place_t() = default;
};
constexpr in_place_t in_place{};

template <class T> struct in_place_type_t {
    explicit in_place_type_t() = default;
};

template <size_t I> struct in_place_index_t {
    explicit in_place_index_t() = default;
};

template <class T> inline constexpr T&& static_forward(typename std::remove_reference<T>::type& t) noexcept
{
  return static_cast<T&&>(t);
}

template <class T> inline constexpr T&& static_forward(typename std::remove_reference<T>::type&& t) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "!!");
    return static_cast<T&&>(t);
}

template <class T> inline constexpr typename std::remove_reference<T>::type&& static_move(T&& t) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}

template <typename T>
struct has_overloaded_addressof
{
  template <class X>
  constexpr static bool has_overload(...) { return false; }
  
  template <class X, size_t S = sizeof(std::declval<X&>().operator&()) >
  constexpr static bool has_overload(bool) { return true; }

  constexpr static bool value = has_overload<T>(true);
};

template <typename T, RXX_REQUIRES(!has_overloaded_addressof<T>)>
constexpr T* static_addressof(T& ref)
{
  return &ref;
}

template <typename T, RXX_REQUIRES(has_overloaded_addressof<T>)>
T* static_addressof(T& ref)
{
  return std::addressof(ref);
}

}

#endif /* end of include guard: __RXX_UTILITY_HPP__ */

#ifndef __RXX_INVOKE_HPP__
#define __RXX_INVOKE_HPP__

#include <type_traits>
#include <utility>

namespace rxx {

// This is C++17 std::invoke implementation copied from cppreference.com, adapted to C++11.
// Do not ask what this shit of code means, and how it is implemented. Just use it with no worry.

namespace detail {
template <class T>
struct is_reference_wrapper : std::false_type {};
template <class U>
struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};
 
template<class T>
struct invoke_impl {
    template<class F, class... Args>
    static auto call(F&& f, Args&&... args)
        -> decltype(std::forward<F>(f)(std::forward<Args>(args)...));
};
 
template<class B, class MT>
struct invoke_impl<MT B::*> {
    template<class T, class Td = typename std::decay<T>::type,
        class = typename std::enable_if<std::is_base_of<B, Td>::value>::type
    >
    static auto get(T&& t) -> T&&;
 
    template<class T, class Td = typename std::decay<T>::type,
        class = typename std::enable_if<is_reference_wrapper<Td>::value>::type
    >
    static auto get(T&& t) -> decltype(t.get());
 
    template<class T, class Td = typename std::decay<T>::type,
        class = typename std::enable_if<!std::is_base_of<B, Td>::value>::type,
        class = typename std::enable_if<!is_reference_wrapper<Td>::value>::type
    >
    static auto get(T&& t) -> decltype(*std::forward<T>(t));
 
    template<class T, class... Args, class MT1,
        class = typename std::enable_if<std::is_function<MT1>::value>::type
    >
    static auto call(MT1 B::*pmf, T&& t, Args&&... args)
        -> decltype((invoke_impl::get(std::forward<T>(t)).*pmf)(std::forward<Args>(args)...));
 
    template<class T>
    static auto call(MT B::*pmd, T&& t)
        -> decltype(invoke_impl::get(std::forward<T>(t)).*pmd);
};
 
template<class F, class... Args, class Fd = typename std::decay<F>::type>
auto INVOKE(F&& f, Args&&... args)
    -> decltype(invoke_impl<Fd>::call(std::forward<F>(f), std::forward<Args>(args)...));
 
} // namespace detail
 
// Minimal C++11 implementation:
// template <class> struct result_of;
// template <class F, class... ArgTypes>
// struct result_of<F(ArgTypes...)> {
//     using type = decltype(detail::INVOKE(std::declval<F>(), std::declval<ArgTypes>()...));
// };
 
// Conforming C++14 implementation (is also a valid C++11 implementation):
namespace detail {
template <typename AlwaysVoid, typename, typename...>
struct invoke_result { };
template <typename F, typename...Args>
struct invoke_result<decltype(void(detail::INVOKE(std::declval<F>(), std::declval<Args>()...))),
                 F, Args...> {
    using type = decltype(detail::INVOKE(std::declval<F>(), std::declval<Args>()...));
};
} // namespace detail
 
template <class> struct result_of;
template <class F, class... ArgTypes>
struct result_of<F(ArgTypes...)> : detail::invoke_result<void, F, ArgTypes...> {};
 
template <class F, class... ArgTypes>
struct invoke_result : detail::invoke_result<void, F, ArgTypes...> {};

template< class F, class... Args>
typename invoke_result<F, Args...>::type invoke(F&& f, Args&&... args) 
  noexcept(std::__is_nothrow_invocable<F, Args...>::type)
{
    return detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
}

} // namespace rxx

#endif /* end of include guard: __RXX_INVOKE_HPP__ */

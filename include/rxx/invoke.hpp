#ifndef __RXX_INVOKE_HPP__
#define __RXX_INVOKE_HPP__

#include <utility>
#include "rxx/type_traits.hpp"
#include "rxx/utility.hpp"

namespace rxx {

// C++11 version of std::invoke and std::apply.

namespace impl {
template <class T>
struct is_reference_wrapper : std::false_type {};
template <class U>
struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};
 
template<class T>
struct invoke_impl {
    template<class F, class... Args>
    static constexpr auto call(F&& f, Args&&... args)
        noexcept(noexcept(static_forward<F>(f)(static_forward<Args>(args)...)))
        -> decltype(static_forward<F>(f)(static_forward<Args>(args)...))
    {
        return static_forward<F>(f)(static_forward<Args>(args)...);
    }
};
 
template<class B, class MT>
struct invoke_impl<MT B::*> {
    template<class T, class Td = typename std::decay<T>::type,
             class = typename std::enable_if<std::is_base_of<B, Td>::value>::type>
    static constexpr auto get(T&& t) noexcept -> T&& {
        return static_move(t);
    }
 
    template<class T, class Td = typename std::decay<T>::type,
             class = typename std::enable_if<is_reference_wrapper<Td>::value>::type>
    static constexpr auto get(T&& t) noexcept -> decltype(t.get()) {
        return t.get();
    }
 
    template<class T, class Td = typename std::decay<T>::type,
             class = typename std::enable_if<!std::is_base_of<B, Td>::value>::type,
             class = typename std::enable_if<!is_reference_wrapper<Td>::value>::type>
    static constexpr auto get(T&& t) noexcept -> decltype(*static_forward<T>(t)) {
        return *static_forward<T>(t);
    }
 
    template<class T, class... Args, class MT1,
             class = typename std::enable_if<std::is_function<MT1>::value>::type>
    static constexpr auto call(MT1 B::*pmf, T&& t, Args&&... args)
        noexcept(noexcept((invoke_impl::get(static_forward<T>(t)).*pmf)(static_forward<Args>(args)...)))
        -> decltype((invoke_impl::get(static_forward<T>(t)).*pmf)(static_forward<Args>(args)...)) {
        return (invoke_impl::get(static_forward<T>(t)).*pmf)(static_forward<Args>(args)...);
    }
 
    template<class T>
    static auto call(MT B::*pmd, T&& t)
        noexcept(noexcept(invoke_impl::get(static_forward<T>(t)).*pmd))
        -> decltype(invoke_impl::get(static_forward<T>(t)).*pmd) {
        return invoke_impl::get(static_forward<T>(t)).*pmd;
    }
};
 
template<class F, class... Args, class Fd = typename std::decay<F>::type>
constexpr auto INVOKE(F&& f, Args&&... args)
    noexcept(noexcept(invoke_impl<Fd>::call(static_forward<F>(f), static_forward<Args>(args)...)))
    -> decltype(invoke_impl<Fd>::call(static_forward<F>(f), static_forward<Args>(args)...)) {
    return invoke_impl<Fd>::call(static_forward<F>(f), static_forward<Args>(args)...);
}

} // namespace impl
 
// invoke_result
namespace impl {

template <typename F, typename... Args>
using invoke_result_ = decltype(impl::INVOKE(std::declval<F>(), std::declval<Args>()...));

template <typename F, typename... Args>
struct invoke_nothrow_ : bool_constant<noexcept(impl::INVOKE(std::declval<F>(), std::declval<Args>()...))> {};

template <typename Void, typename F, typename... Args>
struct invoke_result {};

template <typename F, typename... Args>
struct invoke_result<void_t<invoke_result_<F, Args...>>, F, Args...> {
    using type = invoke_result_<F, Args...>;
};

template<typename Void, typename F, typename... Args>
struct is_invocable : std::false_type {};

template<typename F, typename... Args>
struct is_invocable<void_t<invoke_result_<F, Args...>>, F, Args...> : std::true_type {};

template<typename Void, typename R, typename F, typename... Args>
struct is_invocable_r: std::false_type {};

template<typename R, typename F, typename... Args>
struct is_invocable_r<void_t<invoke_result_<F, Args...>>, R, F, Args...>
    : std::is_convertible<invoke_result_<F, Args...>, R> {};

template<typename Void, typename F, typename... Args>
struct is_nothrow_invocable : std::false_type {};

template<typename F, typename... Args>
struct is_nothrow_invocable<void_t<invoke_result_<F, Args...>>, F, Args...> :
    bool_constant<noexcept(impl::INVOKE(std::declval<F>(), std::declval<Args>()...))> {};

template<typename Void, typename R, typename F, typename... Args>
struct is_nothrow_invocable_r : std::false_type {};

template<typename R, typename F, typename... Args>
struct is_nothrow_invocable_r<void_t<invoke_result_<F, Args...>>, R, F, Args...>
    : conjunction<std::is_convertible<invoke_result_<F, Args...>, R>,
                  invoke_nothrow_<F, Args...>> {};

} // namespace impl

template <class> struct result_of;
template <class F, class... Args>
struct result_of<F(Args...)> : impl::invoke_result<void, F, Args...> {};
 
template<class F, class... Args>
struct invoke_result : impl::invoke_result<void, F, Args...> {};

template<typename F, typename... Args>
using invoke_result_t = typename invoke_result<F, Args...>::type;

template<typename F, typename... Args>
struct is_invocable : impl::is_invocable<void, F, Args...> {};

template<typename R, typename F, typename... Args>
struct is_invocable_r : impl::is_invocable_r<void, R, F, Args...> {};

template<typename F, typename... Args>
struct is_nothrow_invocable : impl::is_nothrow_invocable<void, F, Args...> {};

template<typename R, typename F, typename... Args>
struct is_nothrow_invocable_r : impl::is_nothrow_invocable<void, R, F, Args...> {};

template<class F, class... Args>
constexpr invoke_result_t<F, Args...> invoke(F&& f, Args&&... args) 
    noexcept(is_nothrow_invocable<F, Args...>::value)
{
    return impl::INVOKE(static_forward<F>(f), static_forward<Args>(args)...);
}

} // namespace rxx

#endif /* end of include guard: __RXX_INVOKE_HPP__ */

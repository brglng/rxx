#ifndef __RXX_RESULT_PROTO_HPP__
#define __RXX_RESULT_PROTO_HPP__

#include <type_traits>
#include "rxx/utility.hpp"

namespace rxx {

namespace result {

template<typename T>
struct Ok {
    T m_ok;

    constexpr Ok(T&& t) : m_ok(rxx::move(t)) {}
    constexpr Ok(T const& t) : m_ok(t) {}
    Ok(Ok&&) = default;
    Ok(Ok const&) = default;
    Ok& operator=(Ok&&) = default;
    Ok& operator=(Ok const&) = default;
    ~Ok() = default;
};

template<> struct Ok<void> {};

template<typename E>
struct Err {
    E m_err;

    constexpr Err(E&& e) : m_err(rxx::move(e)) {}
    constexpr Err(E const& e) : m_err(e) {}
    Err(Err&&) = default;
    Err(Err const&) = default;
    Err& operator=(Err&&) = default;
    Err& operator=(Err const&) = default;
    ~Err() = default;
};

template<> struct Err<void> {};

template<typename T> struct is_ok : std::false_type {};
template<typename T> struct is_ok<Ok<T>> : std::true_type {};

template<typename T1, typename T2> struct is_same_ok : std::false_type {};
template<typename T> struct is_same_ok<Ok<T>, Ok<T>> : std::true_type {};

template<typename E> struct is_err : std::false_type {};
template<typename E> struct is_err<Err<E>> : std::true_type {};

template<typename E1, typename E2> struct is_same_err : std::false_type {};
template<typename E> struct is_same_err<Err<E>, Err<E>> : std::true_type {};

}

template<typename T, typename E>    class Result;
template<typename E>                class Result<void, E>;
template<typename T>                class Result<T, void>;
template<>                          class Result<void, void>;
template<typename T, typename E>    class Result<T&, E>;
template<typename T, typename E>    class Result<T, E&>;
template<typename E>                class Result<void, E&>;
template<typename T>                class Result<T&, void>;
template<typename T, typename E>    class Result<T&, E&>;

template<typename T>
inline auto Ok(T&& value) -> result::Ok<rxx::remove_reference_t<T>>;

inline auto Ok() -> result::Ok<void> { return result::Ok<void>(); }

template<typename E>
inline auto Err(E&& err) -> result::Err<rxx::remove_reference_t<E>>;

inline auto Err() -> result::Err<void> { return result::Err<void>(); }

template<typename T> struct is_result : std::false_type {};
template<typename T, typename E> struct is_result<Result<T, E>> : std::true_type {};

template<typename R1, typename R2> struct is_same_result : std::false_type {};
template<typename T, typename E> struct is_same_result<Result<T, E>, Result<T, E>> : std::true_type {};

template<typename R1, typename R2> struct is_result_of_same_err_type : std::false_type {};
template<typename T, typename U, typename E> struct is_result_of_same_err_type<Result<T, E>, Result<U, E>> : std::true_type {};

template<typename R1, typename R2> struct is_result_of_same_value_type : std::false_type {};
template<typename T, typename E1, typename E2> struct is_result_of_same_value_type<Result<T, E1>, Result<T, E2>> : std::true_type {};

}

#endif /* end of include guard: __RXX_RESULT_PROTO_HPP__ */

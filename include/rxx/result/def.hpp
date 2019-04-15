#ifndef __RXX_RESULT_DEF_HPP__
#define __RXX_RESULT_DEF_HPP__

#include <cstdio>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <cassert>
#include "rxx/invoke.hpp"
#include "rxx/str.hpp"
#include "rxx/utility.hpp"

namespace rxx {

namespace result {
namespace impl {

template<typename T>
struct Ok {
    T m_ok;
};

template<> struct Ok<void> { constexpr Ok() {} };

template<typename E>
struct Err {
    E m_err;
};

template<> struct Err<void> { constexpr Err() {} };

}
}

template<typename T, typename E>
class Result;

template<typename E>
class Result<void, E>;

template<typename T, typename E>
class Result<T&, E>;

template<typename T, typename E>
class Result<T, E&>;

template<typename E>
class Result<void, E&>;

template<typename T, typename E>
class Result<T&, E&>;

template<typename T>
class Option;

template<>
class Option<void>;

template<typename T>
class Option<T&>;

template<typename T>
inline auto Ok(T&& value) -> result::impl::Ok<typename std::decay<T>::type>;

inline auto Ok() -> result::impl::Ok<void> { return result::impl::Ok<void>(); }

template<typename E>
inline auto Err(E&& err) -> result::impl::Err<typename std::decay<E>::type>;

template<typename T> struct is_result : std::false_type {};
template<typename T, typename E> struct is_result<Result<T, E>> : std::true_type {};

template<typename R1, typename R2> struct is_same_result : std::false_type {};
template<typename T, typename E> struct is_same_result<Result<T, E>, Result<T, E>> : std::true_type {};

template<typename R1, typename R2> struct is_result_of_same_err_type : std::false_type {};
template<typename T, typename U, typename E> struct is_result_of_same_err_type<Result<T, E>, Result<U, E>> : std::true_type {};

template<typename R1, typename R2> struct is_result_of_same_value_type : std::false_type {};
template<typename T, typename E1, typename E2> struct is_result_of_same_value_type<Result<T, E1>, Result<T, E2>> : std::true_type {};

template<typename T, typename E>
class Result {
private:
    typename std::aligned_union<1, T, E>::type m_buffer;
    bool m_is_ok;

    T& value() & noexcept {
        return *(T*)(&m_buffer);
    }

    T&& value() && noexcept {
        return std::move(*(T*)(&m_buffer));
    }

    constexpr T const& value() const& noexcept {
        return *(T const*)(&m_buffer);
    }

    T* valueptr() noexcept {
        return (T*)(&m_buffer);
    }

    constexpr T const* valueptr() const noexcept {
        return (T const*)(&m_buffer);
    }

    E& error() & noexcept {
        return *(E*)(&m_buffer);
    }

    E&& error() && noexcept {
        return std::move(*(E*)(&m_buffer));
    }

    constexpr E const& error() const& noexcept {
        return *(E const*)(&m_buffer);
    }

    E* errorptr() noexcept {
        return (E*)(&m_buffer);
    }

    constexpr E const* errorptr() const noexcept {
        return (E const*)(&m_buffer);
    }

    void destroy() noexcept {
        if (m_is_ok) {
            value().T::~T();
        } else {
            error().E::~E();
        }
    }

public:
    explicit Result(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
        : m_is_ok{true} {
        new (valueptr()) T{std::move(value)};
    }

    explicit Result(T const& value) noexcept(std::is_nothrow_copy_assignable<T>::value)
        : m_is_ok{true} {
        new (valueptr()) T{value};
    }

    explicit Result(E&& err) noexcept(std::is_nothrow_move_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{std::move(err)};
    }

    explicit Result(E const& err) noexcept(std::is_nothrow_copy_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{err};
    }

    Result(result::impl::Ok<T>&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
        : m_is_ok(true) {
        new (valueptr()) T{std::move(value.m_ok)};
    }

    Result(result::impl::Err<E>&& err) noexcept(std::is_nothrow_move_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{std::move(err.m_err)};
    }

    Result(result::impl::Ok<T> const& value) noexcept(std::is_nothrow_copy_constructible<T>::value)
        : m_is_ok(true) {
        new (valueptr()) T{value.m_ok};
    }

    Result(result::impl::Err<E> const& err) noexcept(std::is_nothrow_copy_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{err.m_err};
    }

    Result(Result&& that) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_constructible<E>::value)
        : m_is_ok{that.m_is_ok} {
        if (m_is_ok) {
            new (valueptr()) T{std::move(that.value())};
        } else {
            new (errorptr()) E{std::move(that.error())};
        }
    }

    Result(Result const& that) noexcept(std::is_nothrow_copy_constructible<T>::value && std::is_nothrow_copy_constructible<E>::value)
        : m_is_ok(that.m_is_ok) {
        if (m_is_ok) {
            new (valueptr()) T{that.value()};
        } else {
            new (errorptr()) E{that.error()};
        }
    }

    ~Result() noexcept {
        destroy();
    }

    auto operator=(Result&& that) noexcept(std::is_nothrow_move_assignable<T>::value && std::is_nothrow_move_assignable<E>::value) -> Result& {
        if (this != &that) {
            destroy();
            m_is_ok = that.m_is_ok;
            if (that.m_is_ok) {
                value() = std::move(that.value());
            } else {
                error() = std::move(that.error());
            }
        }
        return *this;
    }

    auto operator=(Result const& that) noexcept(std::is_nothrow_copy_assignable<T>::value && std::is_nothrow_copy_assignable<E>::value) -> Result& {
        if (this != &that) {
            destroy();
            m_is_ok = that.m_is_ok;
            if (that.m_is_ok) {
                value() = that.value();
            } else {
                error() = that.error();
            }
        }
        return *this;
    }

    auto operator=(result::impl::Ok<T>&& value) noexcept(std::is_nothrow_move_assignable<T>::value) -> Result& {
        destroy();
        m_is_ok = true;
        value() = std::move(value.m_ok);
        return *this;
    }

    auto operator=(result::impl::Ok<T> const& value) noexcept(std::is_nothrow_copy_assignable<T>::value) -> Result& {
        destroy();
        m_is_ok = true;
        value() = value.m_ok;
        return *this;
    }

    auto operator=(result::impl::Err<E>&& err) noexcept(std::is_nothrow_move_assignable<E>::value) -> Result& {
        destroy();
        m_is_ok = true;
        error() = std::move(err.m_err);
        return *this;
    }

    auto operator=(result::impl::Err<E> const& err) noexcept(std::is_nothrow_copy_assignable<E>::value) -> Result& {
        destroy();
        m_is_ok = true;
        error() = err.m_err;
        return *this;
    }

    auto operator==(Result const& that) const -> bool {
        if (m_is_ok && that.m_is_ok) {
            return value() == that.value();
        } else if (!m_is_ok && !that.m_is_ok) {
            return error() == that.error();
        }
        return false;
    }

    auto ok() -> Option<T>;
    auto ok() const -> Option<T>;
    auto err() -> Option<E>;
    auto err() const -> Option<E>;

    auto is_ok() const -> bool {
        return m_is_ok;
    }

    auto is_err() const -> bool {
        return !m_is_ok;
    }

    template<typename F>
    auto map(F&& op) -> Result<typename invoke_result<typename std::decay<F>::type, T&&>::type, E> {
        if (is_ok()) {
            return Ok(invoke(std::forward<F>(op), std::move(value())));
        } else {
            return Err(std::move(error()));
        }
    }

    template<typename F>
    auto map(F&& op) const -> Result<typename invoke_result<typename std::decay<F>::type, T const&>::type, E> {
        if (is_ok()) {
            return Ok(invoke(std::forward<F>(op), std::move(value())));
        } else {
            return Err(std::move(error()));
        }
    }

    template<typename F, typename M>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<F>::type, E&&>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<M>::type, T&&>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<F>::type>::type>::type
    >::type
    map_or_else(F&& fallback, M&& map_func) {
        if (is_ok()) {
            return invoke(std::forward<M>(map_func), std::move(value()));
        } else {
            return invoke(std::forward<F>(fallback), std::move(error()));
        }
    }

    template<typename F, typename M>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<F>::type, E const&>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<M>::type, T const&>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<F>::type>::type>::type
    >::type
    map_or_else(F&& fallback, M&& map_func) const {
        if (is_ok()) {
            return invoke(std::forward<M>(map_func), value());
        } else {
            return invoke(std::forward<F>(fallback), error());
        }
    }

    template<typename O>
    auto map_err(O&& op) -> Result<T, typename invoke_result<typename std::decay<O>::type, E&&>::type> {
        if (!is_ok()) {
            return Err(invoke(std::forward<O>(op), error()));
        } else {
            return Ok(std::move(value()));
        }
    }

    template<typename O>
    auto map_err(O&& op) const -> Result<T, typename invoke_result<typename std::decay<O>::type, E const&>::type> {
        if (!is_ok()) {
            return Err(invoke(std::forward<O>(op), error()));
        } else {
            return Ok(std::move(value()));
        }
    }

    template<typename U>
    auto and_(Result<U, E> res) -> Result<U, E> {
        if (res.is_ok()) {
            return res;
        } else {
            return Err(std::move(error()));
        }
    }

    template<typename U>
    auto and_(Result<U, E> res) const -> Result<U, E> {
        if (res.is_ok()) {
            return res;
        } else {
            return Err(error());
        }
    }

    template<typename F>
    typename std::enable_if<
        is_result_of_same_err_type<
            Result<T, E>,
            typename invoke_result<typename std::decay<F>::type, T&&>::type>::value,
        typename invoke_result<typename std::decay<F>::type, T&&>::type
    >::type
    and_then(F&& op) {
        if (is_ok()) {
            return invoke(op, std::move(value()));
        } else {
            return Err(std::move(error()));
        }
    }

    template<typename F>
    typename std::enable_if<
        is_result_of_same_err_type<
            Result<T, E>,
            typename invoke_result<typename std::decay<F>::type, T const&>::type>::value,
        typename invoke_result<typename std::decay<F>::type, T const&>::type
    >::type
    and_then(F&& op) const {
        if (is_ok()) {
            return invoke(op, value());
        } else {
            return Err(error());
        }
    }

    template<typename F>
    auto or_(Result<T, F> res) -> Result<T, F> {
        if (is_err()) {
            return res;
        } else {
            return Ok(std::move(value()));
        }
    }

    template<typename F>
    auto or_(Result<T, F> res) const -> Result<T, F> {
        if (is_err()) {
            return res;
        } else {
            return Ok(value());
        }
    }

    template<typename O>
    typename std::enable_if<
        is_result_of_same_value_type<
            Result<T, E>,
            typename invoke_result<typename std::decay<O>::type, E&&>::type>::value,
        typename invoke_result<typename std::decay<O>::type, E&&>::type
    >::type
    or_else(O&& op) {
        if (is_err()) {
            return invoke(op, std::move(error()));
        } else {
            return Ok(std::move(value()));
        }
    }

    template<typename O>
    typename std::enable_if<
        is_result_of_same_value_type<
            Result<T, E>,
            typename invoke_result<typename std::decay<O>::type, E const&>::type>::value,
        typename invoke_result<typename std::decay<O>::type, E const&>::type
    >::type
    or_else(O&& op) {
        if (is_err()) {
            return invoke(op, error());
        } else {
            return Ok(std::move(value()));
        }
    }

    template<typename O>
    typename std::enable_if<
        is_result_of_same_value_type<
            Result<T, E>,
            typename invoke_result<typename std::decay<O>::type, E const&>::type>::value,
        typename invoke_result<typename std::decay<O>::type, E const&>::type
    >::type
    or_else(O&& op) const {
        if (is_err()) {
            return invoke(op, error());
        } else {
            return Ok(value());
        }
    }

    auto unwrap_or(T optb) -> T {
        if (is_ok()) {
            return std::move(*this);
        } else {
            return optb;
        }
    }

    auto unwrap_or(T optb) const -> T {
        if (is_ok()) {
            return std::move(*this);
        } else {
            return optb;
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& op) -> T {
        if (is_ok()) {
            return std::move(*this);
        } else {
            return invoke(std::forward<F>(op), std::move(error()));
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& op) const -> T {
        if (is_ok()) {
            return *this;
        } else {
            return invoke(std::forward<F>(op), error());
        }
    }

    auto unwrap() -> T {
        assert(is_ok());
        return std::move(value());
    }

    auto unwrap() const -> T {
        assert(is_ok());
        return value();
    }

    auto expect(Str msg) -> T {
        if (!is_ok()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return std::move(value());
    }

    auto expect(Str msg) const -> T {
        if (!is_ok()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return value();
    }

    auto unwrap_err() -> E {
        assert(is_err());
        return std::move(error());
    }

    auto unwrap_err() const -> E {
        assert(is_err());
        return error();
    }

    auto expect_err(Str msg) -> E {
        if (!is_err()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return std::move(error());
    }

    auto expect_err(Str msg) const -> E {
        if (!is_err()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return std::move(error());
    }

    auto unwrap_or_default() -> T {
        if (is_ok()) {
            return std::move(value());
        } else {
            return T{};
        }
    }

    auto unwrap_or_default() const -> T {
        if (is_ok()) {
            return value();
        } else {
            return T{};
        }
    }
};

template<typename E>
class Result<void, E> {
private:
    typename std::aligned_storage<sizeof(E)>::type m_buffer;
    bool m_is_ok;

    E& error() & noexcept {
        return *(E*)(&m_buffer);
    }

    E&& error() && noexcept {
        return std::move(*(E*)(&m_buffer));
    }

    constexpr E const& error() const& noexcept {
        return *(E const*)(&m_buffer);
    }

    E* errorptr() noexcept {
        return (E*)(&m_buffer);
    }

    constexpr E const* errorptr() const noexcept {
        return (E const*)(&m_buffer);
    }

    void destroy() noexcept {
        if (!m_is_ok) {
            error().E::~E();
        }
    }

public:
    explicit constexpr Result() noexcept : m_is_ok{true} {}

    explicit Result(E&& err) noexcept(std::is_nothrow_move_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{std::move(err)};
    }

    explicit Result(E const& err) noexcept(std::is_nothrow_copy_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{err};
    }

    Result(result::impl::Ok<void>&&) noexcept : m_is_ok(true) {}

    Result(result::impl::Err<E>&& err)
        noexcept(std::is_nothrow_move_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{std::move(err.m_err)};
    }

    Result(result::impl::Ok<void> const&) : m_is_ok(true) {}

    Result(result::impl::Err<E> const& err) noexcept(std::is_nothrow_copy_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{err.m_err};
    }

    Result(Result&& that) noexcept(std::is_nothrow_move_constructible<E>::value)
        : m_is_ok{that.m_is_ok} {
        if (!m_is_ok) {
            new (errorptr()) E{std::move(that.error())};
        }
    }

    Result(Result const& that) noexcept(std::is_nothrow_copy_constructible<E>::value)
        : m_is_ok(that.m_is_ok) {
        if (!m_is_ok) {
            new (errorptr()) E{that.error()};
        }
    }

    ~Result() noexcept {
        destroy();
    }

    auto operator=(Result&& that) noexcept(std::is_nothrow_move_assignable<E>::value) -> Result& {
        if (this != &that) {
            destroy();
            m_is_ok = that.m_is_ok;
            if (!that.m_is_ok) {
                error() = std::move(that.error());
            }
        }
        return *this;
    }

    auto operator=(Result const& that) noexcept(std::is_nothrow_copy_assignable<E>::value) -> Result& {
        if (this != &that) {
            destroy();
            m_is_ok = that.m_is_ok;
            if (!that.m_is_ok) {
                error() = that.error();
            }
        }
        return *this;
    }

    auto operator=(result::impl::Ok<void>&&) noexcept -> Result& {
        destroy();
        m_is_ok = true;
        return *this;
    }

    auto operator=(result::impl::Ok<void> const&) noexcept -> Result& {
        destroy();
        m_is_ok = true;
        return *this;
    }

    auto operator=(result::impl::Err<E>&& err) noexcept(std::is_nothrow_move_assignable<E>::value) -> Result& {
        destroy();
        m_is_ok = true;
        error() = std::move(err.m_err);
        return *this;
    }

    auto operator=(result::impl::Err<E> const& err) noexcept(std::is_nothrow_copy_assignable<E>::value) -> Result& {
        destroy();
        m_is_ok = true;
        error() = err.m_err;
        return *this;
    }

    auto operator==(Result const& that) const -> bool {
        if (!m_is_ok && !that.m_is_ok) {
            return error() == that.error();
        }
        return false;
    }

    auto ok() -> Option<void>;
    auto ok() const -> Option<void>;
    auto err() -> Option<E>;
    auto err() const -> Option<E>;

    auto is_ok() const -> bool {
        return m_is_ok;
    }

    auto is_err() const -> bool {
        return !m_is_ok;
    }

    template<typename F>
    auto map(F&& op) -> Result<void, E> {
        if (is_ok()) {
            return Ok(invoke(std::forward<F>(op)));
        } else {
            return Err(std::move(error()));
        }
    }

    template<typename F>
    auto map(F&& op) const -> Result<typename invoke_result<typename std::decay<F>::type>::type, E> {
        if (is_ok()) {
            return Ok(invoke(std::forward<F>(op)));
        } else {
            return Err(std::move(error()));
        }
    }

    template<typename F, typename M>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<F>::type, E&&>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<M>::type>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<F>::type>::type>::type
    >::type
    map_or_else(F&& fallback, M&& map_func) {
        if (is_ok()) {
            return invoke(std::forward<M>(map_func));
        } else {
            return invoke(std::forward<F>(fallback), std::move(error()));
        }
    }

    template<typename F, typename M>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<F>::type, E const&>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<M>::type>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<F>::type>::type>::type
    >::type
    map_or_else(F&& fallback, M&& map_func) const {
        if (is_ok()) {
            return invoke(std::forward<M>(map_func));
        } else {
            return invoke(std::forward<F>(fallback), error());
        }
    }

    template<typename O>
    auto map_err(O&& op) -> Result<void, typename invoke_result<typename std::decay<O>::type, E&&>::type> {
        if (!is_ok()) {
            return Err(invoke(std::forward<O>(op), error()));
        } else {
            return Ok();
        }
    }

    template<typename O>
    auto map_err(O&& op) const -> Result<void, typename invoke_result<typename std::decay<O>::type, E const&>::type> {
        if (!is_ok()) {
            return Err(invoke(std::forward<O>(op), error()));
        } else {
            return Ok();
        }
    }

    template<typename U>
    auto and_(Result<U, E> res) -> Result<U, E> {
        if (res.is_ok()) {
            return res;
        } else {
            return Err(std::move(error()));
        }
    }

    template<typename U>
    auto and_(Result<U, E> res) const -> Result<U, E> {
        if (res.is_ok()) {
            return res;
        } else {
            return Err(error());
        }
    }

    template<typename F>
    typename std::enable_if<
        is_result_of_same_err_type<
            Result<void, E>,
            typename invoke_result<typename std::decay<F>::type>::type>::value,
        typename invoke_result<typename std::decay<F>::type>::type
    >::type
    and_then(F&& op) {
        if (is_ok()) {
            return invoke(op);
        } else {
            return Err(std::move(error()));
        }
    }

    template<typename F>
    typename std::enable_if<
        is_result_of_same_err_type<
            Result<void, E>,
            typename invoke_result<typename std::decay<F>::type>::type>::value,
        typename invoke_result<typename std::decay<F>::type>::type
    >::type
    and_then(F&& op) const {
        if (is_ok()) {
            return invoke(op);
        } else {
            return Err(error());
        }
    }

    template<typename F>
    auto or_(Result<void, F> res) -> Result<void, F> {
        if (is_err()) {
            return res;
        } else {
            return Ok();
        }
    }

    template<typename F>
    auto or_(Result<void, F> res) const -> Result<void, F> {
        if (is_err()) {
            return res;
        } else {
            return Ok();
        }
    }

    template<typename O>
    typename std::enable_if<
        is_result_of_same_value_type<
            Result<void, E>,
            typename invoke_result<typename std::decay<O>::type, E&&>::type>::value,
        typename invoke_result<typename std::decay<O>::type, E&&>::type
    >::type
    or_else(O&& op) {
        if (is_err()) {
            return invoke(op, std::move(error()));
        } else {
            return Ok();
        }
    }

    template<typename O>
    typename std::enable_if<
        is_result_of_same_value_type<
            Result<void, E>,
            typename invoke_result<typename std::decay<O>::type, E const&>::type>::value,
        typename invoke_result<typename std::decay<O>::type, E const&>::type
    >::type
    or_else(O&& op) {
        if (is_err()) {
            return invoke(op, error());
        } else {
            return Ok();
        }
    }

    template<typename O>
    typename std::enable_if<
        is_result_of_same_value_type<
            Result<void, E>,
            typename invoke_result<typename std::decay<O>::type, E const&>::type>::value,
        typename invoke_result<typename std::decay<O>::type, E const&>::type
    >::type
    or_else(O&& op) const {
        if (is_err()) {
            return invoke(op, error());
        } else {
            return Ok();
        }
    }

    void unwrap_or() const {}

    template<typename F>
    void unwrap_or_else(F&& op) {
        if (is_err()) {
            return invoke(std::forward<F>(op), std::move(error()));
        }
    }

    template<typename F>
    void unwrap_or_else(F&& op) const {
        if (is_err()) {
            return invoke(std::forward<F>(op), error());
        }
    }

    void unwrap() const {
        assert(is_ok());
    }

    void expect(Str msg) const {
        if (!is_ok()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
    }

    auto unwrap_err() -> E {
        assert(is_err());
        return std::move(error());
    }

    auto unwrap_err() const -> E {
        assert(is_err());
        return error();
    }

    auto expect_err(Str msg) -> E {
        if (!is_err()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return std::move(error());
    }

    auto expect_err(Str msg) const -> E {
        if (!is_err()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return std::move(error());
    }

    void unwrap_or_default() const {}
};

template<class T, class E>
class Result<T&&, E> {
    static_assert(sizeof(T) == 0, "Result of rvalue references disallowed");
};

template<class T, class E>
class Result<T, E&&> {
    static_assert(sizeof(E) == 0, "Result of rvalue references disallowed");
};

template<class T, class E>
class Result<T&&, E&&> {
    static_assert(sizeof(T) == 0 && sizeof(E) == 0, "Result of rvalue references disallowed");
};

template<typename T>
inline auto Ok(T&& value) -> result::impl::Ok<typename std::decay<T>::type> {
    return result::impl::Ok<typename std::decay<T>::type>(std::forward<typename std::decay<T>::type>(value));
}

template<typename E>
inline auto Err(E&& err) -> result::impl::Err<typename std::decay<E>::type> {
    return result::impl::Err<typename std::decay<E>::type>{std::forward<typename std::decay<E>::type>(err)};
}

#define RXX_RESULT_TRY(...) ({              \
    auto __res = __VA_ARGS__;               \
    if (__res.is_err())                     \
        return Err(__res.unwrap_err());     \
    __res.unwrap();                         \
})

}

#endif /* end of include guard: __RXX_RESULT_DEF_HPP__ */

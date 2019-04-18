#ifndef __RXX_RESULT_DEF_HPP__
#define __RXX_RESULT_DEF_HPP__

#include <cstdio>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <cassert>
#include "rxx/invoke.hpp"
#include "rxx/option/proto.hpp"
#include "rxx/result/proto.hpp"
#include "rxx/str.hpp"
#include "rxx/utility.hpp"

namespace rxx {

template<typename T, typename E>
class Result {
private:
    aligned_union_t<1, T, E> m_buffer;
    bool m_is_ok;

    T& value() & noexcept {
        return *(T*)(&m_buffer);
    }

    T&& value() && noexcept {
        return static_move(*(T*)(&m_buffer));
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
        return static_move(*(E*)(&m_buffer));
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
        new (valueptr()) T{static_move(value)};
    }

    explicit Result(T const& value) noexcept(std::is_nothrow_copy_assignable<T>::value)
        : m_is_ok{true} {
        new (valueptr()) T{value};
    }

    explicit Result(E&& err) noexcept(std::is_nothrow_move_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{static_move(err)};
    }

    explicit Result(E const& err) noexcept(std::is_nothrow_copy_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{err};
    }

    Result(result::Ok<T>&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
        : m_is_ok(true) {
        new (valueptr()) T{static_move(value.m_ok)};
    }

    Result(result::Err<E>&& err) noexcept(std::is_nothrow_move_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{static_move(err.m_err)};
    }

    Result(result::Ok<T> const& value) noexcept(std::is_nothrow_copy_constructible<T>::value)
        : m_is_ok(true) {
        new (valueptr()) T{value.m_ok};
    }

    Result(result::Err<E> const& err) noexcept(std::is_nothrow_copy_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{err.m_err};
    }

    Result(Result&& that) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_constructible<E>::value)
        : m_is_ok{that.m_is_ok} {
        if (m_is_ok) {
            new (valueptr()) T{static_move(that.value())};
        } else {
            new (errorptr()) E{static_move(that.error())};
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
                value() = static_move(that.value());
            } else {
                error() = static_move(that.error());
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

    auto operator=(result::Ok<T>&& value) noexcept(std::is_nothrow_move_assignable<T>::value) -> Result& {
        destroy();
        m_is_ok = true;
        value() = static_move(value.m_ok);
        return *this;
    }

    auto operator=(result::Ok<T> const& value) noexcept(std::is_nothrow_copy_assignable<T>::value) -> Result& {
        destroy();
        m_is_ok = true;
        value() = value.m_ok;
        return *this;
    }

    auto operator=(result::Err<E>&& err) noexcept(std::is_nothrow_move_assignable<E>::value) -> Result& {
        destroy();
        m_is_ok = true;
        error() = static_move(err.m_err);
        return *this;
    }

    auto operator=(result::Err<E> const& err) noexcept(std::is_nothrow_copy_assignable<E>::value) -> Result& {
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

    explicit operator bool() const { return m_is_ok; }

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
    typename std::enable_if<
        !std::is_void<invoke_result_t<decay_t<F>, T&>>::value,
        Result<invoke_result_t<decay_t<F>, T&>, E>>::type
    map(F&& op) {
        if (is_ok()) {
            return Ok(invoke(static_forward<F>(op), value()));
        } else {
            return Err(static_move(error()));
        }
    }

    template<typename F>
    typename std::enable_if<
        std::is_void<invoke_result_t<decay_t<F>, T&>>::value,
        Result<void, E>>::type
    map(F&& op) {
        if (is_ok()) {
            invoke(static_forward<F>(op), value());
            return Ok();
        } else {
            return Err(static_move(error()));
        }
    }

    template<typename F>
    typename std::enable_if<
        !std::is_void<invoke_result_t<decay_t<F>, T const&>>::value,
        Result<invoke_result_t<decay_t<F>, T&>, E>>::type
    map(F&& op) const {
        if (is_ok()) {
            return Ok(invoke(static_forward<F>(op), value()));
        } else {
            return Err(static_move(error()));
        }
    }

    template<typename F>
    typename std::enable_if<
        std::is_void<invoke_result_t<decay_t<F>, T const&>>::value,
        Result<void, E>>::type
    map(F&& op) const {
        if (is_ok()) {
            return Ok(invoke(static_forward<F>(op), value()));
        } else {
            return Err(static_move(error()));
        }
    }

    template<typename F, typename M>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<F>::type, E&>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<M>::type, T&>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<F>::type>::type>::type
    >::type
    map_or_else(F&& fallback, M&& map_func) {
        if (is_ok()) {
            return invoke(static_forward<M>(map_func), value());
        } else {
            return invoke(static_forward<F>(fallback), error());
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
            return invoke(static_forward<M>(map_func), value());
        } else {
            return invoke(static_forward<F>(fallback), error());
        }
    }

    template<typename O>
    typename std::enable_if<
        !std::is_void<invoke_result_t<decay_t<O>, E&>>::value,
        Result<T, invoke_result_t<decay_t<O>, E&>>>::type
    map_err(O&& op) {
        if (!is_ok()) {
            return Err(invoke(static_forward<O>(op), error()));
        } else {
            return Ok(static_move(value()));
        }
    }

    template<typename O>
    typename std::enable_if<
        std::is_void<invoke_result_t<decay_t<O>, E&>>::value,
        Result<T, void>>::type
    map_err(O&& op) {
        if (!is_ok()) {
            invoke(static_forward<O>(op), error());
            return Err();
        } else {
            return Ok(static_move(value()));
        }
    }

    template<typename O>
    typename std::enable_if<
        !std::is_void<invoke_result_t<decay_t<O>, E const&>>::value,
        Result<T, invoke_result_t<decay_t<O>, E const&>>>::type
    map_err(O&& op) const {
        if (!is_ok()) {
            return Err(invoke(static_forward<O>(op), error()));
        } else {
            return Ok(value());
        }
    }

    template<typename O>
    typename std::enable_if<
        std::is_void<invoke_result_t<decay_t<O>, E const&>>::value,
        Result<T, void>>::type
    map_err(O&& op) const {
        if (!is_ok()) {
            invoke(static_forward<O>(op), error());
            return Err();
        } else {
            return Ok(value());
        }
    }

    template<typename U>
    auto and_(Result<U, E> res) -> Result<U, E> {
        if (res.is_ok()) {
            return res;
        } else {
            return Err(static_move(error()));
        }
    }

    template<typename U>
    auto and_(Result<U, E> res) const -> Result<U, E> {
        if (res.is_ok()) {
            return res;
        } else {
            return Err(static_move(error()));
        }
    }

    template<typename F>
    typename std::enable_if<
        is_result_of_same_err_type<
            Result<T, E>,
            typename invoke_result<typename std::decay<F>::type, T&>::type>::value,
        typename invoke_result<typename std::decay<F>::type, T&>::type
    >::type
    and_then(F&& op) {
        if (is_ok()) {
            return invoke(op, value());
        } else {
            return Err(static_move(error()));
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
            return Ok(static_move(value()));
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
            typename invoke_result<typename std::decay<O>::type, E&>::type>::value,
        typename invoke_result<typename std::decay<O>::type, E&>::type
    >::type
    or_else(O&& op) {
        if (is_err()) {
            return invoke(op, error());
        } else {
            return Ok(static_move(value()));
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
            return static_move(value());
        } else {
            return optb;
        }
    }

    auto unwrap_or(T optb) const -> T {
        if (is_ok()) {
            return value();
        } else {
            return optb;
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& op) -> T {
        if (is_ok()) {
            return static_move(value());
        } else {
            return invoke(static_forward<F>(op), error());
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& op) const -> T {
        if (is_ok()) {
            return value();
        } else {
            return invoke(static_forward<F>(op), error());
        }
    }

    auto unwrap() -> T {
        assert(is_ok());
        return static_move(value());
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
        return static_move(value());
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
        return static_move(error());
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
        return static_move(error());
    }

    auto expect_err(Str msg) const -> E {
        if (!is_err()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return static_move(error());
    }

    auto unwrap_or_default() -> T {
        if (is_ok()) {
            return static_move(value());
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
        return static_move(*(E*)(&m_buffer));
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
        new (errorptr()) E{static_move(err)};
    }

    explicit Result(E const& err) noexcept(std::is_nothrow_copy_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{err};
    }

    Result(result::Ok<void>&&) noexcept : m_is_ok(true) {}

    Result(result::Err<E>&& err)
        noexcept(std::is_nothrow_move_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{static_move(err.m_err)};
    }

    Result(result::Ok<void> const&) : m_is_ok(true) {}

    Result(result::Err<E> const& err) noexcept(std::is_nothrow_copy_constructible<E>::value)
        : m_is_ok(false) {
        new (errorptr()) E{err.m_err};
    }

    Result(Result&& that) noexcept(std::is_nothrow_move_constructible<E>::value)
        : m_is_ok{that.m_is_ok} {
        if (!m_is_ok) {
            new (errorptr()) E{static_move(that.error())};
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
                error() = static_move(that.error());
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

    auto operator=(result::Ok<void>&&) noexcept -> Result& {
        destroy();
        m_is_ok = true;
        return *this;
    }

    auto operator=(result::Ok<void> const&) noexcept -> Result& {
        destroy();
        m_is_ok = true;
        return *this;
    }

    auto operator=(result::Err<E>&& err) noexcept(std::is_nothrow_move_assignable<E>::value) -> Result& {
        destroy();
        m_is_ok = true;
        error() = static_move(err.m_err);
        return *this;
    }

    auto operator=(result::Err<E> const& err) noexcept(std::is_nothrow_copy_assignable<E>::value) -> Result& {
        destroy();
        m_is_ok = true;
        error() = err.m_err;
        return *this;
    }

    auto operator==(Result const& that) const -> bool {
        if (m_is_ok && that.m_is_ok) {
            return true;
        } else if (!m_is_ok && !that.m_is_ok) {
            return error() == that.error();
        }
        return false;
    }

    explicit operator bool() const { return m_is_ok; }

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
            invoke(static_forward<F>(op));
            return Ok();
        } else {
            return Err(static_move(error()));
        }
    }

    template<typename F>
    auto map(F&& op) const -> Result<typename invoke_result<typename std::decay<F>::type>::type, E> {
        if (is_ok()) {
            invoke(static_forward<F>(op));
            return Ok();
        } else {
            return Err(static_move(error()));
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
            return invoke(static_forward<M>(map_func));
        } else {
            return invoke(static_forward<F>(fallback), static_move(error()));
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
            return invoke(static_forward<M>(map_func));
        } else {
            return invoke(static_forward<F>(fallback), error());
        }
    }

    template<typename O>
    typename std::enable_if<
        !std::is_void<invoke_result_t<decay_t<O>, E&>>::value,
        Result<void, invoke_result_t<decay_t<O>, E&>>>::type
    map_err(O&& op) {
        if (!is_ok()) {
            return Err(invoke(static_forward<O>(op), error()));
        } else {
            return Ok();
        }
    }

    template<typename O>
    typename std::enable_if<
        std::is_void<invoke_result_t<decay_t<O>, E&>>::value,
        Result<void, invoke_result_t<decay_t<O>, E&>>>::type
    map_err(O&& op) {
        if (!is_ok()) {
            invoke(static_forward<O>(op), error());
            return Err();
        } else {
            return Ok();
        }
    }

    template<typename O>
    typename std::enable_if<
        !std::is_void<invoke_result_t<decay_t<O>, E const&>>::value,
        Result<void, invoke_result_t<decay_t<O>, E const&>>>::type
    map_err(O&& op) const {
        if (!is_ok()) {
            return Err(invoke(static_forward<O>(op), error()));
        } else {
            return Ok();
        }
    }

    template<typename O>
    typename std::enable_if<
        std::is_void<invoke_result_t<decay_t<O>, E const&>>::value,
        Result<void, invoke_result_t<decay_t<O>, E const&>>>::type
    map_err(O&& op) const {
        if (!is_ok()) {
            invoke(static_forward<O>(op), error());
            return Err();
        } else {
            return Ok();
        }
    }

    template<typename U>
    auto and_(Result<U, E> res) -> Result<U, E> {
        if (res.is_ok()) {
            return res;
        } else {
            return Err(static_move(error()));
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
            return Err(static_move(error()));
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
            typename invoke_result<typename std::decay<O>::type, E&>::type>::value,
        typename invoke_result<typename std::decay<O>::type, E&>::type
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
            return invoke(static_forward<F>(op), error());
        }
    }

    template<typename F>
    void unwrap_or_else(F&& op) const {
        if (is_err()) {
            return invoke(static_forward<F>(op), error());
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
        return static_move(error());
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
        return static_move(error());
    }

    auto expect_err(Str msg) const -> E {
        if (!is_err()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return static_move(error());
    }

    void unwrap_or_default() const {}
};

template<typename T>
class Result<T, void> {
private:
    typename std::aligned_storage<sizeof(T)>::type m_buffer;

    T& value() & noexcept {
        return *(T*)(&m_buffer);
    }

    T&& value() && noexcept {
        return static_move(*(T*)(&m_buffer));
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

    void destroy() noexcept {
        value().T::~T();
    }

public:
    explicit Result(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value) {
        new (valueptr()) T{static_move(value)};
    }

    explicit Result(T const& value) noexcept(std::is_nothrow_copy_assignable<T>::value) {
        new (valueptr()) T{value};
    }

    Result(result::Ok<T>&& value) noexcept(std::is_nothrow_move_constructible<T>::value) {
        new (valueptr()) T{static_move(value.m_ok)};
    }

    Result(result::Err<void>&&) noexcept {}

    Result(result::Ok<T> const& value)
        noexcept(std::is_nothrow_copy_constructible<T>::value) {
        new (valueptr()) T{value.m_ok};
    }

    Result(result::Err<void> const&) noexcept {}

    Result(Result&& that) noexcept(std::is_nothrow_move_constructible<T>::value) {
        new (valueptr()) T{static_move(that.value())};
    }

    Result(Result const& that) noexcept(std::is_nothrow_copy_constructible<T>::value) {
        if (that.m_is_ok) {
            new (valueptr()) T{that.value()};
        }
    }

    ~Result() noexcept {
        destroy();
    }

    auto operator=(Result&& that) noexcept(std::is_nothrow_move_assignable<T>::value) -> Result& {
        if (this != &that) {
            destroy();
            value() = static_move(that.value());
        }
        return *this;
    }

    auto operator=(Result const& that) noexcept(std::is_nothrow_copy_assignable<T>::value) -> Result& {
        if (this != &that) {
            destroy();
            value() = that.value();
        }
        return *this;
    }

    auto operator=(result::Ok<T>&& value) noexcept(std::is_nothrow_move_assignable<T>::value) -> Result& {
        destroy();
        value() = static_move(value.m_ok);
        return *this;
    }

    auto operator=(result::Ok<T> const& value) noexcept(std::is_nothrow_copy_assignable<T>::value) -> Result& {
        destroy();
        value() = value.m_ok;
        return *this;
    }

    auto operator=(result::Err<void>&&) noexcept -> Result& {
        destroy();
        return *this;
    }

    auto operator=(result::Err<void> const&) noexcept -> Result& {
        destroy();
        return *this;
    }

    auto operator==(Result const& that) const -> bool {
        return value() == that.value();
    }

    explicit operator bool() const { return true; }

    auto ok() -> Option<T>;
    auto ok() const -> Option<T>;
    auto err() const -> Option<void>;

    constexpr auto is_ok() const -> bool { return true; }

    constexpr auto is_err() const -> bool { return false; }

    template<typename F>
    typename std::enable_if<
        !std::is_void<invoke_result_t<decay_t<F>, T&>>::value,
        Result<invoke_result_t<decay_t<F>, T&>, void>>::type
    map(F&& op) {
        return Ok(invoke(static_forward<F>(op), value()));
    }

    template<typename F>
    typename std::enable_if<
        std::is_void<invoke_result_t<decay_t<F>, T&>>::value,
        Result<invoke_result_t<decay_t<F>, T&>, void>>::type
    map(F&& op) {
        invoke(static_forward<F>(op), value());
        return Ok();
    }

    template<typename F>
    typename std::enable_if<
        !std::is_void<invoke_result_t<decay_t<F>, T const&>>::value,
        Result<invoke_result_t<decay_t<F>, T&>, void>>::type
    map(F&& op) const {
        return Ok(invoke(static_forward<F>(op), value()));
    }

    template<typename F>
    typename std::enable_if<
        std::is_void<invoke_result_t<decay_t<F>, T const&>>::value,
        Result<invoke_result_t<decay_t<F>, T&>, void>>::type
    map(F&& op) const {
        invoke(static_forward<F>(op), value());
        return Ok();
    }

    template<typename U>
    auto and_(Result<U, void> res) -> Result<U, void> {
        return res;
    }

    template<typename U>
    auto and_(Result<U, void> res) const -> Result<U, void> {
        return res;
    }

    template<typename F>
    typename std::enable_if<
        is_result_of_same_err_type<
            Result<T, void>,
            typename invoke_result<typename std::decay<F>::type, T&>::type>::value,
        typename invoke_result<typename std::decay<F>::type, T&>::type
    >::type
    and_then(F&& op) {
        return invoke(op, value());
    }

    template<typename F>
    typename std::enable_if<
        is_result_of_same_err_type<
            Result<T, void>,
            typename invoke_result<typename std::decay<F>::type, T const&>::type>::value,
        typename invoke_result<typename std::decay<F>::type, T const&>::type
    >::type
    and_then(F&& op) const {
        return invoke(op, value());
    }

    auto unwrap_or(T optb) -> T {
        if (is_ok()) {
            return static_move(value());
        } else {
            return optb;
        }
    }

    auto unwrap_or(T optb) const -> T {
        if (is_ok()) {
            return value();
        } else {
            return optb;
        }
    }

    auto unwrap() -> T {
        return static_move(value());
    }

    constexpr auto unwrap() const -> T {
        return value();
    }

    auto expect(Str) -> T {
        return static_move(value());
    }

    constexpr auto expect(Str) const -> T {
        return value();
    }

    auto unwrap_or_default() -> T {
        return static_move(value());
    }

    auto unwrap_or_default() const -> T {
        return value();
    }
};

template<>
class Result<void, void> {
public:
    explicit constexpr Result() noexcept {}

    constexpr Result(result::Ok<void>&&) noexcept {}

    constexpr Result(result::Err<void>&&) noexcept {}

    constexpr Result(result::Ok<void> const&) noexcept {}

    constexpr Result(result::Err<void> const&) noexcept {}

    Result(Result&&) noexcept {}

    Result(Result const&) noexcept {}

    ~Result() = default;

    Result& operator=(Result&&) noexcept { return *this; }

    Result& operator=(Result const&) noexcept { return *this; }

    Result& operator=(result::Ok<void>&&) noexcept { return *this; }

    Result& operator=(result::Ok<void> const&) noexcept { return *this; }

    Result& operator=(result::Err<void>&&) noexcept { return *this; }

    Result& operator=(result::Err<void> const&) noexcept { return *this; }

    constexpr bool operator==(Result const&) const { return true; }

    explicit operator bool() const { return true; }

    auto ok() const -> Option<void>;
    auto err() const -> Option<void>;

    auto is_ok() const -> bool { return true; }
    auto is_err() const -> bool { return true; }

    template<typename F>
    typename std::enable_if<
        !std::is_void<invoke_result_t<decay_t<F>>>::value,
        Result<invoke_result_t<decay_t<F>>, void>>::type
    map(F&& op) const {
        return Ok(invoke(static_forward<F>(op)));
    }

    template<typename F>
    typename std::enable_if<
        std::is_void<invoke_result_t<decay_t<F>>>::value,
        Result<invoke_result_t<decay_t<F>>, void>>::type
    map(F&& op) const {
        invoke(static_forward<F>(op));
        return Ok();
    }

    template<typename U>
    auto and_(Result<U, void> res) -> Result<U, void> {
        return res;
    }

    template<typename U>
    auto and_(Result<U, void> res) const -> Result<U, void> {
        return res;
    }

    template<typename F>
    typename std::enable_if<
        is_result_of_same_err_type<
            Result<void, void>,
            typename invoke_result<typename std::decay<F>::type>::type>::value,
        typename invoke_result<typename std::decay<F>::type>::type
    >::type
    and_then(F&& op) {
        return invoke(op);
    }

    template<typename F>
    typename std::enable_if<
        is_result_of_same_err_type<
            Result<void, void>,
            typename invoke_result<typename std::decay<F>::type>::type>::value,
        typename invoke_result<typename std::decay<F>::type>::type
    >::type
    and_then(F&& op) const {
        return invoke(op);
    }

    void unwrap() const {}

    void expect(Str) const {}

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
inline auto Ok(T&& value) -> result::Ok<typename std::decay<T>::type> {
    return result::Ok<typename std::decay<T>::type>(static_forward<typename std::decay<T>::type>(value));
}

template<typename E>
inline auto Err(E&& err) -> result::Err<typename std::decay<E>::type> {
    return result::Err<typename std::decay<E>::type>{static_forward<typename std::decay<E>::type>(err)};
}

#define RXX_RESULT_TRY(...) ({              \
    auto __res = __VA_ARGS__;               \
    if (__res.is_err())                     \
        return Err(__res.unwrap_err());     \
    __res.unwrap();                         \
})

}

#endif /* end of include guard: __RXX_RESULT_DEF_HPP__ */

#ifndef __RXX_CORE_RESULT_DEF_HPP__
#define __RXX_CORE_RESULT_DEF_HPP__

#include <type_traits>
#include <utility>
#include <algorithm>
#include "rxx/core/bool/def.hpp"
#include "rxx/core/result/ok.hpp"
#include "rxx/core/result/err.hpp"

namespace rxx {

template<typename T, typename E>
class Result;

template<typename T>
inline auto Ok(T&& value) -> result::impl::Ok<T>;

template<typename E>
inline auto Err(E&& err) -> result::impl::Err<E>;

namespace result {
namespace impl{

template<typename T>
constexpr auto constexpr_max(T const& a, T const& b) -> T const& {
    return a > b ? a : b;
}

}
}

template<typename T, typename E>
class Result {
private:
    Bool m_is_ok;
    typename std::aligned_storage<result::impl::constexpr_max(sizeof(T), sizeof(E)),
                                  result::impl::constexpr_max(alignof(T), alignof(E))>::type m_buffer;

    auto val() -> T& {
        return *reinterpret_cast<T*>(&m_buffer);
    }

    auto val_const() const -> T const& {
        return *reinterpret_cast<T const*>(&m_buffer);
    }

    auto err() -> E& {
        return *reinterpret_cast<E*>(&m_buffer);
    }

    auto err_const() const -> E const& {
        return *reinterpret_cast<E*>(&m_buffer);
    }

public:
    using TypeT = T;
    using TypeE = E;

    explicit Result(Bool is_ok, T&& value) : m_is_ok(is_ok) {
        *reinterpret_cast<T*>(m_buffer) = std::forward<T>(value);
    }

    explicit Result(Bool is_ok, E&& err) : m_is_ok(is_ok) {
        *reinterpret_cast<E*>(m_buffer) = std::forward<E>(err);
    }

    Result(result::impl::Ok<T> value) : m_is_ok(true) {
        val() = std::move(value.m_ok);
    }

    Result(result::impl::Err<E> err) : m_is_ok(false) {
        this->err() = std::move(err.m_err);
    }

    Result(Result&& that) : m_is_ok(that.m_is_ok) {
        if (m_is_ok) {
            val() = std::move(that.val());
        } else {
            err() = std::move(that.err());
        }
    }

    ~Result() {
        if (m_is_ok) {
            val().T::~T();
        } else {
            err().E::~E();
        }
    }

    auto operator=(Result&& that) -> Result& {
        if (this != &that) {
          if (m_is_ok) {
              val().T::~T();
          } else {
              err().E::~E();
          }

          m_is_ok = that.m_is_ok;
          if (that.m_is_ok) {
              val() = std::move(that.val());
          } else {
              err() = std::move(that.err());
          }
        }
        return *this;
    }

    auto operator=(T&& value) -> Result& {
        if (m_is_ok) {
            val().T::~T();
        } else {
            err().E::~E();
        }
        m_is_ok = true;
        val() = std::forward<T>(value);
        return *this;
    }

    auto operator==(Result const& that) const -> Bool {
        if (m_is_ok && that.m_is_ok) {
            return val_const() == that.val_const();
        } else if (!m_is_ok && !that.m_is_ok) {
            return err_const() == that.err_const();
        }
    }

    auto is_ok() const -> Bool {
        return m_is_ok;
    }

    auto is_err() const -> Bool {
        return !m_is_ok;
    }

    auto unwrap() -> T;

    auto unwrap_err() -> E;

    template<typename U, typename F>
    auto map(F op) -> Result<U, E> {
        if (is_ok()) {
            return Ok(std::forward<U>(op(val())));
        } else {
            return Err(std::move(err()));
        }
    }
};

template<typename T>
inline auto Ok(T&& value) -> result::impl::Ok<T> {
    return result::impl::Ok<T> { std::forward<T>(value) };
}

template<typename E>
inline auto Err(E&& err) -> result::impl::Err<E> {
    return result::impl::Err<E> { std::forward<E>(err) };
}

#define RXX_RESULT_TRY(...) ({                                                                                      \
    auto __res = __VA_ARGS__;                                                                                       \
    if (__res.is_err())                                                                                             \
        return Err(std::forward<std::remove_reference<decltype(__res.unwrap_err())>::type>(__res.unwrap_err()));    \
    __res.unwrap();                                                                                                 \
})

}

#endif /* end of include guard: __RXX_CORE_RESULT_DEF_HPP__ */

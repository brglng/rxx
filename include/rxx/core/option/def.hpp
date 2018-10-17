#ifndef __RXX_CORE_OPTION_DEF_HPP__
#define __RXX_CORE_OPTION_DEF_HPP__

#include <type_traits>
#include <utility>
#include "rxx/core/bool/def.hpp"
#include "rxx/core/option/none.hpp"
#include "rxx/rxx/macros.hpp"

namespace rxx {

class str;
template<typename T, typename E> class Result;

template<typename T>
class Option;

template<typename T>
inline auto Some(T&& value) -> Option<T>;

template<typename T>
class Option {
    Bool m_is_some;
    typename std::aligned_storage<sizeof(T), alignof(T)>::type m_value;

    T& val() {
        return *reinterpret_cast<T*>(&m_value);
    }

    T& val_const() const {
        return *reinterpret_cast<T const*>(&m_value);
    }

public:
    explicit Option(Bool is_some, T&& value) noexcept : m_is_some(is_some) {
        val() = std::forward<T>(value);
    }

    Option(option::impl::None const&) noexcept : m_is_some(false) {}

    Option(Option&& that) noexcept : m_is_some(that.m_is_some) {
        val() = std::move(that.val());
        that.m_is_some = false;
    }

    ~Option() {
        if (m_is_some) {
            val().T::~T();
        }
    }

    auto operator=(Option&& that) noexcept -> Option& {
        if (this != &that) {
            if (that.m_is_some) {
                m_is_some = true;
                val() = std::move(that.val());
                that.m_is_some = false;
            } else {
                m_is_some = false;
                val().T::~T();
            }
        }
        return *this;
    }

    auto operator=(option::impl::None const&) noexcept -> Option& {
        if (m_is_some) {
            m_is_some = false;
            val().T::~T();
        }
        return *this;
    }

    auto operator=(T&& that) noexcept -> Option& {
        m_is_some = true;
        val() = std::forward<T>(that);
        return *this;
    }

    auto is_some() const noexcept -> Bool {
        return m_is_some;
    }

    auto is_none() const noexcept -> Bool {
        return !m_is_some;
    }

    auto as_ref() noexcept -> Option<T&> {
        return Option<T&>(m_is_some, val());
    }

    auto as_const() const noexcept -> Option<T const&> {
        return Option<T const&>(m_is_some, val_const());
    }

    auto expect(const str msg) noexcept -> T;

    auto unwrap() -> T {
        RXX_ASSERT(is_some());
        return val();
    }

    auto unwrap_or(T&& def) noexcept -> T {
        if (is_some()) {
            return std::move(val());
        } else {
            return std::forward<T>(def);
        }
    }

    template<typename F>
    auto unwrap_or_else(F f) noexcept -> T {
        if (is_some()) {
            return std::move(val());
        } else {
            return std::forward<T>(f());
        }
    }

    template<typename U, typename F>
    auto map(F f) noexcept -> Option<U> {
        if (is_some()) {
            return Some(std::move(f(val())));
        } else {
            return None;
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F f) noexcept -> U {
        if (is_some()) {
            return f(val());
        } else {
            return std::forward<U>(def);
        }
    }

    template<typename U, typename D, typename F>
    auto map_or_else(D def, F f) noexcept -> U {
        if (is_some()) {
            return f(val());
        } else {
            return std::forward<U>(def());
        }
    }

    template<typename E>
    auto ok_or(E&& err) -> Result<T, E>;

    auto operator==(Option const& that) const noexcept -> Bool {
        if (m_is_some && that.m_is_some) {
            if (val_const() == that.val_const()) {
                return true;
            }
        }

        if (!m_is_some && !that.m_is_some) {
            return true;
        }

        return false;
    }

    auto operator==(option::impl::None const&) const noexcept -> Bool {
      if (!m_is_some) {
          return true;
      }
      return false;
    }
};

template<typename T>
inline auto Some(T&& value) -> Option<T> {
    return Option<T> { true, std::forward<T>(value) };
}

template<typename T>
inline auto Some(T& value) -> Option<T&> {
    return Option<T&> { value };
}

}

#include "rxx/core/option/ref/def.hpp"

#endif /* end of include guard: __RXX_CORE_OPTION_DEF_HPP__ */

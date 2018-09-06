#ifndef __RXX_CORE_OPTION_MOD_HPP__
#define __RXX_CORE_OPTION_MOD_HPP__

#include <type_traits>
#include <utility>
#include "rxx/core/bool.hpp"
#include "rxx/core/str.hpp"
#include "rxx/core/unit.hpp"
#include "rxx/rxx/macros.hpp"

#include "rxx/core/option/none.hpp"

namespace rxx {

template<typename T>
class Option {
    Bool m_is_some;
    typename std::aligned_storage<sizeof(T)>::type m_value;

public:
    explicit Option(Bool is_some, T&& value) : m_is_some(is_some) {
        *reinterpret_cast<T*>(m_value) = value;
    }

    explicit Option(Bool is_some, T const& value) : m_is_some(is_some) {
        *reinterpret_cast<T*>(m_value) = value;
    }

    Option(Option&& that) : m_is_some(that.m_is_some) {
        *reinterpret_cast<T*>(m_value) = *reinterpret_cast<T*>(that.m_value);
        that.m_is_some = false;
    }

    Option(Option const& that) : m_is_some(that.m_is_some) {
        *reinterpret_cast<T*>(m_value) = *reinterpret_cast<T const*>(that.m_value);
    }

    ~Option() {
        if (m_is_some) {
            reinterpret_cast<T*>(m_value)->T::~T();
        }
    }

    auto operator=(Option&& that) -> Option& {
        if (this != &that) {
            if (that.m_is_some) {
                m_is_some = true;
                *reinterpret_cast<T*>(m_value) = *reinterpret_cast<T*>(that.m_value);
                that.m_is_some = false;
            } else {
                m_is_some = false;
                reinterpret_cast<T*>(m_value)->T::~T();
            }
        }
        return *this;
    }

    auto operator=(Option const& that) -> Option& {
        if (this != &that) {
            if (that.m_is_some) {
              m_is_some = true;
              *reinterpret_cast<T*>(m_value) = *reinterpret_cast<T*>(that.m_value);
          } else {
              m_is_some = false;
              reinterpret_cast<T*>(m_value)->T::~T();
          }
        }
        return *this;
    }

    auto operator=(OptionNone const&) -> Option& {
        if (m_is_some) {
            m_is_some = false;
            reinterpret_cast<T*>(m_value)->T::~T();
        }
    }

    auto operator=(T&& that) -> Option& {
        m_is_some = true;
        *reinterpret_cast<T*>(m_value) = that;
    }

    auto operator=(T const& that) -> Option& {
        m_is_some = true;
        *reinterpret_cast<T*>(m_value) = that;
    }

    auto is_some() const -> Bool {
        return m_is_some;
    }

    auto is_none() const -> Bool {
        return !m_is_some;
    }

    auto as_ref() -> Option<T&> {
        return Option<T&>(m_is_some, *reinterpret_cast<T*>(m_value));
    }

    auto as_const() const -> Option<T const&> {
        return Option<T const&>(m_is_some, *reinterpret_cast<T const*>(m_value));
    }

    auto expect(const str msg) -> T {
        RXX_ASSERT(is_some(), msg);
        return std::move(*reinterpret_cast<T*>(m_value));
    }

    auto unwrap() -> T {
        RXX_ASSERT(is_some());
        return std::move(*reinterpret_cast<T*>(m_value));
    }

    auto unwrap_or(T&& def) -> T {
        if (is_some()) {
            return std::move(*reinterpret_cast<T*>(m_value));
        } else {
            return std::forward<T>(def);
        }
    }

    template<typename F>
    auto unwrap_or_else(F f) -> T {
        if (is_some()) {
            return std::move(*reinterpret_cast<T*>(m_value));
        } else {
            return f();
        }
    }

    template<typename U, typename F>
    auto map(F f) -> Option<U> {
        if (is_some()) {
            return f(std::move(*reinterpret_cast<T*>(m_value)));
        } else {
            return None;
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F f) -> U {
        if (is_some()) {
            return f(std::move(*reinterpret_cast<T*>(m_value)));
        } else {
            return std::forward<U>(def);
        }
    }

    template<typename U, typename D, typename F>
    auto map_or_else(D&& def, F f) -> U {
    }

    auto operator==(Option const& that) const -> Bool {
        if (m_is_some && that.m_is_some) {
            if (*reinterpret_cast<T const*>(m_value) == *reinterpret_cast<T const*>(m_value)) {
                return true;
            }
        }

        if (!m_is_some && !that.m_is_some) {
            return true;
        }

        return false;
    }

    auto operator==(OptionNone const&) const -> Bool {
      if (!m_is_some) {
          return true;
      }
      return false;
    }
};

template<typename T>
class Option<T&&> {
    static_assert(sizeof(T) == 0, "Option of rvalue references disallowed");
};

template<typename T>
auto Some(T&& value) -> Option<T> {
    return Option<T>{true, std::forward<T>(value)};
}

}

#include "rxx/core/option/ptr.hpp"
#include "rxx/core/option/ref.hpp"

#endif /* end of include guard: __RXX_CORE_OPTION_NONE_HPP__ */

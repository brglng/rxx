#ifndef __RXX_CORE_OPTION_MOD_HPP__
#define __RXX_CORE_OPTION_MOD_HPP__

#include <type_traits>
#include <utility>
#include "rxx/core/bool.hpp"
#include "rxx/core/str.hpp"
#include "rxx/core/unit.hpp"
#include "rxx/std/macros.hpp"

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
    if (is_none()) {
      panic(msg);
    }
    RXX_ASSERT(is_some());
    return std::move(*reinterpret_cast<T*>(m_value));
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
auto Some(T&& value) -> Option<T> {
  return Option<T>{true, std::forward<T>(value)};
}

}

#include "rxx/core/option/ptr.hpp"

#endif /* end of include guard: __RXX_CORE_OPTION_NONE_HPP__ */

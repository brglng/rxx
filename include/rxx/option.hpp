#ifndef __RXX_OPTION_HPP__
#define __RXX_OPTION_HPP__

#include <type_traits>
#include <utility>
#include "rxx/bool.hpp"
#include "rxx/unit.hpp"

namespace rxx {

struct OptionNone {
  explicit constexpr OptionNone(int) {}
};
constexpr OptionNone None{0};

template<typename T>
class Option {
  Bool m_is_some;
  typename std::aligned_storage<sizeof(T)>::type m_value;

public:
  explicit Option(Bool is_some, T value) : m_is_some(is_some) {
    *reinterpret_cast<T*>(m_value) = value;
  }

  Option(Option&& that) : m_is_some(that.m_is_some) {
    *reinterpret_cast<T*>(m_value) = *reinterpret_cast<T*>(that.m_value);
  }

  Option(Option& that) : Option(std::move(that)) {}

  ~Option() {
    if (m_is_some) {
      reinterpret_cast<T*>(m_value)->T::~T();
    }
  }

  auto operator=(Option&& that) -> Option& {
    if (this != &that) {
      if (that.m_is_some) {
        m_is_some = that.m_is_some;
        *reinterpret_cast<T*>(m_value) = *reinterpret_cast<T*>(that.m_value);
        that.m_is_some = False();
      } else {
        m_is_some = False();
        reinterpret_cast<T*>(m_value)->T::~T();
      }
    }
    return *this;
  }

  auto operator=(Option& that) -> Option& {
    return *this = std::move(that);
  }

  auto operator=(OptionNone const&) -> Option& {
    if (m_is_some) {
      m_is_some = False();
      reinterpret_cast<T*>(m_value)->T::~T();
    }
  }

  auto operator=(T that) -> Option& {
    if (m_is_some) {
      reinterpret_cast<T*>(m_value)->T::~T();
    }

    m_is_some = True();
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

  auto as_const() -> Option<T const&> {
    return Option<T const&>(m_is_some, *reinterpret_cast<T*>(m_value));
  }

  auto operator==(Option const& that) const -> Bool {
    if (m_is_some && that.m_is_some) {
      if (*reinterpret_cast<T*>(m_value) == *reinterpret_cast<T*>(m_value)) {
        return True();
      }
    }

    if (!m_is_some && !that.m_is_some) {
      return True();
    }

    return False();
  }

  auto operator==(OptionNone const&) const -> Bool {
    if (!m_is_some) {
      return True();
    }
    return False();
  }
};

template<typename T>
auto Some(T value) -> Option<T> {
  return Option<T>{True(), value};
}

}

#endif /* end of include guard: __RXX_OPTION_HPP__ */

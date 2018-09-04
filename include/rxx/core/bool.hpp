#ifndef __RXX_CORE_BOOL_HPP__
#define __RXX_CORE_BOOL_HPP__

#include <utility>

namespace rxx {

class Bool {
  bool m_value;

public:
  Bool(bool value) : m_value{value} {}

  operator bool() const {
    return m_value;
  }

  auto operator=(bool value) -> Bool& {
    m_value = value;
    return *this;
  }

  auto operator==(Bool const& that) const -> Bool {
    return this->m_value == that.m_value;
  }

  auto operator!=(Bool const& that) const -> Bool {
    return this->m_value != that.m_value;
  }

  auto operator&&(Bool const& that) const -> Bool {
    return this->m_value && that.m_value;
  }

  auto operator||(Bool const& that) const -> Bool {
    return this->m_value || that.m_value;
  }

  auto operator!() const -> Bool {
    return !this->m_value;
  }

  auto operator&(Bool const& that) const ->Bool {
    return *this && that;
  }

  auto operator|(Bool const& that) const -> Bool {
    return *this || that;
  }

  auto operator~() const -> Bool {
    return !(*this);
  }

  auto operator^(Bool const& that) const -> Bool {
    return *this != that;
  }
};

}

#endif /* end of include guard: __RXX_CORE_BOOL_HPP__ */

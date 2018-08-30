#ifndef __RXX_BOOLEAN_HPP__
#define __RXX_BOOLEAN_HPP__

#include <utility>

namespace rxx {

class Bool {
  bool m_value;

public:
  explicit Bool(bool value) : m_value{value} {}
  Bool(Bool const& that) : m_value{that.m_value} {}
  auto operator=(Bool const& that) -> Bool& {
    m_value = that.m_value;
    return *this;
  }

  operator bool() const {
    return m_value;
  }

  auto operator==(Bool const& that) const -> Bool {
    return Bool{this->m_value == that.m_value};
  }

  auto operator!=(Bool const& that) const -> Bool {
    return Bool{this->m_value != that.m_value};
  }

  auto operator&&(Bool const& that) const -> Bool {
    return Bool{this->m_value && that.m_value};
  }

  auto operator||(Bool const& that) const -> Bool {
    return Bool{this->m_value || that.m_value};
  }

  auto operator!() const -> Bool {
    return Bool{!this->m_value};
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

auto True() -> Bool {
  return Bool{true};
}

auto False() -> Bool {
  return Bool{false};
}

}

#endif /* end of include guard: __RXX_BOOLEAN_HPP__ */

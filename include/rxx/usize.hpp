#ifndef __RXX_USIZE_HPP__
#define __RXX_USIZE_HPP__

#include <cstdlib>
#include "rxx/bool.hpp"

namespace rxx {

class usize {
  std::size_t m_value;
  explicit usize(std::size_t value) : m_value{value} {}

  usize(usize const& that) {
    m_value = that.m_value;
  }

  auto operator=(usize const& that) -> usize& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(usize const& that) const -> Bool {
    return Bool{m_value == that.m_value};
  }

  auto operator!=(usize const& that) const -> Bool {
    return Bool{m_value != that.m_value};
  }

  auto operator>(usize const& that) const -> Bool {
    return Bool{m_value > that.m_value};
  }

  auto operator<(usize const& that) const -> Bool {
    return Bool{m_value < that.m_value};
  }

  auto operator>=(usize const& that) const -> Bool {
    return Bool{m_value >= that.m_value};
  }

  auto operator<=(usize const& that) const -> Bool {
    return Bool{m_value <= that.m_value};
  }
};

}

#endif /* end of include guard: __RXX_USIZE_HPP__ */

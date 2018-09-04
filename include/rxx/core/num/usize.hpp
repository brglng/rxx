#ifndef __RXX_CORE_NUM_USIZE_HPP__
#define __RXX_CORE_NUM_USIZE_HPP__

#include <cstdlib>
#include "rxx/core/bool.hpp"

namespace rxx {

class usize {
  std::size_t m_value;

public:
  usize(std::size_t value) : m_value{value} {}

  usize(usize const& that) {
    m_value = that.m_value;
  }

  auto operator=(usize const& that) -> usize& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(usize const& that) const -> Bool {
    return m_value == that.m_value;
  }

  auto operator!=(usize const& that) const -> Bool {
    return m_value != that.m_value;
  }

  auto operator>(usize const& that) const -> Bool {
    return m_value > that.m_value;
  }

  auto operator<(usize const& that) const -> Bool {
    return m_value < that.m_value;
  }

  auto operator>=(usize const& that) const -> Bool {
    return m_value >= that.m_value;
  }

  auto operator<=(usize const& that) const -> Bool {
    return m_value <= that.m_value;
  }

  auto as_size_t() const -> std::size_t {
    return m_value;
  }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_USIZE_HPP__ */

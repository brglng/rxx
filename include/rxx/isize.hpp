#ifndef __RXX_ISIZE_HPP__
#define __RXX_ISIZE_HPP__

#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class isize {
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
  int64_t m_value;
  explicit isize(int64_t value) : m_value{value} {}
#else
  int32_t m_value;
  explicit isize(int32_t value) : m_value{value} {}
#endif

  isize(isize const& that) {
    m_value = that.m_value;
  }

  auto operator=(isize const& that) -> isize& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(isize const& that) const -> Bool {
    return Bool{m_value == that.m_value};
  }

  auto operator!=(isize const& that) const -> Bool {
    return Bool{m_value != that.m_value};
  }

  auto operator>(isize const& that) const -> Bool {
    return Bool{m_value > that.m_value};
  }

  auto operator<(isize const& that) const -> Bool {
    return Bool{m_value < that.m_value};
  }

  auto operator>=(isize const& that) const -> Bool {
    return Bool{m_value >= that.m_value};
  }

  auto operator<=(isize const& that) const -> Bool {
    return Bool{m_value <= that.m_value};
  }
};

}

#endif /* end of include guard: __RXX_ISIZE_HPP__ */

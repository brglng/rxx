#ifndef __RXX_I8_HPP__
#define __RXX_I8_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class i8 {
  std::int8_t m_value;
public:
  explicit i8(std::int8_t value) : m_value{value} {}

  i8(i8 const& that) {
    m_value = that.m_value;
  }

  auto operator=(i8 const& that) -> i8& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(i8 const& rh) const -> Bool {
    return Bool{m_value == rh.m_value};
  }

  auto operator!=(i8 const& rh) const -> Bool {
    return Bool{m_value != rh.m_value};
  }

  auto operator>(i8 const& rh) const -> Bool {
    return Bool{m_value > rh.m_value};
  }

  auto operator<(i8 const& rh) const -> Bool {
    return Bool{m_value < rh.m_value};
  }

  auto operator>=(i8 const& rh) const -> Bool {
    return Bool{m_value >= rh.m_value};
  }

  auto operator<=(i8 const& rh) const -> Bool {
    return Bool{m_value <= rh.m_value};
  }
};

}

#endif /* end of include guard: __RXX_I8_HPP__ */

#ifndef __RXX_I32_HPP__
#define __RXX_I32_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class i32 {
  std::int32_t m_value;
public:
  explicit i32(std::int32_t value) : m_value{value} {}

  i32(i32 const& that) {
    m_value = that.m_value;
  }

  auto operator=(i32 const& that) -> i32& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(i32 const& rh) const -> Bool {
    return Bool{m_value == rh.m_value};
  }

  auto operator!=(i32 const& rh) const -> Bool {
    return Bool{m_value != rh.m_value};
  }

  auto operator>(i32 const& rh) const -> Bool {
    return Bool{m_value > rh.m_value};
  }

  auto operator<(i32 const& rh) const -> Bool {
    return Bool{m_value < rh.m_value};
  }

  auto operator>=(i32 const& rh) const -> Bool {
    return Bool{m_value >= rh.m_value};
  }

  auto operator<=(i32 const& rh) const -> Bool {
    return Bool{m_value <= rh.m_value};
  }
};

}

#endif /* end of include guard: __RXX_I32_HPP__ */

#ifndef __RXX_U32_HPP__
#define __RXX_U32_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class u32 {
  std::uint32_t m_value;
public:
  u32(std::uint32_t value) : m_value{value} {}

  u32(u32 const& that) {
    m_value = that.m_value;
  }

  auto operator=(u32 const& that) -> u32& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(u32 const& rh) const -> Bool {
    return m_value == rh.m_value;
  }

  auto operator!=(u32 const& rh) const -> Bool {
    return m_value != rh.m_value;
  }

  auto operator>(u32 const& rh) const -> Bool {
    return m_value > rh.m_value;
  }

  auto operator<(u32 const& rh) const -> Bool {
    return m_value < rh.m_value;
  }

  auto operator>=(u32 const& rh) const -> Bool {
    return m_value >= rh.m_value;
  }

  auto operator<=(u32 const& rh) const -> Bool {
    return m_value <= rh.m_value;
  }
};

}

#endif /* end of include guard: __RXX_U32_HPP__ */

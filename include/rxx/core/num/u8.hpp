#ifndef __RXX_U8_HPP__
#define __RXX_U8_HPP__

#include <utility>
#include <cstdint>
#include "rxx/core/bool.hpp"

namespace rxx {

class u8 {
  std::uint8_t m_value;
public:
  u8(std::uint8_t value) : m_value{value} {}

  u8(u8 const& that) {
    m_value = that.m_value;
  }

  auto operator=(u8 const& that) -> u8& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(u8 const& rh) const -> Bool {
    return m_value == rh.m_value;
  }

  auto operator!=(u8 const& rh) const -> Bool {
    return m_value != rh.m_value;
  }

  auto operator>(u8 const& rh) const -> Bool {
    return m_value > rh.m_value;
  }

  auto operator<(u8 const& rh) const -> Bool {
    return m_value < rh.m_value;
  }

  auto operator>=(u8 const& rh) const -> Bool {
    return m_value >= rh.m_value;
  }

  auto operator<=(u8 const& rh) const -> Bool {
    return m_value <= rh.m_value;
  }
};

}

#endif /* end of include guard: __RXX_U8_HPP__ */

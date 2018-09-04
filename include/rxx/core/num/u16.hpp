#ifndef __RXX_U16_HPP__
#define __RXX_U16_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class u16 {
  std::uint16_t m_value;
public:
  u16(std::uint16_t value) : m_value{value} {}

  u16(u16 const& that) {
    m_value = that.m_value;
  }

  auto operator=(u16 const& that) -> u16& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(u16 const& rh) const -> Bool {
    return m_value == rh.m_value;
  }

  auto operator!=(u16 const& rh) const -> Bool {
    return m_value != rh.m_value;
  }

  auto operator>(u16 const& rh) const -> Bool {
    return m_value > rh.m_value;
  }

  auto operator<(u16 const& rh) const -> Bool {
    return m_value < rh.m_value;
  }

  auto operator>=(u16 const& rh) const -> Bool {
    return m_value >= rh.m_value;
  }

  auto operator<=(u16 const& rh) const -> Bool {
    return m_value <= rh.m_value;
  }
};

}

#endif /* end of include guard: __RXX_U16_HPP__ */

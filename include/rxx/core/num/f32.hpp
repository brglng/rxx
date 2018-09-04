#ifndef __RXX_F32_HPP__
#define __RXX_F32_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class f32 {
  float m_value;
public:
  f32(float value) : m_value{value} {}

  auto operator==(f32 const& rh) const -> Bool {
    return m_value == rh.m_value;
  }

  auto operator!=(f32 const& rh) const -> Bool {
    return m_value != rh.m_value;
  }

  auto operator>(f32 const& rh) const -> Bool {
    return m_value > rh.m_value;
  }

  auto operator<(f32 const& rh) const -> Bool {
    return m_value < rh.m_value;
  }

  auto operator>=(f32 const& rh) const -> Bool {
    return m_value >= rh.m_value;
  }

  auto operator<=(f32 const& rh) const -> Bool {
    return m_value <= rh.m_value;
  }
};

}

#endif /* end of include guard: __RXX_F32_HPP__ */

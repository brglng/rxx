#ifndef __RXX_F32_HPP__
#define __RXX_F32_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class f32 {
  float m_value;
public:
  explicit f32(float value) : m_value{value} {}

  f32(f32 const& that) {
    m_value = that.m_value;
  }

  auto operator=(f32 const& that) -> f32& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(f32 const& rh) const -> Bool {
    return Bool{m_value == rh.m_value};
  }

  auto operator!=(f32 const& rh) const -> Bool {
    return Bool{m_value != rh.m_value};
  }

  auto operator>(f32 const& rh) const -> Bool {
    return Bool{m_value > rh.m_value};
  }

  auto operator<(f32 const& rh) const -> Bool {
    return Bool{m_value < rh.m_value};
  }

  auto operator>=(f32 const& rh) const -> Bool {
    return Bool{m_value >= rh.m_value};
  }

  auto operator<=(f32 const& rh) const -> Bool {
    return Bool{m_value <= rh.m_value};
  }
};

}

#endif /* end of include guard: __RXX_F32_HPP__ */

#ifndef __RXX_F64_HPP__
#define __RXX_F64_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class f64 {
  float m_value;
public:
  f64(float value) : m_value{value} {}

  auto operator==(f64 const& rh) const -> Bool {
    return m_value == rh.m_value;
  }

  auto operator!=(f64 const& rh) const -> Bool {
    return m_value != rh.m_value;
  }

  auto operator>(f64 const& rh) const -> Bool {
    return m_value > rh.m_value;
  }

  auto operator<(f64 const& rh) const -> Bool {
    return m_value < rh.m_value;
  }

  auto operator>=(f64 const& rh) const -> Bool {
    return m_value >= rh.m_value;
  }

  auto operator<=(f64 const& rh) const -> Bool {
    return m_value <= rh.m_value;
  }
};

}

#endif /* end of include guard: __RXX_F64_HPP__ */

#ifndef __RXX_F64_HPP__
#define __RXX_F64_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class f64 {
  float m_value;
public:
  explicit f64(float value) : m_value{value} {}

  f64(f64 const& that) {
    m_value = that.m_value;
  }

  auto operator=(f64 const& that) -> f64& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(f64 const& rh) const -> Bool {
    return Bool{m_value == rh.m_value};
  }

  auto operator!=(f64 const& rh) const -> Bool {
    return Bool{m_value != rh.m_value};
  }

  auto operator>(f64 const& rh) const -> Bool {
    return Bool{m_value > rh.m_value};
  }

  auto operator<(f64 const& rh) const -> Bool {
    return Bool{m_value < rh.m_value};
  }

  auto operator>=(f64 const& rh) const -> Bool {
    return Bool{m_value >= rh.m_value};
  }

  auto operator<=(f64 const& rh) const -> Bool {
    return Bool{m_value <= rh.m_value};
  }
};

}

#endif /* end of include guard: __RXX_F64_HPP__ */

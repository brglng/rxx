#ifndef __RXX_I16_HPP__
#define __RXX_I16_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class i16 {
  std::int16_t m_value;
public:
  i16(std::int16_t value) : m_value{value} {}

  auto operator==(i16 const& rh) const -> Bool {
    return m_value == rh.m_value;
  }

  auto operator!=(i16 const& rh) const -> Bool {
    return m_value != rh.m_value;
  }

  auto operator>(i16 const& rh) const -> Bool {
    return m_value > rh.m_value;
  }

  auto operator<(i16 const& rh) const -> Bool {
    return m_value < rh.m_value;
  }

  auto operator>=(i16 const& rh) const -> Bool {
    return m_value >= rh.m_value;
  }

  auto operator<=(i16 const& rh) const -> Bool {
    return m_value <= rh.m_value;
  }
};

}

#endif /* end of include guard: __RXX_I16_HPP__ */

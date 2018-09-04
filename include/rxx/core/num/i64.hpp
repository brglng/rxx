#ifndef __RXX_I64_HPP__
#define __RXX_I64_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class i64 {
  std::int64_t m_value;
public:
  i64(std::int64_t value) : m_value{value} {}

  auto operator==(i64 const& rh) const -> Bool {
    return m_value == rh.m_value;
  }

  auto operator!=(i64 const& rh) const -> Bool {
    return m_value != rh.m_value;
  }

  auto operator>(i64 const& rh) const -> Bool {
    return m_value > rh.m_value;
  }

  auto operator<(i64 const& rh) const -> Bool {
    return m_value < rh.m_value;
  }

  auto operator>=(i64 const& rh) const -> Bool {
    return m_value >= rh.m_value;
  }

  auto operator<=(i64 const& rh) const -> Bool {
    return m_value <= rh.m_value;
  }
};

}

#endif /* end of include guard: __RXX_I64_HPP__ */

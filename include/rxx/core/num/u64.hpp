#ifndef __RXX_U64_HPP__
#define __RXX_U64_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class u64 {
  std::uint64_t m_value;
public:
  u64(std::uint64_t value) : m_value{value} {}

  u64(u64 const& that) {
    m_value = that.m_value;
  }

  auto operator=(u64 const& that) -> u64& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(u64 const& rh) const -> Bool {
    return m_value == rh.m_value;
  }

  auto operator!=(u64 const& rh) const -> Bool {
    return m_value != rh.m_value;
  }

  auto operator>(u64 const& rh) const -> Bool {
    return m_value > rh.m_value;
  }

  auto operator<(u64 const& rh) const -> Bool {
    return m_value < rh.m_value;
  }

  auto operator>=(u64 const& rh) const -> Bool {
    return m_value >= rh.m_value;
  }

  auto operator<=(u64 const& rh) const -> Bool {
    return m_value <= rh.m_value;
  }
};

}

#endif /* end of include guard: __RXX_U64_HPP__ */

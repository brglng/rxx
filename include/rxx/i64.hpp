#ifndef __RXX_I64_HPP__
#define __RXX_I64_HPP__

#include <utility>
#include <cstdint>
#include "rxx/bool.hpp"

namespace rxx {

class i64 {
  std::int64_t m_value;
public:
  explicit i64(std::int64_t value) : m_value{value} {}

  i64(i64 const& that) {
    m_value = that.m_value;
  }

  auto operator=(i64 const& that) -> i64& {
    m_value = that.m_value;
    return *this;
  }

  auto operator==(i64 const& rh) const -> Bool {
    return Bool{m_value == rh.m_value};
  }

  auto operator!=(i64 const& rh) const -> Bool {
    return Bool{m_value != rh.m_value};
  }

  auto operator>(i64 const& rh) const -> Bool {
    return Bool{m_value > rh.m_value};
  }

  auto operator<(i64 const& rh) const -> Bool {
    return Bool{m_value < rh.m_value};
  }

  auto operator>=(i64 const& rh) const -> Bool {
    return Bool{m_value >= rh.m_value};
  }

  auto operator<=(i64 const& rh) const -> Bool {
    return Bool{m_value <= rh.m_value};
  }
};

}

#endif /* end of include guard: __RXX_I64_HPP__ */

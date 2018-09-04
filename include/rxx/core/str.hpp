#ifndef __RXX_CORE_STR_HPP__
#define __RXX_CORE_STR_HPP__

#include "rxx/core/slice.hpp"
#include "rxx/core/num/u8.hpp"

namespace rxx {

class str {
  static constexpr size_t str_literal_len(char const* s) {
    return *s ? 1 + str_literal_len(s + 1) : 0;
  }

  slice<u8> m_bytes;

public:
  str(char const* s) : m_bytes{reinterpret_cast<u8*>(const_cast<char*>(s)), usize{str_literal_len(s)}} {}

  auto len() const -> usize {
    return m_bytes.len();
  }

  auto is_empty() const -> Bool {
    return m_bytes.len() == usize{0};
  }

  auto is_char_boundary(usize index) const -> Bool {

  }

  auto as_bytes() -> slice<u8> {
    return m_bytes;
  }

  auto as_bytes_const() const -> slice<const u8> {
    return slice<const u8>{m_bytes.as_const_ptr(), m_bytes.len()};
  }
};

}

#endif /* end of include guard: __RXX_CORE_STR_HPP__ */

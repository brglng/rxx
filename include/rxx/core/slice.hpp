#ifndef __RXX_CORE_SLICE_HPP__
#define __RXX_CORE_SLICE_HPP__

#include <initializer_list>
#include "rxx/core/num/usize.hpp"
#include "rxx/core/option/none.hpp"

namespace rxx {

template<typename T>
class Option;

template<typename T>
Option<T> Some(T&&);

template<typename T>
class slice {
  T*        m_ptr;
  usize     m_len;

public:
  explicit slice(T* ptr, usize len) : m_ptr{ptr}, m_len{len} {}
  explicit slice(std::initializer_list<T> l) : m_ptr{const_cast<T*>(l.begin())}, m_len{l.size()} {}

  auto len() const -> usize {
    return m_len;
  }

  auto is_empty() const -> Bool {
    return m_len == 0;
  }

  auto first() -> Option<T&> {
    if (m_len > 0) {
      return Some(m_ptr[0]);
    } else {
      return None;
    }
  }

  auto first_const() const -> Option<T const&> {
    if (m_len > 0) {
      return Some(m_ptr[0]);
    } else {
      return None;
    }
  }

  // auto split_first() -> Option<tuple<T&, slice<T&>> {

  auto as_ptr() -> T* {
    return m_ptr;
  }

  auto as_const_ptr() const -> T const* {
    return m_ptr;
  }

  // }
};

}

#endif /* end of include guard: __RXX_CORE_SLICE_HPP__ */

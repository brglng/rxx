#ifndef __RXX_CORE_OPTION_PTR_HPP__
#define __RXX_CORE_OPTION_PTR_HPP__

#include "rxx/core/bool.hpp"
#include "rxx/core/str.hpp"
#include "rxx/core/panic.hpp"

namespace rxx {

template<typename T> class Option;

template<typename T>
class Option<T*> {
  T* m_ptr;

public:
  explicit Option(T* ptr) : m_ptr{ptr} {}

  Option(Option&& that) : m_ptr{that.m_ptr} {
    that.m_ptr = nullptr;
  }

  Option(Option const& that) : m_ptr{that.m_ptr} {}

  auto operator=(Option&& that) -> Option& {
    if (this != &that) {
      m_ptr = that.m_ptr;
    }
    return *this;
  }

  auto operator=(Option const& that) -> Option& {
    if (this != &that) {
      m_ptr = that.m_ptr;
    }
    return *this;
  }

  auto is_some() const -> Bool {
    return m_ptr != nullptr;
  }

  auto is_none() const -> Bool {
    return !is_some();
  }

  auto as_ref() -> Option<T*> {
    return Option<T*>{m_ptr};
  }

  auto as_const() const -> Option<T const*> {
    return Option<T const*>{m_ptr};
  }

  auto expect(str msg) -> T* {
    if (is_none()) {
      panic(msg);
    }
    T* ptr = m_ptr;
    m_ptr = nullptr;
    return ptr;
  }

  auto operator==(Option const& that) const -> Bool {
    return m_ptr == that.m_ptr;
  }

  auto operator==(OptionNone const&) const -> Bool {
    return m_ptr == nullptr;
  }
};

}

#endif /* end of include guard: __RXX_CORE_OPTION_PTR_HPP__ */

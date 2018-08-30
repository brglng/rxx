#ifndef __RXX_RESULT_HPP__
#define __RXX_RESULT_HPP__

#include <type_traits>
#include <utility>
#include <algorithm>
#include "rxx/bool.hpp"
#include "rxx/unit.hpp"

namespace rxx {

template<typename T, typename E>
class Result {
  Bool m_is_ok;
  typename std::aligned_storage<std::max(sizeof(T), sizeof(E))>::type m_buffer;

public:
  explicit Result(Bool is_ok, T value) : m_is_ok(is_ok) {
    *reinterpret_cast<T*>(m_buffer) = value;
  }

  explicit Result(Bool is_ok, E err) : m_is_ok(is_ok) {
    *reinterpret_cast<E*>(m_buffer) = err;
  }

  Result(Result&& that) : m_is_ok(that.m_is_ok) {
    if (m_is_ok) {
      *reinterpret_cast<T*>(m_buffer) = *reinterpret_cast<T*>(that.m_buffer);
    } else {
      *reinterpret_cast<E*>(m_buffer) = *reinterpret_cast<E*>(that.m_buffer);
    }
  }

  Result(Result& that) : Result(std::move(that)) {}

  ~Result() {
    if (m_is_ok) {
      reinterpret_cast<T*>(m_buffer)->T::~T();
    } else {
      reinterpret_cast<E*>(m_buffer)->E::~E();
    }
  }

  auto operator=(Result&& that) -> Result& {
    if (that.m_is_ok) {
      *reinterpret_cast<T*>(m_buffer) = *reinterpret_cast<T*>(that.m_buffer);
    } else {
      *reinterpret_cast<E*>(m_buffer) = *reinterpret_cast<E*>(that.m_buffer);
    }
  }

  auto operator==(Result&& that) const -> Bool {
    if (m_is_ok && that.m_is_ok) {
      return *reinterpret_cast<T*>(m_buffer) = *reinterpret_cast<T*>(m_buffer);
    } else if (!m_is_ok && !that.m_is_ok) {
      return *reinterpret_cast<E*>(m_buffer) = *reinterpret_cast<E*>(m_buffer);
    }
  }
};

template<typename T, typename E>
auto Ok(T value) -> Result<T, E> {
  return Result<T, E>{True(), value};
}

template<typename T, typename E>
auto Err(E err) -> Result<T, E> {
  return Result<T, E>{False(), err};
}

}

#endif /* end of include guard: __RXX_RESULT_HPP__ */

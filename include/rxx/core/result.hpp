#ifndef __RXX_RESULT_HPP__
#define __RXX_RESULT_HPP__

#include <type_traits>
#include <utility>
#include <algorithm>
#include "rxx/core/bool.hpp"
#include "rxx/core/unit.hpp"

namespace rxx {

template<typename T, typename E>
class Result {
public:
  enum class Tag {
    MOVED,
    OK,
    ERR,
  };

private:
  Tag m_tag;
  typename std::aligned_storage<std::max(sizeof(T), sizeof(E))>::type m_buffer;

public:
  explicit Result(Tag tag, T&& value) : m_tag(tag) {
    *reinterpret_cast<T*>(m_buffer) = std::move(value);
  }

  explicit Result(Tag tag, T const& value) : m_tag(tag) {
    *reinterpret_cast<T*>(m_buffer) = value;
  }

  explicit Result(Tag tag, E&& err) : m_tag(tag) {
    *reinterpret_cast<E*>(m_buffer) = std::move(err);
  }

  explicit Result(Tag tag, E const& err) : m_tag(tag) {
    *reinterpret_cast<E*>(m_buffer) = err;
  }

  Result(Result&& that) : m_tag(that.m_tag) {
    if (m_tag == Tag::OK) {
      *reinterpret_cast<T*>(m_buffer) = std::move(*reinterpret_cast<T*>(that.m_buffer));
    } else if (m_tag == Tag::ERR) {
      *reinterpret_cast<E*>(m_buffer) = std::move(*reinterpret_cast<E*>(that.m_buffer));
    }
  }

  Result(Result const& that) : m_tag(that.m_tag) {
    if (m_tag == Tag::OK) {
      *reinterpret_cast<T*>(m_buffer) = *reinterpret_cast<T*>(that.m_buffer);
    } else if (m_tag == Tag::ERR) {
      *reinterpret_cast<E*>(m_buffer) = *reinterpret_cast<E*>(that.m_buffer);
    }
  }

  ~Result() {
    if (m_tag == Tag::OK) {
      reinterpret_cast<T*>(m_buffer)->T::~T();
    } else if (m_tag == Tag::ERR) {
      reinterpret_cast<E*>(m_buffer)->E::~E();
    }
  }

  auto operator=(Result&& that) -> Result& {
    if (this != &that) {
      if (m_tag == Tag::OK) {
        reinterpret_cast<T*>(m_buffer)->T::~T();
      } else if (m_tag == Tag::ERR) {
        reinterpret_cast<E*>(m_buffer)->E::~E();
      }

      m_tag == that.m_tag;
      if (that.m_tag == Tag::OK) {
        *reinterpret_cast<T*>(m_buffer) = std::move(*reinterpret_cast<T*>(that.m_buffer));
      } else if (that.m_tag == Tag::ERR) {
        *reinterpret_cast<E*>(m_buffer) = std::move(*reinterpret_cast<E*>(that.m_buffer));
      }

      that.m_tag = Tag::MOVED;
    }
    return *this;
  }

  auto operator=(Result const& that) -> Result& {
    if (this != &that) {
      if (m_tag == Tag::OK) {
        reinterpret_cast<T*>(m_buffer)->T::~T();
      } else if (m_tag == Tag::ERR) {
        reinterpret_cast<E*>(m_buffer)->E::~E();
      }

      m_tag == that.m_tag;
      if (that.m_tag == Tag::OK) {
        *reinterpret_cast<T*>(m_buffer) = *reinterpret_cast<T*>(that.m_buffer);
      } else if (that.m_tag == Tag::ERR) {
        *reinterpret_cast<E*>(m_buffer) = *reinterpret_cast<E*>(that.m_buffer);
      }
    }
    return *this;
  }

  auto operator=(T&& value) -> Result& {
    if (m_tag == Tag::OK) {
      reinterpret_cast<T*>(m_buffer)->T::~T();
    } else if (m_tag == Tag::ERR) {
      reinterpret_cast<E*>(m_buffer)->E::~E();
    }
    m_tag = Tag::OK;
    *reinterpret_cast<T*>(m_buffer) = std::move(value);
    return *this;
  }

  auto operator=(T const& value) -> Result& {
    if (m_tag == Tag::OK) {
      reinterpret_cast<T*>(m_buffer)->T::~T();
    } else if (m_tag == Tag::ERR) {
      reinterpret_cast<E*>(m_buffer)->E::~E();
    }
    m_tag = Tag::OK;
    *reinterpret_cast<T*>(m_buffer) = value;
    return *this;
  }

  auto operator==(Result const& that) const -> Bool {
    if (m_tag == Tag::OK && that.m_tag == Tag::OK) {
      return *reinterpret_cast<T const*>(m_buffer) == *reinterpret_cast<T const*>(m_buffer);
    } else if (m_tag == Tag::ERR && that.m_tag == Tag::ERR) {
      return *reinterpret_cast<E const*>(m_buffer) == *reinterpret_cast<E const*>(m_buffer);
    }
  }
};

template<typename T, typename E>
auto Ok(T&& value) -> Result<T, E> {
  return Result<T, E>{Result<T, E>::Tag::OK, std::forward<T>(value)};
}

template<typename T, typename E>
auto Err(E&& err) -> Result<T, E> {
  return Result<T, E>{Result<T, E>::Tag::ERR, std::forward<T>(err)};
}

}

#endif /* end of include guard: __RXX_RESULT_HPP__ */

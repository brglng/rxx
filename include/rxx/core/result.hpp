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
private:
    Bool m_is_ok;
    typename std::aligned_storage<std::max(sizeof(T), sizeof(E))>::type m_buffer;

public:
    explicit Result(Bool is_ok, T&& value) : m_is_ok(is_ok) {
        *reinterpret_cast<T*>(m_buffer) = value;
    }

    explicit Result(Bool is_ok, T const& value) : m_is_ok(is_ok) {
        *reinterpret_cast<T*>(m_buffer) = value;
    }

    explicit Result(Bool is_ok, E&& err) : m_is_ok(is_ok) {
        *reinterpret_cast<E*>(m_buffer) = err;
    }

    explicit Result(Bool is_ok, E const& err) : m_is_ok(is_ok) {
        *reinterpret_cast<E*>(m_buffer) = err;
    }

    Result(Result&& that) : m_is_ok(that.m_is_ok) {
        if (m_is_ok) {
            *reinterpret_cast<T*>(m_buffer) = *reinterpret_cast<T*>(that.m_buffer);
        } else {
            *reinterpret_cast<E*>(m_buffer) = *reinterpret_cast<E*>(that.m_buffer);
        }
    }

    Result(Result const& that) : m_is_ok(that.m_is_ok) {
        if (m_is_ok) {
            *reinterpret_cast<T*>(m_buffer) = *reinterpret_cast<T*>(that.m_buffer);
        } else {
            *reinterpret_cast<E*>(m_buffer) = *reinterpret_cast<E*>(that.m_buffer);
        }
    }

    ~Result() {
        if (m_is_ok) {
            reinterpret_cast<T*>(m_buffer)->T::~T();
        } else {
            reinterpret_cast<E*>(m_buffer)->E::~E();
        }
    }

    auto operator=(Result&& that) -> Result& {
        if (this != &that) {
          if (m_is_ok) {
              reinterpret_cast<T*>(m_buffer)->T::~T();
          } else {
              reinterpret_cast<E*>(m_buffer)->E::~E();
          }

          m_is_ok = that.m_is_ok;
          if (that.m_is_ok) {
              *reinterpret_cast<T*>(m_buffer) = *reinterpret_cast<T*>(that.m_buffer);
          } else {
              *reinterpret_cast<E*>(m_buffer) = *reinterpret_cast<E*>(that.m_buffer);
          }
        }
        return *this;
    }

    auto operator=(Result const& that) -> Result& {
        if (this != &that) {
            if (m_is_ok) {
                reinterpret_cast<T*>(m_buffer)->T::~T();
            } else {
                reinterpret_cast<E*>(m_buffer)->E::~E();
            }

            m_is_ok = that.m_is_ok;
            if (that.m_is_ok) {
                *reinterpret_cast<T*>(m_buffer) = *reinterpret_cast<T*>(that.m_buffer);
            } else {
                *reinterpret_cast<E*>(m_buffer) = *reinterpret_cast<E*>(that.m_buffer);
            }
        }
        return *this;
    }

    auto operator=(T&& value) -> Result& {
        if (m_is_ok) {
            reinterpret_cast<T*>(m_buffer)->T::~T();
        } else {
            reinterpret_cast<E*>(m_buffer)->E::~E();
        }
        m_is_ok = true;
        *reinterpret_cast<T*>(m_buffer) = value;
        return *this;
    }

    auto operator=(T const& value) -> Result& {
        if (m_is_ok) {
            reinterpret_cast<T*>(m_buffer)->T::~T();
        } else {
            reinterpret_cast<E*>(m_buffer)->E::~E();
        }
        m_is_ok = true;
        *reinterpret_cast<T*>(m_buffer) = value;
        return *this;
    }

    auto operator==(Result const& that) const -> Bool {
        if (m_is_ok && that.m_is_ok) {
            return *reinterpret_cast<T const*>(m_buffer) == *reinterpret_cast<T const*>(m_buffer);
        } else if (!m_is_ok && !that.m_is_ok) {
            return *reinterpret_cast<E const*>(m_buffer) == *reinterpret_cast<E const*>(m_buffer);
        }
    }
};

template<typename T, typename E>
auto Ok(T&& value) -> Result<T, E> {
    return Result<T, E>{true, std::forward<T>(value)};
}

template<typename T, typename E>
auto Err(E&& err) -> Result<T, E> {
    return Result<T, E>{false, std::forward<T>(err)};
}

}

#endif /* end of include guard: __RXX_RESULT_HPP__ */

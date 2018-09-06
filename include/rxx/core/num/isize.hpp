#ifndef __RXX_CORE_NUM_ISIZE_HPP__
#define __RXX_CORE_NUM_ISIZE_HPP__

#include <cstdint>
#include "rxx/core/bool.hpp"

namespace rxx {

class isize {
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
    std::int64_t m_value;

public:
    constexpr isize(std::int64_t value) noexcept : m_value{value} {}

    auto operator=(std::int64_t value) noexcept -> isize& {
        m_value = value;
        return *this;
    }
#else
    int32_t m_value;

public:
    constexpr isize(std::int32_t value) noexcept : m_value{value} {}

    auto operator=(std::int32_t value) noexcept : isize& {
        m_value = value;
        return *this;
    }
#endif

    auto constexpr operator==(isize const& that) const noexcept -> Bool {
        return m_value == that.m_value;
    }

    auto constexpr operator!=(isize const& that) const noexcept -> Bool {
        return m_value != that.m_value;
    }

    auto constexpr operator>(isize const& that) const noexcept -> Bool {
        return m_value > that.m_value;
    }

    auto constexpr operator<(isize const& that) const noexcept -> Bool {
        return m_value < that.m_value;
    }

    auto constexpr operator>=(isize const& that) const noexcept -> Bool {
        return m_value >= that.m_value;
    }

    auto constexpr operator<=(isize const& that) const noexcept -> Bool {
        return m_value <= that.m_value;
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_ISIZE_HPP__ */

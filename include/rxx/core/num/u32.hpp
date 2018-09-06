#ifndef __RXX_CORE_NUM_U32_HPP__
#define __RXX_CORE_NUM_U32_HPP__

#include <utility>
#include <cstdint>
#include "rxx/core/bool.hpp"

namespace rxx {

class u32 {
    std::uint32_t m_value;

public:
    constexpr u32(std::uint32_t value) noexcept : m_value{value} {}

    auto operator=(std::uint32_t value) noexcept -> u32& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(u32 const& rh) const noexcept -> Bool {
        return m_value == rh.m_value;
    }

    auto constexpr operator!=(u32 const& rh) const noexcept -> Bool {
        return m_value != rh.m_value;
    }

    auto constexpr operator>(u32 const& rh) const noexcept -> Bool {
        return m_value > rh.m_value;
    }

    auto constexpr operator<(u32 const& rh) const noexcept -> Bool {
        return m_value < rh.m_value;
    }

    auto constexpr operator>=(u32 const& rh) const noexcept -> Bool {
        return m_value >= rh.m_value;
    }

    auto constexpr operator<=(u32 const& rh) const noexcept -> Bool {
        return m_value <= rh.m_value;
    }
};

}

#endif /* end of include guard: __RXX_U32_HPP__ */

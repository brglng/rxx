#ifndef __RXX_CORE_NUM_U8_HPP__
#define __RXX_CORE_NUM_U8_HPP__

#include <utility>
#include <cstdint>
#include "rxx/core/bool.hpp"

namespace rxx {

class u8 {
    std::uint8_t m_value;

public:
    constexpr u8(std::uint8_t value) noexcept : m_value{value} {}

    auto operator=(std::uint8_t value) noexcept -> u8& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(u8 const& rh) const noexcept -> Bool {
        return m_value == rh.m_value;
    }

    auto constexpr operator!=(u8 const& rh) const noexcept -> Bool {
        return m_value != rh.m_value;
    }

    auto constexpr operator>(u8 const& rh) const noexcept -> Bool {
        return m_value > rh.m_value;
    }

    auto constexpr operator<(u8 const& rh) const noexcept -> Bool {
        return m_value < rh.m_value;
    }

    auto constexpr operator>=(u8 const& rh) const noexcept -> Bool {
        return m_value >= rh.m_value;
    }

    auto constexpr operator<=(u8 const& rh) const noexcept -> Bool {
        return m_value <= rh.m_value;
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_U8_HPP__ */

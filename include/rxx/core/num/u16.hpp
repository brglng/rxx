#ifndef __RXX_CORE_NUM_U16_HPP__
#define __RXX_CORE_NUM_U16_HPP__

#include <utility>
#include <cstdint>
#include "rxx/core/bool.hpp"

namespace rxx {

class u16 {
    std::uint16_t m_value;

public:
    constexpr u16(std::uint16_t value) noexcept : m_value{value} {}

    auto operator=(std::uint16_t value) noexcept -> u16& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(u16 const& rh) const noexcept -> Bool {
        return m_value == rh.m_value;
    }

    auto constexpr operator!=(u16 const& rh) const noexcept -> Bool {
        return m_value != rh.m_value;
    }

    auto constexpr operator>(u16 const& rh) const noexcept -> Bool {
        return m_value > rh.m_value;
    }

    auto constexpr operator<(u16 const& rh) const noexcept -> Bool {
        return m_value < rh.m_value;
    }

    auto constexpr operator>=(u16 const& rh) const noexcept -> Bool {
        return m_value >= rh.m_value;
    }

    auto constexpr operator<=(u16 const& rh) const noexcept -> Bool {
        return m_value <= rh.m_value;
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_U16_HPP__ */

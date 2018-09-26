#ifndef __RXX_CORE_NUM_U8_DEF_HPP__
#define __RXX_CORE_NUM_U8_DEF_HPP__

#include "rxx/core/bool/def.hpp"

namespace rxx {

class u8 {
    unsigned char m_value;

public:
    constexpr u8(unsigned char value) noexcept : m_value{value} {}

    constexpr auto as_uint8_t() const noexcept -> unsigned char {
        return m_value;
    }

    template<typename T>
    T as() const noexcept;

    auto operator=(unsigned char value) noexcept -> u8& {
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

#endif /* end of include guard: __RXX_CORE_NUM_U8_DEF_HPP__ */

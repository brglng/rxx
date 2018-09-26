#ifndef __RXX_CORE_NUM_F32_HPP__
#define __RXX_CORE_NUM_F32_HPP__

#include <cstdint>
#include "rxx/core/as.hpp"
#include "rxx/core/bool/def.hpp"

namespace rxx {

class f32 {
    float m_value;

public:
    constexpr f32(float value) noexcept : m_value{value} {}

    auto operator=(float value) noexcept -> f32& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(f32 const& rh) const noexcept -> Bool {
        return m_value == rh.m_value;
    }

    auto constexpr operator!=(f32 const& rh) const noexcept -> Bool {
        return m_value != rh.m_value;
    }

    auto constexpr operator>(f32 const& rh) const noexcept -> Bool {
        return m_value > rh.m_value;
    }

    auto constexpr operator<(f32 const& rh) const noexcept -> Bool {
        return m_value < rh.m_value;
    }

    auto constexpr operator>=(f32 const& rh) const noexcept -> Bool {
        return m_value >= rh.m_value;
    }

    auto constexpr operator<=(f32 const& rh) const noexcept -> Bool {
        return m_value <= rh.m_value;
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_F32_HPP__ */

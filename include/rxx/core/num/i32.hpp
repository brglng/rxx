#ifndef __RXX_CORE_NUM_I32_HPP__
#define __RXX_CORE_NUM_I32_HPP__

#include <utility>
#include <cstdint>
#include "rxx/core/bool/def.hpp"

namespace rxx {

class i32 {
    std::int32_t m_value;

public:
    constexpr i32(std::int32_t value) noexcept : m_value{value} {}

    auto operator=(std::int32_t value) noexcept -> i32& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(i32 const& rh) const noexcept -> Bool {
        return m_value == rh.m_value;
    }

    auto constexpr operator!=(i32 const& rh) const noexcept -> Bool {
        return m_value != rh.m_value;
    }

    auto constexpr operator>(i32 const& rh) const noexcept -> Bool {
        return m_value > rh.m_value;
    }

    auto constexpr operator<(i32 const& rh) const noexcept -> Bool {
        return m_value < rh.m_value;
    }

    auto constexpr operator>=(i32 const& rh) const noexcept -> Bool {
        return m_value >= rh.m_value;
    }

    auto constexpr operator<=(i32 const& rh) const noexcept -> Bool {
        return m_value <= rh.m_value;
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_I32_HPP__ */

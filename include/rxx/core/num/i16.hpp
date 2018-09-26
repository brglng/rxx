#ifndef __RXX_CORE_NUM_I16_HPP__
#define __RXX_CORE_NUM_I16_HPP__

#include <utility>
#include <cstdint>
#include "rxx/core/bool/def.hpp"

namespace rxx {

class i16 {
    std::int16_t m_value;

public:
    constexpr i16(std::int16_t value) noexcept : m_value{value} {}

    auto operator=(std::int16_t value) noexcept -> i16& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(i16 const& rh) const noexcept -> Bool {
        return m_value == rh.m_value;
    }

    auto constexpr operator!=(i16 const& rh) const noexcept -> Bool {
        return m_value != rh.m_value;
    }

    auto constexpr operator>(i16 const& rh) const noexcept -> Bool {
        return m_value > rh.m_value;
    }

    auto constexpr operator<(i16 const& rh) const noexcept -> Bool {
        return m_value < rh.m_value;
    }

    auto constexpr operator>=(i16 const& rh) const noexcept -> Bool {
        return m_value >= rh.m_value;
    }

    auto constexpr operator<=(i16 const& rh) const noexcept -> Bool {
        return m_value <= rh.m_value;
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_I16_HPP__ */

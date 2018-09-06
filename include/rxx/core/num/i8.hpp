#ifndef __RXX_CORE_NUM_I8_HPP__
#define __RXX_CORE_NUM_I8_HPP__

#include <utility>
#include <cstdint>
#include "rxx/core/bool.hpp"

namespace rxx {

class i8 {
    std::int8_t m_value;
public:
    constexpr i8(std::int8_t value) noexcept : m_value{value} {}

    auto operator=(std::int8_t value) noexcept -> i8& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(i8 const& rh) const noexcept -> Bool {
        return m_value == rh.m_value;
    }

    auto constexpr operator!=(i8 const& rh) const noexcept -> Bool {
        return m_value != rh.m_value;
    }

    auto constexpr operator>(i8 const& rh) const noexcept -> Bool {
        return m_value > rh.m_value;
    }

    auto constexpr operator<(i8 const& rh) const noexcept -> Bool {
        return m_value < rh.m_value;
    }

    auto constexpr operator>=(i8 const& rh) const noexcept -> Bool {
        return m_value >= rh.m_value;
    }

    auto constexpr operator<=(i8 const& rh) const noexcept -> Bool {
      return m_value <= rh.m_value;
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_I8_HPP__ */

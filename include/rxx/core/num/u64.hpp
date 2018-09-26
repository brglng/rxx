#ifndef __RXX_CORE_NUM_U64_HPP__
#define __RXX_CORE_NUM_U64_HPP__

#include <utility>
#include <cstdint>
#include "rxx/core/bool/def.hpp"

namespace rxx {

class u64 {
    std::uint64_t m_value;

public:
    constexpr u64(std::uint64_t value) noexcept : m_value{value} {}

    auto operator=(std::uint64_t value) noexcept -> u64& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(u64 const& rh) const noexcept -> Bool {
        return m_value == rh.m_value;
    }

    auto constexpr operator!=(u64 const& rh) const noexcept -> Bool {
        return m_value != rh.m_value;
    }

    auto constexpr operator>(u64 const& rh) const noexcept -> Bool {
        return m_value > rh.m_value;
    }

    auto constexpr operator<(u64 const& rh) const noexcept -> Bool {
        return m_value < rh.m_value;
    }

    auto constexpr operator>=(u64 const& rh) const noexcept -> Bool {
        return m_value >= rh.m_value;
    }

    auto constexpr operator<=(u64 const& rh) const noexcept -> Bool {
        return m_value <= rh.m_value;
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_U64_HPP__ */

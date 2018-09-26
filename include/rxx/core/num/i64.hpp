#ifndef __RXX_CORE_NUM_I64_HPP__
#define __RXX_CORE_NUM_I64_HPP__

#include <utility>
#include <cstdint>
#include "rxx/core/bool/def.hpp"

namespace rxx {

class i64 {
    std::int64_t m_value;

public:
    constexpr i64(std::int64_t value) noexcept : m_value{value} {}

    auto operator=(std::int64_t value) noexcept -> i64& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(i64 const& rh) const noexcept -> Bool {
        return m_value == rh.m_value;
    }

    auto constexpr operator!=(i64 const& rh) const noexcept -> Bool {
        return m_value != rh.m_value;
    }

    auto constexpr operator>(i64 const& rh) const noexcept -> Bool {
        return m_value > rh.m_value;
    }

    auto constexpr operator<(i64 const& rh) const noexcept -> Bool {
        return m_value < rh.m_value;
    }

    auto constexpr operator>=(i64 const& rh) const noexcept -> Bool {
        return m_value >= rh.m_value;
    }

    auto constexpr operator<=(i64 const& rh) const noexcept -> Bool {
        return m_value <= rh.m_value;
    }
};

}

#endif /* end of include guard: __RXX_I64_HPP__ */

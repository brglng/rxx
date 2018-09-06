#ifndef __RXX_CORE_NUM_F64_HPP__
#define __RXX_CORE_NUM_F64_HPP__

#include <utility>
#include <cstdint>
#include "rxx/core/bool.hpp"

namespace rxx {

class f64 {
    double m_value;

public:
    constexpr f64(double value) noexcept : m_value{value} {}

    auto operator=(double value) noexcept -> f64& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(f64 const& rh) const noexcept -> Bool {
        return m_value == rh.m_value;
    }

    auto operator!=(f64 const& rh) const -> Bool {
        return m_value != rh.m_value;
    }

    auto operator>(f64 const& rh) const -> Bool {
        return m_value > rh.m_value;
    }

    auto operator<(f64 const& rh) const -> Bool {
        return m_value < rh.m_value;
    }

    auto operator>=(f64 const& rh) const -> Bool {
        return m_value >= rh.m_value;
    }

    auto operator<=(f64 const& rh) const -> Bool {
        return m_value <= rh.m_value;
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_F64_HPP__ */

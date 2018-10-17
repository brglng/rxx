#ifndef __RXX_CORE_NUM_I32_IMPL_HPP__
#define __RXX_CORE_NUM_I32_IMPL_HPP__

#include "rxx/core/bool/def.hpp"
#include "rxx/core/num/i32/def.hpp"

namespace rxx {

auto constexpr i32::operator==(i32 rh) const noexcept -> Bool {
    return m_value == rh.m_value;
}

auto constexpr i32::operator!=(i32 rh) const noexcept -> Bool {
    return m_value != rh.m_value;
}

auto constexpr i32::operator>(i32 rh) const noexcept -> Bool {
    return m_value > rh.m_value;
}

auto constexpr i32::operator<(i32 rh) const noexcept -> Bool {
    return m_value < rh.m_value;
}

auto constexpr i32::operator>=(i32 rh) const noexcept -> Bool {
    return m_value >= rh.m_value;
}

auto constexpr i32::operator<=(i32 rh) const noexcept -> Bool {
    return m_value <= rh.m_value;
}

}

#endif /* end of include guard: __RXX_CORE_NUM_I32_IMPL_HPP__ */

#ifndef __RXX_I32_IMPL_HPP__
#define __RXX_I32_IMPL_HPP__

#include "rxx/bool/def.hpp"
#include "rxx/i32/def.hpp"

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

#endif /* end of include guard: __RXX_I32_IMPL_HPP__ */

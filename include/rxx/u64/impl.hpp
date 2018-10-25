#ifndef __RXX_U64_IMPL_HPP__
#define __RXX_U64_IMPL_HPP__

#include <limits>
#include "rxx/bool/def.hpp"
#include "rxx/u64/def.hpp"

namespace rxx {

constexpr u64 u64::MAX = std::numeric_limits<std::uint64_t>::max();

auto constexpr u64::operator==(u64 rh) const noexcept -> Bool {
    return m_value == rh.m_value;
}

auto constexpr u64::operator!=(u64 rh) const noexcept -> Bool {
    return m_value != rh.m_value;
}

auto constexpr u64::operator>(u64 rh) const noexcept -> Bool {
    return m_value > rh.m_value;
}

auto constexpr u64::operator<(u64 rh) const noexcept -> Bool {
    return m_value < rh.m_value;
}

auto constexpr u64::operator>=(u64 rh) const noexcept -> Bool {
    return m_value >= rh.m_value;
}

auto constexpr u64::operator<=(u64 rh) const noexcept -> Bool {
    return m_value <= rh.m_value;
}

}

#endif /* end of include guard: __RXX_U64_IMPL_HPP__ */

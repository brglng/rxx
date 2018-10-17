#ifndef __RXX_CORE_NUM_U8_IMPL_HPP__
#define __RXX_CORE_NUM_U8_IMPL_HPP__

#include <limits>
#include "rxx/core/as/def.hpp"
#include "rxx/core/bool/def.hpp"
#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/usize/def.hpp"

namespace rxx {

constexpr u8 u8::MAX = std::numeric_limits<std::uint8_t>::max();

auto constexpr u8::operator==(u8 rh) const noexcept -> Bool {
    return m_value == rh.m_value;
}

auto constexpr u8::operator!=(u8 rh) const noexcept -> Bool {
    return m_value != rh.m_value;
}

auto constexpr u8::operator>(u8 rh) const noexcept -> Bool {
    return m_value > rh.m_value;
}

auto constexpr u8::operator<(u8 rh) const noexcept -> Bool {
    return m_value < rh.m_value;
}

auto constexpr u8::operator>=(u8 rh) const noexcept -> Bool {
    return m_value >= rh.m_value;
}

auto constexpr u8::operator<=(u8 rh) const noexcept -> Bool {
    return m_value <= rh.m_value;
}

template<typename T>
T u8::as() const noexcept {
    return ::rxx::as<T>(*this);
}

auto As<usize, u8*>::as(u8* from) noexcept -> usize {
    return reinterpret_cast<std::size_t>(from);
}

auto constexpr As<usize, u8>::as(u8 from) noexcept -> usize {
    return from.as_uint8_t();
}

inline auto operator+(u8* lh, usize rh) noexcept -> u8* {
    return reinterpret_cast<u8*>(reinterpret_cast<unsigned char*>(lh) + rh.as_size_t());
}

inline auto operator-(u8* lh, usize rh) noexcept -> u8* {
    return reinterpret_cast<u8*>(reinterpret_cast<unsigned char*>(lh) - rh.as_size_t());
}

inline auto operator-(u8* lh, u8 rh) noexcept -> u8* {
    return reinterpret_cast<u8*>(reinterpret_cast<unsigned char*>(lh) - rh.as_uint8_t());
}

};

#endif /* end of include guard: __RXX_CORE_NUM_U8_IMPL_HPP__ */

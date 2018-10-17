#ifndef __RXX_CORE_NUM_USIZE_AS_SIZE_T_HPP__
#define __RXX_CORE_NUM_USIZE_AS_SIZE_T_HPP__

#include "rxx/core/as/def.hpp"
#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/usize/def.hpp"
#include "rxx/core/option/def.hpp"

namespace rxx {

constexpr usize::usize(u8 value) noexcept : m_value{value.as_uint8_t()} {}

template<typename T>
inline T usize::as() const noexcept {
    return ::rxx::as<T>(*this);
}

auto constexpr usize::operator==(usize that) const noexcept -> Bool {
    return m_value == that.m_value;
}

auto constexpr usize::operator==(std::size_t that) const noexcept -> Bool {
    return m_value == that;
}

auto constexpr usize::operator!=(usize that) const noexcept -> Bool {
    return m_value != that.m_value;
}

auto constexpr usize::operator!=(std::size_t that) const noexcept -> Bool {
    return m_value != that;
}

auto constexpr usize::operator>(usize that) const noexcept -> Bool {
    return m_value > that.m_value;
}

auto constexpr usize::operator>(std::size_t that) const noexcept -> Bool {
    return m_value > that;
}

auto constexpr usize::operator<(usize that) const noexcept -> Bool {
    return m_value < that.m_value;
}

auto constexpr usize::operator<(std::size_t that) const noexcept -> Bool {
    return m_value < that;
}

auto constexpr usize::operator>=(usize that) const noexcept -> Bool {
    return m_value >= that.m_value;
}

auto constexpr usize::operator>=(std::size_t that) const noexcept -> Bool {
    return m_value >= that;
}

auto constexpr usize::operator<=(usize that) const noexcept -> Bool {
    return m_value <= that.m_value;
}

auto constexpr usize::operator<=(std::size_t that) const noexcept -> Bool {
    return m_value <= that;
}

auto constexpr usize::is_power_of_two() const noexcept -> Bool {
    return m_value && !(m_value & (m_value - 1));
}

inline auto usize::overflowing_add(usize rhs) const noexcept -> std::pair<usize, Bool> {
    std::size_t result = m_value + rhs.m_value;
    if (result < m_value || result < rhs.m_value) {
        return std::make_pair(usize { result }, Bool { true });
    } else {
        return std::make_pair(usize { result }, Bool { false });
    }
}

inline auto usize::overflowing_sub(usize rhs) const noexcept -> std::pair<usize, Bool> {
    std::size_t result = m_value - rhs.m_value;
    if (m_value < rhs.m_value) {
        return std::make_pair(usize { result }, Bool { true });
    } else {
        return std::make_pair(usize { result }, Bool { false });
    }
}

inline auto usize::checked_add(usize rhs) const noexcept -> Option<usize> {
    auto result = overflowing_add(rhs);
    if (result.second) {
        return None;
    } else {
        return Some(std::move(result.first));
    }
}

inline auto usize::checked_sub(usize rhs) const noexcept -> Option<usize> {
    auto result = overflowing_sub(rhs);
    if (result.second) {
        return None;
    } else {
        return Some(std::move(result.first));
    }
}

auto constexpr As<u8, usize>::as(usize u) noexcept -> u8 {
    return u.as_size_t();
}

auto constexpr As<std::size_t, usize>::as(usize from) noexcept -> std::size_t {
    return from.as_size_t();
}

};

#endif /* end of include guard: __RXX_CORE_NUM_USIZE_AS_SIZE_T_HPP__ */

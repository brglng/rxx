#ifndef __RXX_USIZE_IMPL_HPP__
#define __RXX_USIZE_IMPL_HPP__

#include <limits>
#include "rxx/as/def.hpp"
#include "rxx/u8/def.hpp"
#include "rxx/usize/def.hpp"
#include "rxx/u64/def.hpp"
#include "rxx/option/def.hpp"

namespace rxx {

constexpr usize::usize(u8 i) noexcept : m_value{i.as_uint8_t()} {}

constexpr usize::usize(u64 i) noexcept : m_value { i.as_uint64_t() } {}

const usize usize::MAX = std::numeric_limits<std::size_t>::max();

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

auto constexpr operator==(std::size_t const& lh, usize const& rh) noexcept -> Bool {
    return lh == rh.as_size_t();
}

auto constexpr operator!=(std::size_t const& lh, usize const& rh) noexcept -> Bool {
    return lh != rh.as_size_t();
}

auto constexpr operator<(std::size_t const& lh, usize const& rh) noexcept -> Bool {
    return lh < rh.as_size_t();
}

auto constexpr operator>(std::size_t const& lh, usize const& rh) noexcept -> Bool {
    return lh > rh.as_size_t();
}

auto constexpr operator<=(std::size_t const& lh, usize const& rh) noexcept -> Bool {
    return lh <= rh.as_size_t();
}

auto constexpr operator>=(std::size_t const& lh, usize const& rh) noexcept -> Bool {
    return lh >= rh.as_size_t();
}

auto constexpr As<u8, usize>::as(usize u) noexcept -> u8 {
    return u.as_size_t();
}

auto constexpr As<std::size_t, usize>::as(usize from) noexcept -> std::size_t {
    return from.as_size_t();
}

};

#endif /* end of include guard: __RXX_USIZE_IMPL_HPP__ */

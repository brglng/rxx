#ifndef __RXX_CORE_NUM_USIZE_HPP__
#define __RXX_CORE_NUM_USIZE_HPP__

#include <cstdlib>
#include "rxx/core/bool.hpp"

namespace rxx {

class usize {
    std::size_t m_value;

public:
    constexpr usize(std::size_t value) noexcept : m_value{value} {}

    auto constexpr as_size_t() const noexcept -> std::size_t {
        return m_value;
    }

    auto constexpr operator==(usize const& that) const noexcept -> Bool {
        return m_value == that.m_value;
    }

    auto constexpr operator!=(usize const& that) const noexcept -> Bool {
        return m_value != that.m_value;
    }

    auto constexpr operator>(usize const& that) const noexcept -> Bool {
        return m_value > that.m_value;
    }

    auto constexpr operator<(usize const& that) const noexcept -> Bool {
        return m_value < that.m_value;
    }

    auto constexpr operator>=(usize const& that) const noexcept -> Bool {
        return m_value >= that.m_value;
    }

    auto constexpr operator<=(usize const& that) const noexcept -> Bool {
        return m_value <= that.m_value;
    }
};

template<typename T, typename U> struct As;

template<>
struct As<std::size_t, usize> {
    static constexpr auto as(usize u) noexcept -> std::size_t {
        return u.as_size_t();
    }
};

template<>
struct As<std::size_t*, usize*> {
    static auto as(usize* u) noexcept -> std::size_t* {
        return reinterpret_cast<std::size_t*>(u);
    }
};

template<>
struct As<std::size_t const*, usize const*> {
    static auto as(usize const* u) noexcept -> std::size_t const* {
        return reinterpret_cast<std::size_t const*>(u);
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_USIZE_HPP__ */

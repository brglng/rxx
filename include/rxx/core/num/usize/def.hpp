#ifndef __RXX_CORE_NUM_USIZE_DEF_HPP__
#define __RXX_CORE_NUM_USIZE_DEF_HPP__

#include <cstdlib>
#include "rxx/core/bool/def.hpp"

namespace rxx {

class usize {
    std::size_t m_value;

public:
    constexpr usize(std::size_t value) noexcept : m_value{value} {}

    template<typename T>
    T as() const noexcept;

    auto constexpr as_size_t() const noexcept -> std::size_t {
        return m_value;
    }

    auto constexpr operator==(usize const& that) const noexcept -> Bool {
        return m_value == that.m_value;
    }

    auto constexpr operator==(std::size_t const& that) const noexcept -> Bool {
        return m_value == that;
    }

    auto constexpr operator!=(usize const& that) const noexcept -> Bool {
        return m_value != that.m_value;
    }

    auto constexpr operator!=(std::size_t const& that) const noexcept -> Bool {
        return m_value != that;
    }

    auto constexpr operator>(usize const& that) const noexcept -> Bool {
        return m_value > that.m_value;
    }

    auto constexpr operator>(std::size_t const& that) const noexcept -> Bool {
        return m_value > that;
    }

    auto constexpr operator<(usize const& that) const noexcept -> Bool {
        return m_value < that.m_value;
    }

    auto constexpr operator<(std::size_t const& that) const noexcept -> Bool {
        return m_value < that;
    }

    auto constexpr operator>=(usize const& that) const noexcept -> Bool {
        return m_value >= that.m_value;
    }

    auto constexpr operator>=(std::size_t const& that) const noexcept -> Bool {
        return m_value >= that;
    }

    auto constexpr operator<=(usize const& that) const noexcept -> Bool {
        return m_value <= that.m_value;
    }

    auto constexpr operator<=(std::size_t const& that) const noexcept -> Bool {
        return m_value <= that;
    }

    auto constexpr operator+(usize const& that) const noexcept -> usize {
        return m_value + that.m_value;
    }

    auto constexpr operator+(std::size_t const& that) const noexcept -> usize {
        return m_value + that;
    }

    auto constexpr operator-(usize const& that) const noexcept -> usize {
        return m_value - that.m_value;
    }

    auto constexpr operator-(std::size_t const& that) const noexcept -> usize {
        return m_value - that;
    }

    auto constexpr operator*(usize const& that) const noexcept -> usize {
        return m_value * that.m_value;
    }

    auto constexpr operator*(std::size_t const& that) const noexcept -> usize {
        return m_value * that;
    }

    auto constexpr operator/(usize const& that) const noexcept -> usize {
        return m_value / that.m_value;
    }

    auto constexpr operator/(std::size_t const& that) const noexcept -> usize {
        return m_value / that;
    }

    auto constexpr operator%(usize const& that) const noexcept -> usize {
        return m_value % that.m_value;
    }

    auto constexpr operator%(std::size_t that) const noexcept -> usize {
        return m_value % that;
    }

    auto constexpr operator&(usize const& that) const noexcept -> usize {
        return m_value & that.m_value;
    }

    auto constexpr operator&(std::size_t that) const noexcept -> usize {
        return m_value & that;
    }

    auto constexpr operator|(usize const& that) const noexcept -> usize {
        return m_value | that.m_value;
    }

    auto constexpr operator|(std::size_t that) const noexcept -> usize {
        return m_value | that;
    }

    auto constexpr operator~() const noexcept -> usize {
        return ~m_value;
    }

    auto constexpr operator^(usize const& that) const noexcept -> usize {
        return m_value ^ that.m_value;
    }

    auto constexpr operator^(std::size_t that) const noexcept -> usize {
        return m_value ^ that;
    }

    auto constexpr operator<<(usize const& that) const noexcept -> usize {
        return m_value << that.m_value;
    }

    auto constexpr operator<<(std::size_t const& that) const noexcept -> usize {
        return m_value << that;
    }

    auto constexpr operator>>(usize const& that) const noexcept -> usize {
        return m_value >> that.m_value;
    }

    auto constexpr operator>>(std::size_t const& that) const noexcept -> usize {
        return m_value >> that;
    }

    auto operator<<=(usize const& that) noexcept -> usize& {
        m_value <<= that.m_value;
        return *this;
    }

    auto operator<<=(std::size_t const& that) noexcept -> usize& {
        m_value <<= that;
        return *this;
    }

    auto operator>>=(usize const& that) noexcept -> usize& {
        m_value >>= that.m_value;
        return *this;
    }

    auto operator>>=(std::size_t const& that) noexcept -> usize& {
        m_value >>= that;
        return *this;
    }

    auto constexpr is_power_of_two() const noexcept -> Bool {
        return m_value && !(m_value & (m_value - 1));
    }
};

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

auto constexpr operator+(std::size_t lh, usize const& rh) noexcept -> usize {
    return lh + rh.as_size_t();
}

auto constexpr operator-(std::size_t lh, usize const& rh) noexcept -> usize {
    return lh - rh.as_size_t();
}

auto constexpr operator*(std::size_t lh, usize const& rh) noexcept -> usize {
    return lh * rh.as_size_t();
}

auto constexpr operator/(std::size_t lh, usize const& rh) noexcept -> usize {
    return lh / rh.as_size_t();
}

auto constexpr operator%(std::size_t lh, usize const& rh) noexcept -> usize {
    return lh % rh.as_size_t();
}

auto constexpr operator&(std::size_t lh, usize const& rh) noexcept -> usize {
    return lh & rh.as_size_t();
}

auto constexpr operator|(std::size_t lh, usize const& rh) noexcept -> usize {
    return lh | rh.as_size_t();
}

auto constexpr operator^(std::size_t lh, usize const& rh) noexcept -> usize {
    return lh ^ rh.as_size_t();
}

auto constexpr operator<<(std::size_t const& lh, usize const& rh) noexcept -> usize {
    return lh << rh.as_size_t();
}

auto constexpr operator>>(std::size_t const& lh, usize const& rh) noexcept -> usize {
    return lh >> rh.as_size_t();
}

}

#endif /* end of include guard: __RXX_CORE_NUM_USIZE_DEF_HPP__ */

#ifndef __RXX_CORE_NUM_USIZE_DEF_HPP__
#define __RXX_CORE_NUM_USIZE_DEF_HPP__

#include <cstdlib>
#include <tuple>

namespace rxx {

template<typename T> class Option;

class Bool;
class u8;

template<typename T, typename U> struct As;

class usize {
    std::size_t m_value;

public:
    constexpr usize(std::size_t value) noexcept : m_value{value} {}
    constexpr usize(u8 value) noexcept;

    static const usize MAX;

    template<typename T>
    T as() const noexcept;

    auto constexpr as_size_t() const noexcept -> std::size_t {
        return m_value;
    }

    auto constexpr operator==(usize that) const noexcept -> Bool;
    auto constexpr operator==(std::size_t that) const noexcept -> Bool;
    auto constexpr operator!=(usize that) const noexcept -> Bool;
    auto constexpr operator!=(std::size_t that) const noexcept -> Bool;
    auto constexpr operator>(usize that) const noexcept -> Bool;
    auto constexpr operator>(std::size_t that) const noexcept -> Bool;
    auto constexpr operator<(usize that) const noexcept -> Bool;
    auto constexpr operator<(std::size_t that) const noexcept -> Bool;
    auto constexpr operator>=(usize that) const noexcept -> Bool;
    auto constexpr operator>=(std::size_t that) const noexcept -> Bool;
    auto constexpr operator<=(usize that) const noexcept -> Bool;
    auto constexpr operator<=(std::size_t that) const noexcept -> Bool;

    auto constexpr operator+(usize that) const noexcept -> usize {
        return m_value + that.m_value;
    }

    auto constexpr operator+(std::size_t that) const noexcept -> usize {
        return m_value + that;
    }

    auto constexpr operator-(usize that) const noexcept -> usize {
        return m_value - that.m_value;
    }

    auto constexpr operator-(std::size_t that) const noexcept -> usize {
        return m_value - that;
    }

    auto constexpr operator*(usize that) const noexcept -> usize {
        return m_value * that.m_value;
    }

    auto constexpr operator*(std::size_t that) const noexcept -> usize {
        return m_value * that;
    }

    auto constexpr operator/(usize that) const noexcept -> usize {
        return m_value / that.m_value;
    }

    auto constexpr operator/(std::size_t that) const noexcept -> usize {
        return m_value / that;
    }

    auto constexpr operator%(usize that) const noexcept -> usize {
        return m_value % that.m_value;
    }

    auto constexpr operator%(std::size_t that) const noexcept -> usize {
        return m_value % that;
    }

    auto constexpr operator&(usize that) const noexcept -> usize {
        return m_value & that.m_value;
    }

    auto constexpr operator&(std::size_t that) const noexcept -> usize {
        return m_value & that;
    }

    auto constexpr operator|(usize that) const noexcept -> usize {
        return m_value | that.m_value;
    }

    auto constexpr operator|(std::size_t that) const noexcept -> usize {
        return m_value | that;
    }

    auto constexpr operator~() const noexcept -> usize {
        return ~m_value;
    }

    auto constexpr operator^(usize that) const noexcept -> usize {
        return m_value ^ that.m_value;
    }

    auto constexpr operator^(std::size_t that) const noexcept -> usize {
        return m_value ^ that;
    }

    auto constexpr operator<<(usize that) const noexcept -> usize {
        return m_value << that.m_value;
    }

    auto constexpr operator<<(std::size_t that) const noexcept -> usize {
        return m_value << that;
    }

    auto constexpr operator>>(usize that) const noexcept -> usize {
        return m_value >> that.m_value;
    }

    auto constexpr operator>>(std::size_t that) const noexcept -> usize {
        return m_value >> that;
    }

    auto operator<<=(usize that) noexcept -> usize& {
        m_value <<= that.m_value;
        return *this;
    }

    auto operator<<=(std::size_t that) noexcept -> usize& {
        m_value <<= that;
        return *this;
    }

    auto operator>>=(usize that) noexcept -> usize& {
        m_value >>= that.m_value;
        return *this;
    }

    auto operator>>=(std::size_t that) noexcept -> usize& {
        m_value >>= that;
        return *this;
    }

    auto constexpr is_power_of_two() const noexcept -> Bool;

    auto next_power_of_two() const noexcept -> usize {
        auto v = m_value;
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        if (sizeof(v) == 8) {
            v |= v >> 32;
        }
        v++;
        return v;
    }

    auto constexpr wrapping_add(usize rhs) const noexcept -> usize {
        return m_value + rhs.m_value;
    }

    auto constexpr wrapping_sub(usize rhs) const noexcept -> usize {
        return m_value - rhs.m_value;
    }

    auto overflowing_add(usize rhs) const noexcept -> std::pair<usize, Bool>;
    auto overflowing_sub(usize rhs) const noexcept -> std::pair<usize, Bool>;
    auto checked_add(usize rhs) const noexcept -> Option<usize>;
    auto checked_sub(usize rhs) const noexcept -> Option<usize>;
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

template<>
struct As<std::size_t, usize> {
    static constexpr auto as(usize u) noexcept -> std::size_t;
};

template<>
struct As<u8, usize> {
    static constexpr auto as(usize u) noexcept -> u8;
};

}

#endif /* end of include guard: __RXX_CORE_NUM_USIZE_DEF_HPP__ */

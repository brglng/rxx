#ifndef __RXX_U64_DEF_HPP__
#define __RXX_U64_DEF_HPP__

#include <utility>
#include <cstdint>

namespace rxx {

class Bool;

class u64 {
    std::uint64_t m_value;

public:
    static const u64 MAX;

    constexpr u64(std::uint64_t value) noexcept : m_value{value} {}

    auto operator=(std::uint64_t value) noexcept -> u64& {
        m_value = value;
        return *this;
    }

    constexpr auto as_uint64_t() const noexcept -> std::uint64_t { return m_value; }

    auto constexpr operator==(u64 rh) const noexcept -> Bool;
    auto constexpr operator!=(u64 rh) const noexcept -> Bool;
    auto constexpr operator>(u64 rh) const noexcept -> Bool;
    auto constexpr operator<(u64 rh) const noexcept -> Bool;
    auto constexpr operator>=(u64 rh) const noexcept -> Bool;
    auto constexpr operator<=(u64 rh) const noexcept -> Bool;
};

}

#endif /* end of include guard: __RXX_U64_DEF_HPP__ */

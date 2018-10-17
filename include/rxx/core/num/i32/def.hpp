#ifndef __RXX_CORE_NUM_I32_HPP__
#define __RXX_CORE_NUM_I32_HPP__

#include <utility>
#include <cstdint>

namespace rxx {

class Bool;

class i32 {
    std::int32_t m_value;

public:
    constexpr i32(std::int32_t value) noexcept : m_value{value} {}

    auto operator=(std::int32_t value) noexcept -> i32& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(i32 rh) const noexcept -> Bool;
    auto constexpr operator!=(i32 rh) const noexcept -> Bool;
    auto constexpr operator>(i32 rh) const noexcept -> Bool;
    auto constexpr operator<(i32 rh) const noexcept -> Bool;
    auto constexpr operator>=(i32 rh) const noexcept -> Bool;
    auto constexpr operator<=(i32 rh) const noexcept -> Bool;

    template<typename H>
    void hash(H& hasher) const {
        hasher.write_i32(*this);
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_I32_HPP__ */

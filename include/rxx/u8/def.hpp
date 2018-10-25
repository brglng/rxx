#ifndef __RXX_U8_DEF_HPP__
#define __RXX_U8_DEF_HPP__

namespace rxx {

template<typename T, typename U> struct As;

class Bool;
class usize;

class u8 {
    unsigned char m_value;

public:
    static const u8 MAX;

    constexpr u8(unsigned char value) noexcept : m_value{value} {}

    constexpr auto as_uint8_t() const noexcept -> unsigned char {
        return m_value;
    }

    template<typename T>
    T as() const noexcept;

    auto operator=(unsigned char value) noexcept -> u8& {
        m_value = value;
        return *this;
    }

    auto constexpr operator==(u8 rh) const noexcept -> Bool;
    auto constexpr operator!=(u8 rh) const noexcept -> Bool;
    auto constexpr operator>(u8 rh) const noexcept -> Bool;
    auto constexpr operator<(u8 rh) const noexcept -> Bool;
    auto constexpr operator>=(u8 rh) const noexcept -> Bool;
    auto constexpr operator<=(u8 rh) const noexcept -> Bool;
};

inline auto operator+(u8* lh, usize rh) noexcept -> u8*;
inline auto operator-(u8* lh, usize rh) noexcept -> u8*;
inline auto operator-(u8* lh, usize rh) noexcept -> u8*;
inline auto operator-(u8* lh, u8 rh) noexcept -> u8*;

template<>
struct As<usize, u8*> {
    static auto as(u8* from) noexcept -> usize;
};

template<>
struct As<usize, u8> {
    static constexpr auto as(u8 from) noexcept -> usize;
};

}

#endif /* end of include guard: __RXX_U8_DEF_HPP__ */

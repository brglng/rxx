#ifndef __RXX_STR_HPP__
#define __RXX_STR_HPP__

#include <cstdint>
#include <cstdlib>
#include <string>
#include "rxx/slice.hpp"

namespace rxx {

class MutStr {
    Slice<uint8_t> m_bytes;

public:
    template<std::size_t N>
    constexpr MutStr(char (&s)[N]) noexcept : m_bytes{static_cast<unsigned char (&)[N]>(s)} {}

    explicit constexpr MutStr(char *s, size_t len) noexcept : m_bytes{(uint8_t*)s, len} {}

    constexpr auto len() const noexcept -> size_t {
        return m_bytes.len();
    }

    constexpr auto is_empty() const noexcept -> bool {
        return m_bytes.is_empty();
    }

    // auto is_char_boundary(size_t index) const -> Bool {

    // }

    constexpr const char* c_str() const {
        return (const char*)m_bytes.as_ptr();
    }

    auto as_bytes() noexcept -> Slice<uint8_t> {
        return m_bytes;
    }

    constexpr auto as_bytes() const noexcept -> Slice<uint8_t> {
        return m_bytes;
    }

    uint8_t& operator[](size_t i) { return m_bytes[i]; }

#ifdef NDEBUG
    constexpr uint8_t const& operator[](size_t i) const { return m_bytes[i]; }
#else
    uint8_t const& operator[](size_t i) const { return m_bytes[i]; }
#endif
};


class Str {
    Slice<const uint8_t> m_bytes;

public:
    template<std::size_t N>
    constexpr Str(const char (&s)[N]) noexcept :
        m_bytes { static_cast<const unsigned char (&)[N]>(s) }
    {}

    template<std::size_t N>
    constexpr Str(char (&s)[N]) noexcept :
        m_bytes { static_cast<const unsigned char (&)[N]>(s) }
    {}

    explicit constexpr Str(const char* s, size_t len) noexcept : m_bytes{(uint8_t const*)s, len} {}

    constexpr Str(const MutStr s) noexcept : m_bytes{s.as_bytes().as_ptr(), s.len()} {}

    constexpr auto len() const -> size_t {
        return m_bytes.len();
    }

    constexpr auto is_empty() const -> bool {
        return m_bytes.is_empty();
    }

    constexpr const char* c_str() const {
        return (const char*)m_bytes.as_ptr();
    }

    constexpr auto as_bytes() const -> Slice<const uint8_t> {
        return m_bytes;
    }

    uint8_t const& operator[](size_t i) { return m_bytes[i]; }

#ifdef NDEBUG
    constexpr uint8_t const& operator[](size_t i) const { return m_bytes[i]; }
#else
    uint8_t const& operator[](size_t i) const { return m_bytes[i]; }
#endif
};

template<std::size_t N>
constexpr auto str(const char (&s)[N]) -> Str {
    return Str{s};
}

template<std::size_t N>
constexpr auto str(char (&s)[N]) -> Str {
    return MutStr{s};
}

Str str(std::string const& s) {
    return Str{&s[0], s.size()};
}

MutStr str(std::string&& s) {
    return MutStr{&s[0], s.size()};
}

}

#endif /* end of include guard: __RXX_STR_HPP__ */

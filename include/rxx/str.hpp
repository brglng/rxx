#ifndef __RXX_STR_HPP__
#define __RXX_STR_HPP__

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include "rxx/slice.hpp"

namespace rxx {

class Str;

class MutStr {
    Slice<uint8_t> m_bytes;

public:
    template<std::size_t N>
    constexpr MutStr(char (&s)[N]) noexcept : m_bytes((std::uint8_t*)s, s[N - 1] == 0 ? N - 1 : N) {}

    MutStr(std::string& s) noexcept : MutStr(&s[0], s.size()) {}

    explicit MutStr(char *s, size_t len) noexcept : m_bytes((uint8_t*)s, len) {}

    constexpr auto len() const noexcept -> size_t {
        return m_bytes.len();
    }

    constexpr auto is_empty() const noexcept -> bool {
        return m_bytes.is_empty();
    }

    // auto is_char_boundary(size_t index) const -> Bool {

    // }

    const char* c_str() const {
        return (const char*)m_bytes.as_ptr();
    }

    auto as_bytes() noexcept -> Slice<uint8_t> {
        return m_bytes;
    }

    constexpr auto as_bytes() const noexcept -> Slice<const uint8_t> {
        return m_bytes;
    }

    uint8_t& operator[](size_t i) { return m_bytes[i]; }

    constexpr bool operator==(MutStr const& rhs) const {
        return m_bytes == rhs.m_bytes;
    }

#ifdef NDEBUG
    constexpr uint8_t const& operator[](size_t i) const { return m_bytes[i]; }
#else
    uint8_t const& operator[](size_t i) const { return m_bytes[i]; }
#endif
};


class Str {
    const Slice<const uint8_t> m_bytes;

public:
    template<std::size_t N>
    constexpr Str(const char (&s)[N]) noexcept :
        m_bytes((const std::uint8_t*)s, s[N - 1] == 0 ? N - 1 : N)
    {}

    template<std::size_t N>
    constexpr Str(char (&s)[N]) noexcept :
        m_bytes((const std::uint8_t*)s, s[N - 1] == 0 ? N - 1 : N)
    {}

    Str(std::string& s) noexcept : Str(&s[0], s.size()) {}
    Str(const std::string& s) noexcept : Str(&s[0], s.size()) {}

    explicit Str(const char* s, size_t len) noexcept : m_bytes((uint8_t const*)s, len) {}

    constexpr Str(const MutStr s) noexcept : m_bytes(s.as_bytes().as_ptr(), s.as_bytes().len()) {}

    constexpr auto len() const -> size_t {
        return m_bytes.len();
    }

    constexpr auto is_empty() const -> bool {
        return m_bytes.is_empty();
    }

    const char* c_str() const {
        return (const char*)m_bytes.as_ptr();
    }

    constexpr auto as_bytes() const -> Slice<const uint8_t> {
        return m_bytes;
    }

    bool starts_with(Str pat) const {
        if (pat.len() <= len()) {
            return strncmp(c_str(), pat.c_str(), pat.len()) == 0;
        } else {
            return false;
        }
    }

    uint8_t const& operator[](size_t i) { return m_bytes[i]; }

    constexpr bool operator==(Str const& rhs) const {
        return m_bytes == rhs.m_bytes;
    }

    constexpr bool operator==(MutStr const& rhs) const {
        return m_bytes == rhs.as_bytes();
    }

#ifdef NDEBUG
    constexpr uint8_t const& operator[](size_t i) const { return m_bytes[i]; }
#else
    uint8_t const& operator[](size_t i) const { return m_bytes[i]; }
#endif
};

inline constexpr bool operator==(MutStr const& lhs, Str const& rhs) noexcept {
    return lhs.as_bytes() == rhs.as_bytes();
}

namespace str {

template<std::size_t N>
inline constexpr auto init(const char (&s)[N]) noexcept -> Str {
    return Str(s);
}

template<std::size_t N>
inline constexpr auto init(char (&s)[N]) noexcept -> MutStr {
    return MutStr(s);
}

inline Str ref(std::string const& s) noexcept {
    return Str(&s[0], s.size());
}

inline MutStr ref(std::string& s) noexcept {
    return MutStr(&s[0], s.size());
}

inline auto from_c_str(const char* s) noexcept -> Str {
    return Str(s, std::strlen(s));
}

inline auto from_c_str(char* s) noexcept -> MutStr {
    return MutStr(s, std::strlen(s));
}

inline MutStr from_raw_parts(char* s, std::size_t len) {
    return MutStr(s, len);
}

inline Str from_raw_parts(const char* s, std::size_t len) {
    return Str(s, len);
}

} // namespace str
} // namespace rxx

#endif /* end of include guard: __RXX_STR_HPP__ */

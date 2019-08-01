#ifndef __RXX_STR_HPP__
#define __RXX_STR_HPP__

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

    explicit constexpr MutStr(char *s, size_t len) noexcept : m_bytes((uint8_t*)s, len) {}

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

    constexpr auto as_bytes() const noexcept -> Slice<const uint8_t> {
        return m_bytes;
    }

    uint8_t& operator[](size_t i) { return m_bytes[i]; }

    bool operator==(MutStr const& rhs) const {
        if (len() != rhs.len()) {
            return false;
        }

        return std::strncmp(c_str(), rhs.c_str(), len()) == 0;
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

    explicit constexpr Str(const char* s, size_t len) noexcept : m_bytes((uint8_t const*)s, len) {}

    constexpr Str(const MutStr s) noexcept : m_bytes(s.as_bytes().as_ptr(), s.as_bytes().len()) {}

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

    bool starts_with(Str pat) const {
        if (pat.len() <= len()) {
            return strncmp(c_str(), pat.c_str(), pat.len()) == 0;
        } else {
            return false;
        }
    }

    uint8_t const& operator[](size_t i) { return m_bytes[i]; }

    bool operator==(Str const& rhs) const {
        if (len() != rhs.len())
            return false;

        return std::strncmp(c_str(), rhs.c_str(), len()) == 0;
    }

    bool operator==(MutStr const& rhs) const {
        if (len() != rhs.len())
            return false;

        return std::strncmp(c_str(), rhs.c_str(), len()) == 0;
    }

#ifdef NDEBUG
    constexpr uint8_t const& operator[](size_t i) const { return m_bytes[i]; }
#else
    uint8_t const& operator[](size_t i) const { return m_bytes[i]; }
#endif
};

inline bool operator==(MutStr const& lhs, Str const& rhs) noexcept {
    if (lhs.len() != rhs.len())
        return false;

    return std::strncmp(lhs.c_str(), rhs.c_str(), lhs.len());
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

inline auto from_c_str(const char* s) noexcept -> Str {
    return Str(s, std::strlen(s));
}

inline auto from_c_str(char* s) noexcept -> MutStr {
    return MutStr(s, std::strlen(s));
}

inline Str from_std_string(std::string const& s) noexcept {
    return Str(&s[0], s.size());
}

inline MutStr from_std_string(std::string& s) noexcept {
    return MutStr(&s[0], s.size());
}

inline constexpr MutStr from_raw_parts(char* s, std::size_t len) {
    return MutStr(s, len);
}

inline constexpr Str from_raw_parts(const char* s, std::size_t len) {
    return Str(s, len);
}

} // namespace str
} // namespace rxx

#endif /* end of include guard: __RXX_STR_HPP__ */

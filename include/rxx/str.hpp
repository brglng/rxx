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
    constexpr MutStr(char (&s)[N]) noexcept : m_bytes(static_cast<unsigned char (&)[N]>(s)) {}

    explicit constexpr MutStr(char *s, size_t len) noexcept : m_bytes((uint8_t*)s, len + 1) {}

    constexpr auto len() const noexcept -> size_t {
        return m_bytes.len() - 1;
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
        m_bytes((const unsigned char (&)[N])(s))
    {}

    template<std::size_t N>
    constexpr Str(char (&s)[N]) noexcept :
        m_bytes((const unsigned char (&)[N])(s))
    {}

    explicit constexpr Str(const char* s, size_t len) noexcept : m_bytes((uint8_t const*)s, len + 1) {}

    constexpr Str(const MutStr s) noexcept : m_bytes(s.as_bytes().as_ptr(), s.as_bytes().len()) {}

    constexpr auto len() const -> size_t {
        return m_bytes.len() - 1;
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

inline bool operator==(MutStr const& lhs, Str const& rhs) {
    if (lhs.len() != rhs.len())
        return false;

    return std::strncmp(lhs.c_str(), rhs.c_str(), lhs.len());
}

template<std::size_t N>
inline constexpr auto str(const char (&s)[N]) -> Str {
    return Str(s);
}

template<std::size_t N>
inline constexpr auto str(char (&s)[N]) -> Str {
    return MutStr(s);
}

inline auto str(const char* s) -> Str {
    return Str(s, std::strlen(s));
}

inline auto str(char* s) -> MutStr {
    return MutStr(s, std::strlen(s));
}

inline Str str(std::string const& s) {
    return Str(&s[0], s.size());
}

inline MutStr str(std::string&& s) {
    return MutStr(&s[0], s.size());
}

}

#endif /* end of include guard: __RXX_STR_HPP__ */

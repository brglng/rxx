#ifndef __RXX_STR_CONST_DEF_HPP__
#define __RXX_STR_CONST_DEF_HPP__

#include "rxx/slice/def.hpp"
#include "rxx/u8/def.hpp"

namespace rxx {

class str;

class conststr {
    Slice<const u8> m_bytes;

public:
    template<std::size_t N>
    constexpr conststr(const char (&s)[N]) :
        m_bytes { static_cast<const unsigned char (&)[N]>(s) }
    {}

    template<std::size_t N>
    constexpr conststr(char (&s)[N]) :
        m_bytes { static_cast<const unsigned char (&)[N]>(s) }
    {}

    constexpr conststr(str s);

    constexpr auto len() const -> usize {
        return m_bytes.len();
    }

    constexpr auto is_empty() const -> Bool;

    constexpr auto as_bytes() const -> const Slice<const u8>& {
        return m_bytes;
    }

    constexpr auto as_bytes_const() const -> const Slice<const u8>& {
        return m_bytes;
    }
};

}

#endif /* end of include guard: __RXX_STR_CONST_DEF_HPP__ */

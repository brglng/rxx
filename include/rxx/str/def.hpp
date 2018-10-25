#ifndef __RXX_STR_DEF_HPP__
#define __RXX_STR_DEF_HPP__

#include "rxx/slice/def.hpp"
#include "rxx/u8/def.hpp"

namespace rxx {

class str {
    Slice<u8> m_bytes;

public:
    template<std::size_t N>
    constexpr str(char (&s)[N]) : m_bytes{static_cast<unsigned char (&)[N]>(s)} {}

    constexpr auto len() const -> usize {
        return m_bytes.len();
    }

    constexpr auto is_empty() const -> Bool;

    // auto is_char_boundary(usize index) const -> Bool {

    // }

    auto as_bytes() -> Slice<u8>& {
        return m_bytes;
    }

    constexpr auto as_bytes_const() const -> const Slice<u8>& {
        return m_bytes;
    }
};

}

#include "rxx/str/const/def.hpp"

#endif /* end of include guard: __RXX_STR_DEF_HPP__ */

#ifndef __RXX_SLICE_CONST_DEF_HPP__
#define __RXX_SLICE_CONST_DEF_HPP__

#include "rxx/usize/def.hpp"

namespace rxx {

template<typename T> class Slice;

template<typename T>
class Slice<const T> {
    T const*    m_ptr;
    usize       m_len;

public:
    template<std::size_t N>
    constexpr Slice(const T (&&a)[N]) : m_ptr { a }, m_len { N } {}

    template<std::size_t N>
    constexpr Slice(const T (&a)[N]) : m_ptr { a }, m_len { N } {}

    constexpr Slice(T const* ptr, usize len) : m_ptr { ptr }, m_len { len } {}

    constexpr Slice(Slice<T> mutslice) :
        m_ptr { mutslice.as_ptr() }, m_len { mutslice.len() }
    {}

    auto constexpr len() const -> usize {
        return m_len;
    }

    auto constexpr is_empty() const -> Bool;

    auto first() -> Option<T const&>;

    auto first_const() const -> Option<T const&>;

    // auto split_first() -> Option<tuple<T&, slice<T&>> {
    // }

    auto as_ptr() -> T const* {
        return m_ptr;
    }

    auto as_const_ptr() const -> T const* {
        return m_ptr;
    }
};

}

#endif /* end of include guard: __RXX_SLICE_CONST_HPP__ */

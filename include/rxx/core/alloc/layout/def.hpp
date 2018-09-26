#ifndef __RXX_CORE_ALLOC_LAYOUT_DEF_HPP__
#define __RXX_CORE_ALLOC_LAYOUT_DEF_HPP__

#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/usize/def.hpp"

namespace rxx {

class Layout {
    usize   m_size;
    usize   m_align;

public:
    constexpr Layout(usize size, usize align) noexcept : m_size{size}, m_align{align} {}

    constexpr static auto from_size_align_unchecked(usize size, usize align) noexcept -> Layout {
        return Layout {size, align};
    }

    constexpr auto size() const noexcept -> usize { return m_size; }

    constexpr auto align() const noexcept -> usize { return m_align; }

    template<typename T>
    constexpr static auto New() noexcept -> Layout {
        return Layout::from_size_align_unchecked(sizeof(T), alignof(T));
    }
};

}

#endif /* end of include guard: __RXX_CORE_ALLOC_LAYOUT_DEF_HPP__ */

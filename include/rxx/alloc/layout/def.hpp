#ifndef __RXX_ALLOC_LAYOUT_DEF_HPP__
#define __RXX_ALLOC_LAYOUT_DEF_HPP__

#include <algorithm>
#include <tuple>
#include "rxx/usize/def.hpp"

namespace rxx {

template<typename T> class Option;
template<typename T, typename E> class Result;

namespace alloc {

class LayoutErr {};

class Layout {
    usize   m_size;
    usize   m_align;

public:
    constexpr Layout(usize size, usize align) noexcept : m_size{size}, m_align{align} {}

    static auto from_size_align(usize size, usize align) noexcept -> Result<Layout, LayoutErr>;

    constexpr static auto from_size_align_unchecked(usize size, usize align) noexcept -> Layout {
        return Layout { size, align };
    }

    constexpr auto size() const noexcept -> usize { return m_size; }

    constexpr auto align() const noexcept -> usize { return m_align; }

    template<typename T>
    constexpr static auto New() noexcept -> Layout {
        return Layout::from_size_align_unchecked(sizeof(T), alignof(T));
    }

    auto padding_needed_for(usize align) const -> usize {
        auto len = size();
        auto len_rounded_up = len.wrapping_add(align).wrapping_sub(1) & ~align.wrapping_sub(1);
        return len_rounded_up.wrapping_sub(len);
    }

    auto extend(Layout next) const noexcept -> Result<std::pair<Layout, usize>, LayoutErr>;

    template<typename T>
    static auto array(usize n) noexcept -> Result<Layout, LayoutErr>;
};

}
}

#endif /* end of include guard: __RXX_ALLOC_LAYOUT_DEF_HPP__ */

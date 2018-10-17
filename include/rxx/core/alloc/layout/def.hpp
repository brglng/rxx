#ifndef __RXX_CORE_ALLOC_LAYOUT_DEF_HPP__
#define __RXX_CORE_ALLOC_LAYOUT_DEF_HPP__

#include <algorithm>
#include <tuple>
#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/usize/def.hpp"
#include "rxx/core/option/def.hpp"
#include "rxx/core/result/def.hpp"

namespace rxx {

class LayoutErr {};

class Layout {
    usize   m_size;
    usize   m_align;

public:
    constexpr Layout(usize size, usize align) noexcept : m_size{size}, m_align{align} {}

    static auto from_size_align(usize size, usize align) noexcept -> Result<Layout, LayoutErr> {
        if (!align.is_power_of_two()) {
            return Err(LayoutErr {});
        }

        if (size > usize::MAX - (align - 1)) {
            return Err(LayoutErr {});
        }

        return Ok(Layout::from_size_align_unchecked(size, align));
    }

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

    auto extend(Layout next) const noexcept -> Result<std::pair<Layout, usize>, LayoutErr> {
        auto new_align = std::max(align(), next.align());
        auto pad = padding_needed_for(next.align());

        auto res_offset = size().checked_add(pad).ok_or(LayoutErr {});
        if (res_offset.is_err()) {
            return Err(res_offset.unwrap_err());
        }
        auto offset = res_offset.unwrap();

        auto res_new_size = offset.checked_add(next.size()).ok_or(LayoutErr {});
        if (res_new_size.is_err()) {
            return Err(res_new_size.unwrap_err());
        }
        auto new_size = res_new_size.unwrap();

        auto res_layout = Layout::from_size_align(new_size, new_align);
        if (res_layout.is_err()) {
            return Err(res_layout.unwrap_err());
        }
        auto layout = res_layout.unwrap();

        return Ok(std::make_pair(std::move(layout), std::move(offset)));
    }

    template<typename T>
    static auto array(usize n) noexcept -> Result<Layout, LayoutErr> {
        return Layout::from_size_align(sizeof(T[n.as_size_t()]), alignof(T[n.as_size_t()]));
    }
};

}

#endif /* end of include guard: __RXX_CORE_ALLOC_LAYOUT_DEF_HPP__ */

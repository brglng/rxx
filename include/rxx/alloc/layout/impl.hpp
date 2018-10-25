#ifndef __RXX_ALLOC_LAYOUT_IMPL_HPP__
#define __RXX_ALLOC_LAYOUT_IMPL_HPP__

#include "rxx/option/def.hpp"
#include "rxx/result/def.hpp"
#include "rxx/alloc/layout/def.hpp"

namespace rxx {

namespace alloc {

auto Layout::from_size_align(usize size, usize align) noexcept -> Result<Layout, LayoutErr> {
    if (!align.is_power_of_two()) {
        return Err(LayoutErr {});
    }

    if (size > usize::MAX - (align - 1)) {
        return Err(LayoutErr {});
    }

    return Ok(Layout::from_size_align_unchecked(size, align));
}

auto Layout::extend(Layout next) const noexcept -> Result<std::pair<Layout, usize>, LayoutErr> {
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
auto Layout::array(usize n) noexcept -> Result<Layout, LayoutErr> {
    return Layout::from_size_align(sizeof(T[n.as_size_t()]), alignof(T[n.as_size_t()]));
}

}
}

#endif /* end of include guard: __RXX_ALLOC_LAYOUT_IMPL_HPP__ */

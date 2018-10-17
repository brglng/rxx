#ifndef __RXX_ALLOC_GLOBAL_IMPL_HPP__
#define __RXX_ALLOC_GLOBAL_IMPL_HPP__

#include <cstdlib>
#include <cstring>
#include "rxx/alloc/global/def.hpp"
#include "rxx/core/alloc/layout/def.hpp"
#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/usize/def.hpp"

namespace rxx {

inline auto _Global::alloc(Layout layout) noexcept -> u8* {
    auto request_size = layout.size() + layout.align();
    auto buf = static_cast<u8*>(std::malloc(request_size.as_size_t()));

    if (!buf)
        std::abort();

    auto remainder = (as<usize>(buf)) % layout.align();
    auto offset = layout.align() - remainder;

    auto ret = buf + offset;

    *(ret - 1) = offset.as<u8>();

    return ret;
}

inline void _Global::dealloc(u8* ptr, Layout) noexcept {
    auto offset = *(ptr - 1);
    std::free(ptr - offset);
}

inline auto _Global::alloc_zeroed(Layout layout) noexcept -> u8* {
    auto ptr = alloc(layout);
    std::memset(ptr, 0, layout.size().as<size_t>());
    return ptr;
}

inline auto _Global::realloc(u8* ptr, Layout layout, usize new_size) noexcept -> u8* {
    auto offset = (*(ptr - 1)).as<usize>();
    auto buf = static_cast<u8*>(std::realloc(ptr - offset, new_size.as<size_t>()));

    if (!buf)
        std::abort();

    auto remainder = (as<usize>(buf)) % layout.align();
    offset = layout.align() - remainder;

    auto ret = buf + offset;

    *(ret - 1) = offset.as<u8>();

    return ret;
}


}

#endif /* end of include guard: __RXX_ALLOC_GLOBAL_IMPL_HPP__ */

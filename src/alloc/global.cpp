#include <cstdlib>
#include <cstring>
#include <memory>
#include <pthread.h>
#include "rxx/alloc/global/global.hpp"
#include "rxx/core/as.hpp"
#include "rxx/core/num/u8/u8.hpp"

namespace rxx {

auto _Global::alloc(Layout layout) noexcept -> u8* {
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

void _Global::dealloc(u8* ptr, Layout) noexcept {
    auto offset = *(ptr - 1);
    std::free(ptr - offset);
}

auto _Global::alloc_zeroed(Layout layout) noexcept -> u8* {
    auto ptr = alloc(layout);
    std::memset(ptr, 0, layout.size().as<size_t>());
    return ptr;
}

auto _Global::realloc(u8* ptr, Layout layout, usize new_size) noexcept -> u8* {
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

_Global Global {};

}

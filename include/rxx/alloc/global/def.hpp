#ifndef __RXX_ALLOC_GLOBAL_DEF_HPP__
#define __RXX_ALLOC_GLOBAL_DEF_HPP__

#include <utility>

namespace rxx {

class u8;
class usize;
class Layout;

class _Global {
public:
    constexpr _Global() noexcept {}
    constexpr static auto New() noexcept -> _Global {
        return _Global {};
    }
    auto as_raw() noexcept -> _Global* {
        return new _Global { std::move(*this) };
    }
    auto alloc(Layout layout) noexcept -> u8*;
    void dealloc(u8*, Layout layout) noexcept;
    auto alloc_zeroed(Layout layout) noexcept -> u8*;
    auto realloc(u8* ptr, Layout layout, usize new_size) noexcept -> u8*;
};

extern _Global Global;

}

#endif /* end of include guard: __RXX_ALLOC_GLOBAL_DEF_HPP__ */

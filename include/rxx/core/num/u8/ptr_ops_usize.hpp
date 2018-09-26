#ifndef __RXX_CORE_NUM_U8_PTR_OPS_USIZE_HPP__
#define __RXX_CORE_NUM_U8_PTR_OPS_USIZE_HPP__

#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/usize/def.hpp"

namespace rxx {

inline auto operator+(u8* lh, usize rh) noexcept -> u8* {
    return reinterpret_cast<u8*>(reinterpret_cast<unsigned char*>(lh) + rh.as_size_t());
}

inline auto operator-(u8* lh, usize rh) noexcept -> u8* {
    return reinterpret_cast<u8*>(reinterpret_cast<unsigned char*>(lh) - rh.as_size_t());
}

inline auto operator-(u8* lh, u8 rh) noexcept -> u8* {
    return reinterpret_cast<u8*>(reinterpret_cast<unsigned char*>(lh) - rh.as_uint8_t());
}

}

#endif /* end of include guard: __RXX_CORE_NUM_U8_PTR_OPS_USIZE_HPP__ */

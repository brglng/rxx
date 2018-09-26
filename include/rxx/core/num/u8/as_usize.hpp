#ifndef __RXX_CORE_NUM_U8_AS_USIZE_HPP__
#define __RXX_CORE_NUM_U8_AS_USIZE_HPP__

#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/usize/def.hpp"
#include "rxx/core/as.hpp"

namespace rxx {

template<>
struct As<usize, u8> {
    static constexpr auto as(u8 from) noexcept -> usize {
        return from.as_uint8_t();
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_U8_AS_USIZE_HPP__ */

#ifndef __RXX_CORE_NUM_USIZE_AS_U8_HPP__
#define __RXX_CORE_NUM_USIZE_AS_U8_HPP__

#include "rxx/core/as.hpp"
#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/usize/def.hpp"

namespace rxx {

template<>
struct As<u8, usize> {
    static constexpr auto as(usize u) noexcept -> u8 {
        return u.as_size_t();
    }
};

}

#endif /* end of include guard: __RXX_CORE_NUM_USIZE_AS_U8_HPP__ */

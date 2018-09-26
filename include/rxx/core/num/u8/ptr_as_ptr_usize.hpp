#ifndef __RXX_CORE_NUM_USIZE_AS_SIZE_T_HPP__
#define __RXX_CORE_NUM_USIZE_AS_SIZE_T_HPP__

#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/usize/def.hpp"
#include "rxx/core/as.hpp"

namespace rxx {

template<>
struct As<usize, u8*> {
    static auto as(u8* from) noexcept -> usize {
        return reinterpret_cast<std::size_t>(from);
    }
};

};

#endif /* end of include guard: __RXX_CORE_NUM_USIZE_AS_SIZE_T_HPP__ */

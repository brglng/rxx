#ifndef __RXX_CORE_NUM_USIZE_AS_SIZE_T_HPP__
#define __RXX_CORE_NUM_USIZE_AS_SIZE_T_HPP__

#include "rxx/core/num/usize/def.hpp"
#include "rxx/core/as.hpp"

namespace rxx {

template<>
struct As<std::size_t, usize> {
    static constexpr auto as(usize u) noexcept -> std::size_t {
        return u.as_size_t();
    }
};

};

#endif /* end of include guard: __RXX_CORE_NUM_USIZE_AS_SIZE_T_HPP__ */

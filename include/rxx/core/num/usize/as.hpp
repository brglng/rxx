#ifndef __RXX_CORE_NUM_USIZE_AS_HPP__
#define __RXX_CORE_NUM_USIZE_AS_HPP__

#include "rxx/core/num/usize/def.hpp"
#include "rxx/core/as.hpp"

namespace rxx {

template<typename T>
inline T usize::as() const noexcept {
    return as<T>(*this);
}

}

#endif /* end of include guard: __RXX_CORE_NUM_USIZE_AS_HPP__ */

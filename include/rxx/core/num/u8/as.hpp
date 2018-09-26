#ifndef __RXX_CORE_NUM_U8_AS_HPP__
#define __RXX_CORE_NUM_U8_AS_HPP__

#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/usize/def.hpp"
#include "rxx/core/as.hpp"

namespace rxx {

template<typename T>
inline T u8::as() const noexcept {
    return as<T>(*this);
}

}

#endif /* end of include guard: __RXX_CORE_NUM_U8_AS_HPP__ */

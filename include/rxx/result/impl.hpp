#ifndef __RXX_RESULT_IMPL_HPP__
#define __RXX_RESULT_IMPL_HPP__

#include "rxx/result/def.hpp"
#include "rxx/panic/def.hpp"
#include "rxx/macros.hpp"

namespace rxx {

template<typename T, typename E>
inline auto Result<T, E>::unwrap() -> T {
    RXX_ASSERT(is_ok());
    return std::move(val());
}

template<typename T, typename E>
inline auto Result<T, E>::unwrap_err() -> E {
    RXX_ASSERT(!is_ok());
    return std::move(err());
}

}

#endif /* end of include guard: __RXX_RESULT_IMPL_HPP__ */

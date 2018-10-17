#ifndef __RXX_CORE_RESULT_IMPL_HPP__
#define __RXX_CORE_RESULT_IMPL_HPP__

#include "rxx/core/result/def.hpp"
#include "rxx/core/panic/def.hpp"

namespace rxx {

template<typename T, typename E>
inline auto Result<T, E>::unwrap() -> T {
    if (!is_ok()) {
        panic();
    }
    return std::move(val());
}

template<typename T, typename E>
inline auto Result<T, E>::unwrap_err() -> E {
    if (is_ok()) {
        panic();
    }
    return std::move(err());
}

}

#endif /* end of include guard: __RXX_CORE_RESULT_IMPL_HPP__ */

#ifndef __RXX_OPTION_IMPL_HPP__
#define __RXX_OPTION_IMPL_HPP__

#include "rxx/option/def.hpp"
#include "rxx/str/def.hpp"
#include "rxx/result/def.hpp"

namespace rxx {

template<typename T>
template<typename E>
inline auto Option<T>::ok_or(E&& err) -> Result<T, E> {
    if (is_some()) {
        return Ok(std::move(val()));
    } else {
        return Err(std::forward<E>(err));
    }
}

template<typename T>
inline auto Option<T>::expect(conststr msg) noexcept -> T {
    RXX_ASSERT(is_some(), msg);
    return std::move(val());
}

}

#include "rxx/option/ref/impl.hpp"

#endif /* end of include guard: __RXX_CORE_OPTION_IMPL_HPP__ */

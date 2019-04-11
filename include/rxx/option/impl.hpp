#ifndef __RXX_OPTION_IMPL_HPP__
#define __RXX_OPTION_IMPL_HPP__

#include "rxx/option/def.hpp"
#include "rxx/result/def.hpp"

namespace rxx {

template<typename T>
template<typename E>
auto Option<T>::ok_or(E&& err) noexcept -> Result<T, E> {
    if (inited()) {
        return Ok(std::move(val()));
    } else {
        return Err(std::forward<E>(err));
    }
}

template<typename T>
template<typename E>
auto Option<T>::ok_or(E&& err) const noexcept -> Result<T, E> {
    if (inited()) {
        return Ok(val());
    } else {
        return Err(std::forward<E>(err));
    }
}

template<typename T>
template<typename F>
auto Option<T>::ok_or_else(F&& err) -> Result<T, typename invoke_result<typename std::decay<F>::type>::type> {
    if (inited()) {
        return Ok(std::move(val()));
    } else {
        return Err(invoke(std::forward<F>(err)));
    }
}

template<typename T>
template<typename F>
auto Option<T>::ok_or_else(F&& err) const -> Result<T, typename invoke_result<typename std::decay<F>::type>::type> {
    if (inited()) {
        return Ok(val());
    } else {
        return Err(invoke(std::forward<F>(err)));
    }
}

}

#endif /* end of include guard: __RXX_OPTION_IMPL_HPP__ */

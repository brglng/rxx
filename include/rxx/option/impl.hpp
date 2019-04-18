#ifndef __RXX_OPTION_IMPL_HPP__
#define __RXX_OPTION_IMPL_HPP__

#include "rxx/option/def.hpp"
#include "rxx/result/def.hpp"

namespace rxx {

template<typename T>
template<typename E>
inline auto Option<T>::ok_or(E&& err) noexcept -> Result<T, E> {
    if (inited()) {
        return Ok(static_move(val()));
    } else {
        return Err(static_forward<E>(err));
    }
}

template<typename T>
template<typename E>
inline auto Option<T>::ok_or(E&& err) const noexcept -> Result<T, E> {
    if (inited()) {
        return Ok(val());
    } else {
        return Err(static_forward<E>(err));
    }
}

template<typename T>
template<typename F>
inline
enable_if_t<!std::is_void<invoke_result_t<decay_t<F>>>::value,
            Result<T, invoke_result_t<decay_t<F>>>>
Option<T>::ok_or_else(F&& err) {
    if (inited()) {
        return Ok(static_move(val()));
    } else {
        return Err(invoke(static_forward<F>(err)));
    }
}

template<typename T>
template<typename F>
inline
enable_if_t<std::is_void<invoke_result_t<decay_t<F>>>::value, Result<T, void>>
Option<T>::ok_or_else(F&& err) {
    if (inited()) {
        return Ok(static_move(val()));
    } else {
        invoke(static_forward<F>(err));
        return Err();
    }
}

template<typename T>
template<typename F>
inline
enable_if_t<!std::is_void<invoke_result_t<decay_t<F>>>::value,
            Result<T, invoke_result_t<decay_t<F>>>>
Option<T>::ok_or_else(F&& err) const {
    if (inited()) {
        return Ok(val());
    } else {
        return Err(invoke(static_forward<F>(err)));
    }
}

template<typename T>
template<typename F>
inline
enable_if_t<std::is_void<invoke_result_t<decay_t<F>>>::value, Result<T, void>>
Option<T>::ok_or_else(F&& err) const {
    if (inited()) {
        return Ok(val());
    } else {
        invoke(static_forward<F>(err));
        return Err();
    }
}

}

#endif /* end of include guard: __RXX_OPTION_IMPL_HPP__ */

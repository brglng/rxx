#ifndef __RXX_RESULT_IMPL_HPP__
#define __RXX_RESULT_IMPL_HPP__

#include "rxx/result/def.hpp"
#include "rxx/option/def.hpp"

namespace rxx {

template<typename T, typename E>
inline auto Result<T, E>::ok() -> Option<T> {
    if (is_ok()) {
        return Some(static_move(value()));
    } else {
        return None;
    }
}

template<typename T, typename E>
inline auto Result<T, E>::ok() const -> Option<T> {
    if (is_ok()) {
        return Some(value());
    } else {
        return None;
    }
}

template<typename T, typename E>
inline auto Result<T, E>::err() -> Option<E> {
    if (is_err()) {
        return Some(static_move(error()));
    } else {
        return None;
    }
}

template<typename T, typename E>
inline auto Result<T, E>::err() const -> Option<E> {
    if (is_err()) {
        return Some(error());
    } else {
        return None;
    }
}

template<typename E>
inline auto Result<void, E>::ok() const -> Option<void> {
    return Option<void>();
}

template<typename E>
inline auto Result<void, E>::err() -> Option<E> {
    if (is_err()) {
        return Some(static_move(error()));
    } else {
        return None;
    }
}

template<typename E>
inline auto Result<void, E>::err() const -> Option<E> {
    if (is_err()) {
        return Some(error());
    } else {
        return None;
    }
}

template<typename T>
inline auto Result<T, void>::ok() -> Option<T> {
    return Some(static_move(value()));
}

template<typename T>
inline auto Result<T, void>::ok() const -> Option<T> {
    return Some(value());
}

template<typename T>
inline auto Result<T, void>::err() const -> Option<void> {
    return None;
}

inline auto Result<void, void>::ok() const -> Option<void> {
    return None;
}

inline auto Result<void, void>::err() const -> Option<void> {
    return None;
}

}

#endif /* end of include guard: __RXX_RESULT_IMPL_HPP__ */

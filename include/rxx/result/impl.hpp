#ifndef __RXX_RESULT_IMPL_HPP__
#define __RXX_RESULT_IMPL_HPP__

#include "rxx/result/def.hpp"
#include "rxx/option/def.hpp"

namespace rxx {

template<typename T, typename E>
auto Result<T, E>::ok() -> Option<T> {
    if (is_ok()) {
        return Some(std::move(value()));
    } else {
        return None;
    }
}

template<typename T, typename E>
auto Result<T, E>::ok() const -> Option<T> {
    if (is_ok()) {
        return Some(value());
    } else {
        return None;
    }
}

template<typename T, typename E>
auto Result<T, E>::err() -> Option<E> {
    if (is_err()) {
        return Some(std::move(error()));
    } else {
        return None;
    }
}

template<typename T, typename E>
auto Result<T, E>::err() const -> Option<E> {
    if (is_err()) {
        return Some(error());
    } else {
        return None;
    }
}

}

#endif /* end of include guard: __RXX_RESULT_IMPL_HPP__ */

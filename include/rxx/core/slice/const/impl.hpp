#ifndef __RXX_CORE_SLICE_CONST_IMPL_HPP__
#define __RXX_CORE_SLICE_CONST_IMPL_HPP__

#include "rxx/core/bool/def.hpp"
#include "rxx/core/option/def.hpp"
#include "rxx/core/slice/const/def.hpp"

namespace rxx {

template<typename T>
inline auto constexpr Slice<const T>::is_empty() const -> Bool {
    return m_len == 0;
}

template<typename T>
inline auto Slice<const T>::first() -> Option<T const&> {
    if (m_len > 0) {
        return Some(m_ptr[0]);
    } else {
        return None;
    }
}

template<typename T>
inline auto Slice<const T>::first_const() const -> Option<T const&> {
    if (m_len > 0) {
        return Some(m_ptr[0]);
    } else {
        return None;
    }
}

}

#include "rxx/core/slice/const/impl.hpp"

#endif /* end of include guard: __RXX_CORE_SLICE_CONST_IMPL_HPP__ */

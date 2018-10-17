#ifndef __RXX_CORE_OPTION_PTR_IMPL_HPP__
#define __RXX_CORE_OPTION_PTR_IMPL_HPP__

#include "rxx/core/bool/def.hpp"
#include "rxx/core/option/ref/def.hpp"
#include "rxx/core/str/def.hpp"
#include "rxx/core/panic/def.hpp"

namespace rxx {

template<typename T>
inline auto Option<T&>::is_some() const -> Bool {
    return m_ptr != nullptr;
}

template<typename T>
inline auto Option<T&>::is_none() const -> Bool {
    return !is_some();
}

template<typename T>
inline auto Option<T&>::expect(const str msg) -> T& {
    if (is_none()) {
        panic(msg);
    }
    T* ptr = m_ptr;
    m_ptr = nullptr;
    return ptr;
}

template<typename T>
template<typename E>
inline auto Option<T&>::ok_or(E&& err) -> Result<T&, E> {
    if (is_some()) {
        return Ok(*m_ptr);
    } else {
        return Err(std::forward<E>(err));
    }
}

template<typename T>
inline auto Option<T&>::operator==(Option<T&> const& that) const -> Bool {
    return *m_ptr == *that.m_ptr;
}

template<typename T>
inline auto Option<T&>::operator==(option::impl::None const&) const -> Bool {
    return m_ptr == nullptr;
}

}

#endif /* end of include guard: __RXX_CORE_OPTION_PTR_IMPL_HPP__ */

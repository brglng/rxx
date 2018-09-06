#ifndef __RXX_CORE_AS_HPP__
#define __RXX_CORE_AS_HPP__

#include <utility>

namespace rxx {

template<typename T, typename U>
struct As;

template<typename T, typename U>
inline T as(U&& u) noexcept(As<T, U>::as(std::forward<U>(u))) {
    return As<T, U>::as(std::forward<U>(u));
};

}

#endif /* end of include guard: __RXX_CORE_AS_HPP__ */

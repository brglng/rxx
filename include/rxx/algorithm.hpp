#ifndef __RXX_ALGORITHM_HPP__
#define __RXX_ALGORITHM_HPP__

#include <algorithm>

namespace rxx {

template<class T>
constexpr const T& static_max(T const& a) {
    return a;
}

template<class T, class... Args>
constexpr const T& static_max(T const& a, Args const&... args) {
    return a > static_max(args...) ? a : static_max(args...);
}

}

#endif /* end of include guard: __RXX_ALGORITHM_HPP__ */

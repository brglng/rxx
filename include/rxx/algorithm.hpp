#ifndef __RXX_ALGORITHM_HPP__
#define __RXX_ALGORITHM_HPP__

#include <algorithm>

namespace rxx {

template<class T>
constexpr const T& max(T const& a) {
    return a;
}

template<class T, class... Args>
constexpr const T& max(T const& a, Args const&... args) {
    return a > rxx::max(args...) ? a : rxx::max(args...);
}

}

#endif /* end of include guard: __RXX_ALGORITHM_HPP__ */

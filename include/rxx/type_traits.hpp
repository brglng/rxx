#ifndef __RXX_TYPE_TRAITS_HPP__
#define __RXX_TYPE_TRAITS_HPP__

#include <type_traits>

namespace rxx {

#define RXX_REQUIRES(...) typename std::enable_if<__VA_ARGS__::value, bool>::type = false

}

#endif /* end of include guard: __RXX_TYPE_TRAITS_HPP__ */

#ifndef __RXX_OPTION_PROTO_HPP__
#define __RXX_OPTION_PROTO_HPP__

#include <type_traits>

namespace rxx {
namespace option {

struct None {
    struct Init {};
    explicit constexpr None(Init) {}
};

}

constexpr option::None None(option::None::Init{});

template<typename T>    class Option;
template<>              class Option<void>;
template<typename T>    class Option<T&>;

template<typename T>
inline auto Some(T&& value) -> Option<typename std::decay<T>::type>;

template<typename T> struct is_option : std::false_type {};
template<typename T> struct is_option<Option<T>> : std::true_type {};

template<typename O1, typename O2> struct is_same_option : std::false_type {};
template<typename T1, typename T2> struct is_same_option<Option<T1>, Option<T2>> : std::true_type {};

}

#endif /* end of include guard: __RXX_OPTION_PROTO_HPP__ */

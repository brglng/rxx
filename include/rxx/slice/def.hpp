#ifndef __RXX_SLICE_DEF_HPP__
#define __RXX_SLICE_DEF_HPP__

#include <type_traits>
#include "rxx/usize/def.hpp"

namespace rxx {

class Bool;
template<typename T> class Option;
template<typename T> class Slice;
template<typename T> class Slice<const T>;

template<typename T>
class Slice {
    T*        m_ptr;
    usize     m_len;

public:
    template<std::size_t N>
    constexpr Slice(T (&&a)[N]) : m_ptr { a }, m_len { N } {}

    template<std::size_t N>
    constexpr Slice(T (&a)[N]) : m_ptr { a }, m_len { N } {}

    constexpr Slice(T* ptr, usize len) : m_ptr { ptr }, m_len { len } {}

    auto constexpr len() const -> usize {
        return m_len;
    }

    auto constexpr is_empty() const -> Bool;
    auto first() -> Option<T&>;
    auto first_const() const -> Option<T const&>;

    // auto split_first() -> Option<tuple<T&, slice<T&>> {
    // }

    auto as_ptr() -> T* {
        return m_ptr;
    }

    auto as_const_ptr() const -> T const* {
        return m_ptr;
    }
};

template<typename T, std::size_t N>
constexpr auto slice(T (&&a)[N]) -> Slice<T> {
    return Slice<T> { std::forward<T[N]>(a) };
}

template<typename T, std::size_t N>
constexpr auto slice(T (&a)[N]) -> Slice<T> {
    return Slice<T> { a };
}

template<typename T>
constexpr auto slice(T* ptr, usize len) -> Slice<T> {
    return Slice<T> { ptr, len };
}

template<typename... Args>
constexpr auto slice(Args&&... args) -> Slice<typename std::common_type<Args...>::type> {
    return { std::forward<Args>(args)... };
}

}

#include "rxx/slice/const/def.hpp"

#endif /* end of include guard: __RXX_SLICE_DEF_HPP__ */

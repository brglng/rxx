#ifndef __RXX_SLICE_HPP__
#define __RXX_SLICE_HPP__

#include <cassert>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <cstdlib>
#include "rxx/utility.hpp"

namespace rxx {

template<typename T> class Slice;
template<typename T> class Slice<const T>;

template<typename T>
class Slice {
    T*      m_ptr;
    size_t  m_len;

public:
    template<std::size_t N>
    constexpr Slice(T (&&a)[N]) : m_ptr(a), m_len(N) {}

    template<std::size_t N>
    constexpr Slice(T (&a)[N]) : m_ptr(a), m_len(N) {}

    constexpr Slice(T* ptr, size_t len) : m_ptr(ptr), m_len(len) {}

    auto constexpr len() const -> size_t {
        return m_len;
    }

    auto constexpr is_empty() const -> bool {
        return m_len == 0;
    }

    // auto first() -> Option<T&>;
    // constexpr auto first() const -> Option<T const&>;

    // auto split_first() -> Option<tuple<T&, slice<T&>> {
    // }

    auto as_ptr() -> T* {
        return m_ptr;
    }

    constexpr auto as_ptr() const -> T const* {
        return m_ptr;
    }

    T& operator[](size_t i) {
        assert(i < m_len);
        return m_ptr[i];
    }

#ifdef NDEBUG
    constexpr T const& operator[](size_t i) const {
        return m_ptr[i];
    }
#else
    T const& operator[](size_t i) const {
        assert(i < m_len);
        return m_ptr[i];
    }
#endif
};

template<typename T>
class Slice<const T> {
    T const*    m_ptr;
    size_t      m_len;

public:
    template<std::size_t N>
    constexpr Slice(const T (&&a)[N]) : m_ptr(a), m_len(N) {}

    template<std::size_t N>
    constexpr Slice(const T (&a)[N]) : m_ptr(a), m_len(N) {}

    constexpr Slice(T const* ptr, size_t len) : m_ptr(ptr), m_len(len) {}

    constexpr Slice(Slice<T> mutslice) :
        m_ptr(mutslice.as_ptr()), m_len(mutslice.len())
    {}

    constexpr auto len() const -> size_t {
        return m_len;
    }

    constexpr auto is_empty() const -> bool {
        return m_len == 0;
    }

    // auto first() -> Option<T const&>;

    // constexpr auto first() const -> Option<T const&>;

    // auto split_first() -> Option<tuple<T&, slice<T&>> {
    // }

    constexpr auto as_ptr() const -> T const* {
        return m_ptr;
    }

#ifdef NDEBUG
    constexpr T const& operator[](size_t i) const {
        return m_ptr[i];
    }
#else
    T const& operator[](size_t i) const {
        assert(i < m_len);
        return m_ptr[i];
    }
#endif
};

template<typename T, std::size_t N>
constexpr auto slice(T (&&a)[N]) -> Slice<T> {
    return Slice<T>(rxx::forward<T[N]>(a));
}

template<typename T, std::size_t N>
constexpr auto slice(T (&a)[N]) -> Slice<T> {
    return Slice<T>(a);
}

template<typename T>
constexpr auto slice(T* ptr, size_t len) -> Slice<T> {
    return Slice<T>(ptr, len);
}

template<typename... Args>
constexpr auto slice(Args&&... args) -> Slice<rxx::remove_reference_t<rxx::common_type_t<Args...>>> {
    using T = rxx::remove_reference_t<rxx::common_type_t<Args...>>;
    return T(rxx::forward<Args>(args)...);
}

}

#endif /* end of include guard: __RXX_SLICE_HPP__ */

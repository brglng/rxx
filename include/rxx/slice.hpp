#ifndef __RXX_SLICE_HPP__
#define __RXX_SLICE_HPP__

#include <cassert>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include "rxx/type_traits.hpp"
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
    constexpr Slice(T (&a)[N]) : m_ptr(a), m_len(N) {}

    constexpr Slice(T* ptr, size_t len) : m_ptr(ptr), m_len(len) {}

    Slice(std::vector<T>& vec) : m_ptr(&vec[0]), m_len(vec.size()) {}

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

    T* begin() { return &m_ptr[0]; }
    T* end() { return &m_ptr[m_len]; }
    constexpr const T* begin() const { return &m_ptr[0]; }
    constexpr const T* end() const { return &m_ptr[m_len]; }
};

template<typename T>
class Slice<const T> {
    const T*    m_ptr;
    size_t      m_len;

public:
    template<std::size_t N>
    constexpr Slice(const T (&a)[N]) : m_ptr(a), m_len(N) {}

    constexpr Slice(std::initializer_list<T> il) : m_ptr(il.begin()), m_len(il.size()) {}

    constexpr Slice(const T* ptr, size_t len) : m_ptr(ptr), m_len(len) {}

    constexpr Slice(Slice<T> mutslice) :
        m_ptr(mutslice.as_ptr()), m_len(mutslice.len())
    {}

    Slice(const std::vector<T>& vec) : m_ptr(&vec[0]), m_len(vec.size()) {}

    constexpr auto len() const -> size_t {
        return m_len;
    }

    constexpr auto is_empty() const -> bool {
        return m_len == 0;
    }

    // auto first() -> Option<T const&>;

    // constexpr auto first() const -> Option<const T&>;

    // auto split_first() -> Option<tuple<T&, slice<T&>> {
    // }

    constexpr auto as_ptr() const -> const T* {
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

    constexpr const T* begin() const { return &m_ptr[0]; }
    constexpr const T* end() const { return &m_ptr[m_len]; }
};

namespace slice {

template<typename T, std::size_t N>
inline constexpr auto init(T (&a)[N]) -> Slice<T> {
    return Slice<T>(a);
}

template<typename T, std::size_t N>
inline constexpr auto init(const T (&a)[N]) -> Slice<const T> {
    return Slice<const T>(a);
}

template<class T>
inline constexpr auto init(std::initializer_list<T> il) -> Slice<const T> {
    return Slice<const T>(il);
}

template<typename T>
inline constexpr auto from_raw_parts(T* ptr, size_t len) -> Slice<T> {
    return Slice<T>(ptr, len);
}

} // namespace slice

} // namespace rxx

#endif /* end of include guard: __RXX_SLICE_HPP__ */

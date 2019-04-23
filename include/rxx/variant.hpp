#ifndef __RXX_ENUM_HPP__
#define __RXX_ENUM_HPP__

#include <cassert>
#include <iostream>
#include <typeindex>
#include "rxx/type_traits.hpp"
#include "rxx/utility.hpp"

namespace rxx {

namespace variant {
namespace impl {

template<typename T, typename... Ts>
struct Destroy {
    static void destroy(std::type_index type_id, void* storage) {
        if (type_id == std::type_index(typeid(T))) {
            reinterpret_cast<T*>(storage)->T::~T();
        } else {
            Destroy<Ts...>::destroy(type_id, storage);
        }
    }
};

template<typename T>
struct Destroy<T> {
    static void destroy(std::type_index type_id, void* storage) {
        if (type_id == std::type_index(typeid(T))) {
            reinterpret_cast<T*>(storage)->T::~T();
        }
    }
};

template<typename T, typename... Ts>
struct Move {
    static void construct(std::type_index from_id, void* from, void* to) {
        if (from_id == std::type_index(typeid(T))) {
            new (to) T(std::move(*reinterpret_cast<T*>(from)));
        } else {
            Move<Ts...>::construct(from_id, from, to);
        }
    }

    static void assign(std::type_index from_id, void* from, void* to) {
        if (from_id == std::type_index(typeid(T))) {
            *reinterpret_cast<T*>(to) = std::move(*reinterpret_cast<T*>(from));
        } else {
            Move<Ts...>::assign(from_id, from, to);
        }
    }
};

template<typename T>
struct Move<T> {
    static void construct(std::type_index from_id, void* from, void* to) {
        if (from_id == std::type_index(typeid(T))) {
            new (to) T(std::move(*reinterpret_cast<T*>(from)));
        }
    }

    static void assign(std::type_index from_id, void* from, void* to) {
        if (from_id == std::type_index(typeid(T))) {
            *reinterpret_cast<T*>(to) = std::move(*reinterpret_cast<T*>(from));
        }
    }
};

template<typename T, typename... Ts>
struct Copy {
    static void construct(std::type_index from_id, void const* from, void* to) {
        if (from_id == std::type_index(typeid(T))) {
            new (to) T(*reinterpret_cast<T const*>(from));
        } else {
            Copy<Ts...>::construct(from_id, from, to);
        }
    }

    static void assign(std::type_index from_id, void const* from, void* to) {
        if (from_id == std::type_index(typeid(T))) {
            *reinterpret_cast<T*>(to) = *reinterpret_cast<T const*>(from);
        } else {
            Copy<Ts...>::assign(from_id, from, to);
        }
    }
};

template<typename T>
struct Copy<T> {
    static void construct(std::type_index from_id, void const* from, void* to) {
        if (from_id == std::type_index(typeid(T))) {
            new (to) T(*reinterpret_cast<T const*>(from));
        }
    }

    static void assign(std::type_index from_id, void const* from, void* to) {
        if (from_id == std::type_index(typeid(T))) {
            *reinterpret_cast<T*>(to) = *reinterpret_cast<T const*>(from);
        }
    }
};

template <typename T>
struct Identity { using type = T; };

template <typename... Ts> struct Overload;

template <> struct Overload<> { void operator()() const; };

template <typename T, typename... Ts>
struct Overload<T, Ts...> : Overload<Ts...> {
    using Overload<Ts...>::operator();
    Identity<T> operator()(T) const;
};

// void is a valid variant alternative, but "T operator()(T)" is ill-formed
// when T is void
template <typename... Ts>
struct Overload<void, Ts...> : Overload<Ts...> {
    using Overload<Ts...>::operator();
    Identity<void> operator()() const;
};

// Find the best match out of `Ts...` with `T` as the argument.
template <typename T, typename... Ts>
using BestMatch = typename std::result_of<Overload<Ts...>(T)>::type;

} // namespace impl
} // namespace variant

template<typename... Ts>
class Variant {
    aligned_union_t<1, Ts...> m_storage;
    std::type_index m_type_id;

    void destroy() {
        if (m_type_id != void_type_id()) {
            variant::impl::Destroy<Ts...>::destroy(m_type_id, &m_storage);
        }
    }

    static std::type_index void_type_id() {
        return typeid(void);
    }

public:
    Variant() : m_type_id(void_type_id()) {}

    Variant(Variant<Ts...>& that) : Variant(std::move(that)) {}

    Variant(Variant<Ts...>&& that) : m_type_id(that.m_type_id) {
        if (that.m_type_id != void_type_id()) {
            variant::impl::Move<Ts...>::construct(that.m_type_id, &that.m_storage, &m_storage);
            that.m_type_id = void_type_id();
        }
    }

    Variant(Variant<Ts...> const& that) {
        if (m_type_id != that.m_type_id) {
            destroy();
        }
        if (that.m_type_id != void_type_id()) {
            variant::impl::Copy<Ts...>::construct(that.m_type_id, &that.m_storage, &m_storage);
            m_type_id = that.m_type_id;
        }
    }

    template<typename T>
    Variant(T&& t) : m_type_id(typeid(typename variant::impl::BestMatch<T&&, Ts...>::type)) {
        using U = typename variant::impl::BestMatch<T&&, Ts...>::type;
        new (&m_storage) U(std::forward<T>(t));
    }

    ~Variant() {
        destroy();
    }

    Variant<Ts...>& operator=(Variant<Ts...>&& that) {
        if (this != &that) {
            if (m_type_id != that.m_type_id) {
                destroy();
            }
            if (that.m_type_id != void_type_id()) {
                variant::impl::Move<Ts...>::assign(that.m_type_id, &that.m_storage, &m_storage);
                m_type_id = that.m_type_id;
                that.m_type_id = void_type_id();
            }
        }
        return *this;
    }

    Variant<Ts...>& operator=(Variant<Ts...> const& that) {
        if (this != &that) {
            if (m_type_id != that.m_type_id) {
                destroy();
            }
            if (that.m_type_id != void_type_id()) {
                variant::impl::Copy<Ts...>::assign(that.m_type_id, &that.m_storage, &m_storage);
                m_type_id = that.m_type_id;
            }
        }
        return *this;
    }

    template<typename T>
    Variant<Ts...>& operator=(T&& t) {
        using U = typename variant::impl::BestMatch<T&&, Ts...>::type;
        *reinterpret_cast<U*>(&m_storage) = std::forward<T>(t);
        m_type_id = std::type_index(typeid(U));
        return *this;
    }

    std::type_index type_index() const {
        return m_type_id;
    }

    template<typename T>
    bool is() const {
        return m_type_id == std::type_index(typeid(T));
    }

    template<typename T>
    T& as() {
        assert(m_type_id == std::type_index(typeid(T)));
        return *reinterpret_cast<T*>(&m_storage);
    }

    template<typename T>
    T const& as() const {
        assert(m_type_id == std::type_index(typeid(T)));
        return *reinterpret_cast<T const*>(&m_storage);
    }
};

namespace variant {

template<class... Args, class T>
auto make(T&& value) -> Variant<decay_t<Args>...> {
    return Variant<decay_t<Args>...>(static_forward<T>(value));
}

}

}

#endif /* end of include guard: __RXX_ENUM_HPP__ */

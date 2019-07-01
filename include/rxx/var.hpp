#ifndef __RXX_VAR_HPP__
#define __RXX_VAR_HPP__

#include <cstdio>
#include <cassert>
#include "rxx/invoke.hpp"
#include "rxx/type_traits.hpp"
#include "rxx/utility.hpp"

namespace rxx {

namespace var {
namespace impl {

template<class T, class... Ts> struct TypeIndex;

template<class T> struct TypeIndex<T> {
    static constexpr std::size_t value = 0;
};

template<class T, class... Ts> struct TypeIndex<T, T, Ts...> {
    static constexpr std::size_t value = 1;
};

template<class T, class U, class... Ts> struct TypeIndex<T, U, Ts...> {
    static constexpr std::size_t value = TypeIndex<T, Ts...>::value + 1;
};

template<size_t Index, class... Ts> struct TypeOfIndex;

template<size_t Index> struct TypeOfIndex<Index> {};

template<class... Ts> struct TypeOfIndex<0, Ts...> {};

template<class T, class... Ts> struct TypeOfIndex<1, T, Ts...> {
    using type = T;
};

template<size_t Index, class T, class... Ts> struct TypeOfIndex<Index, T, Ts...> {
    using type = typename TypeOfIndex<Index - 1, Ts...>::type;
};

template<typename T> struct Destroy {
    static void destroy(void* storage) noexcept {
        reinterpret_cast<T*>(storage)->T::~T();
    }
};

template<> struct Destroy<void> {
    static void destroy(void*) noexcept {}
};

typedef void DestroyFunc(void* storage);

template<typename T> struct Move {
    static void construct(void* from, void* to) {
        new (to) T(rxx::move(*reinterpret_cast<T*>(from)));
    }

    static void assign(void* from, void* to) {
        *reinterpret_cast<T*>(to) = rxx::move(*reinterpret_cast<T*>(from));
    }
};

template<> struct Move<void> {
    static void construct(void*, void*) noexcept {}
    static void assign(void*, void*) noexcept {}
};

typedef void MoveConstructFunc(void* from, void* to);
typedef void MoveAssignFunc(void* from, void* to);

template<typename T> struct Copy {
    static void construct(void const* from, void* to) {
        new (to) T(*reinterpret_cast<T const*>(from));
    }

    static void assign(void const* from, void* to) {
        *reinterpret_cast<T*>(to) = *reinterpret_cast<T const*>(from);
    }
};

template<> struct Copy<void> {
    static void construct(void const*, void*) noexcept {}
    static void assign(void const*, void*) noexcept {}
};

typedef void CopyConstructFunc(void const* from, void* to);
typedef void CopyAssignFunc(void const* from, void* to);

template<class R, class V, class T> struct Visit {
    static constexpr R visit(V&& visitor, void* storage) {
        return rxx::invoke(visitor, *reinterpret_cast<T*>(storage));
    };
};

template<class V, class T> struct Visit<void, V, T> {
    static void visit(V&& visitor, void* storage) {
        rxx::invoke(visitor, *reinterpret_cast<T*>(storage));
    };
};

template<class R, class V> struct Visit<R, V, void> {
    static constexpr R visit(V&& visitor, void*) {
        return rxx::invoke(visitor);
    };
};

template<class V> struct Visit<void, V, void> {
    static void visit(V&& visitor, void*) {
        rxx::invoke(visitor);
    };
};

template<class V> struct InvalidVisit {
    static void visit(V&&, void*) {
        assert(0);
    };
};

template<class R, class V>
using VisitFunc = decltype(Visit<R, V, void>::visit);

template<class R, class V, class... Ts> struct VisitDispatcher {
    static constexpr rxx::var::impl::VisitFunc<R, V>* funcs[] = {
        &InvalidVisit<V>::visit,
        &Visit<R, V, Ts>::visit...
    };
};

template<class R, class V, class... Ts>
constexpr rxx::var::impl::VisitFunc<R, V>* rxx::var::impl::VisitDispatcher<R, V, Ts...>::funcs[];

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
} // namespace var

template<typename... Ts>
class Var {
    rxx::aligned_union_t<1, Ts...> m_storage;
    std::size_t m_type_index;

    static constexpr rxx::var::impl::DestroyFunc* m_destroy_funcs[] = {
        &rxx::var::impl::Destroy<Ts>::destroy...
    };

    static constexpr rxx::var::impl::MoveConstructFunc* m_move_construct_funcs[] = {
        &rxx::var::impl::Move<Ts>::construct...
    };

    static constexpr rxx::var::impl::MoveAssignFunc* m_move_assign_funcs[] = {
        &rxx::var::impl::Move<Ts>::assign...
    };

    static constexpr rxx::var::impl::CopyConstructFunc* m_copy_construct_funcs[] = {
        &rxx::var::impl::Copy<Ts>::construct...
    };

    static constexpr rxx::var::impl::CopyAssignFunc* m_copy_assign_funcs[] = {
        &rxx::var::impl::Copy<Ts>::assign...
    };

    void destroy() {
        m_destroy_funcs[m_type_index - 1](&m_storage);
    }

public:
    Var() : m_type_index(0) {}

    Var(Var<Ts...> const& that) {
        if (m_type_index != that.m_type_index) {
            destroy();
        }
        if (that.m_type_index != 0) {
            m_copy_construct_funcs[that.m_type_index - 1](&that.m_storage, &m_storage);
            m_type_index = that.m_type_index;
        }
    }

    Var(Var<Ts...>&& that) : m_type_index(that.m_type_index) {
        if (that.m_type_index != 0) {
            m_move_construct_funcs[that.m_type_index - 1](&that.m_storage, &m_storage);
            that.m_type_index = 0;
        }
    }

    template<typename T>
    Var(T&& t) : m_type_index(rxx::var::impl::TypeIndex<typename rxx::var::impl::BestMatch<T&&, Ts...>::type, Ts...>::value) {
        using U = typename rxx::var::impl::BestMatch<T&&, Ts...>::type;
        new (&m_storage) U(rxx::move(t));
    }

    ~Var() {
        destroy();
    }

    Var<Ts...>& operator=(Var<Ts...> const& that) {
        if (this != &that) {
            if (m_type_index != that.m_type_index) {
                destroy();
            }
            if (that.m_type_index != 0) {
                m_copy_assign_funcs[that.m_type_index - 1](&that.m_storage, &m_storage);
                m_type_index = that.m_type_index;
            }
        }
        return *this;
    }

    Var<Ts...>& operator=(Var<Ts...>&& that) {
        if (this != &that) {
            if (m_type_index != that.m_type_index) {
                destroy();
            }
            if (that.m_type_index != 0) {
                m_move_assign_funcs[that.m_type_index - 1](&that.m_storage, &m_storage);
                m_type_index = that.m_type_index;
                that.m_type_index = 0;
            }
        }
        return *this;
    }

    template<typename T>
    Var<Ts...>& operator=(T&& t) {
        using U = typename rxx::var::impl::BestMatch<T&&, Ts...>::type;
        *reinterpret_cast<U*>(&m_storage) = std::forward<T>(t);
        m_type_index = rxx::var::impl::TypeIndex<U, Ts...>::value;
        return *this;
    }

    void* storage() { return &m_storage; }
    void const* storage() const { return &m_storage; }

    std::size_t index() const noexcept {
        assert(m_type_index != 0);
        return m_type_index - 1;
    }

    bool valid() const noexcept {
        return m_type_index != 0;
    }

    template<typename T>
    bool is() const noexcept {
        return m_type_index == rxx::var::impl::TypeIndex<T, Ts...>::value;
    }

    template<typename T>
    T& as() noexcept {
        assert((m_type_index == rxx::var::impl::TypeIndex<T, Ts...>::value));
        return *reinterpret_cast<T*>(&m_storage);
    }

    template<typename T>
    T const& as() const noexcept {
        assert((m_type_index == rxx::var::impl::TypeIndex<T, Ts...>::value));
        return *reinterpret_cast<T const*>(&m_storage);
    }
};

template<typename... Ts> constexpr rxx::var::impl::DestroyFunc*         rxx::Var<Ts...>::m_destroy_funcs[];
template<typename... Ts> constexpr rxx::var::impl::MoveConstructFunc*   rxx::Var<Ts...>::m_move_construct_funcs[];
template<typename... Ts> constexpr rxx::var::impl::MoveAssignFunc*      rxx::Var<Ts...>::m_move_assign_funcs[];
template<typename... Ts> constexpr rxx::var::impl::CopyConstructFunc*   rxx::Var<Ts...>::m_copy_construct_funcs[];
template<typename... Ts> constexpr rxx::var::impl::CopyAssignFunc*      rxx::Var<Ts...>::m_copy_assign_funcs[];

template<class R, class V, class... Ts>
inline constexpr R visit(V&& v, Var<Ts...>& var) {
    return rxx::var::impl::VisitDispatcher<R, V, Ts...>::funcs[var.index() + 1](rxx::forward<V>(v), var.storage());
}

template<class R, class V, class... Ts>
inline constexpr R visit(V&& v, Var<Ts...> const& var) {
    return rxx::var::impl::VisitDispatcher<R, V, Ts...>::funcs[var.index() + 1](rxx::forward<V>(v), var.storage());
}

template<class R, class V, class... Ts>
inline constexpr R visit(V&& v, Var<Ts...>&& var) {
    return rxx::var::impl::VisitDispatcher<R, V, Ts...>::funcs[var.index() + 1](rxx::forward<V>(v), var.storage());
}

template<class V, class... Ts>
inline void visit(V&& v, Var<Ts...>& var) {
    rxx::var::impl::VisitDispatcher<void, V, Ts...>::funcs[var.index() + 1](rxx::forward<V>(v), var.storage());
}

template<class V, class... Ts>
inline void visit(V&& v, Var<Ts...> const& var) {
    rxx::var::impl::VisitDispatcher<void, V, Ts...>::funcs[var.index() + 1](rxx::forward<V>(v), var.storage());
}

template<class V, class... Ts>
inline void visit(V&& v, Var<Ts...>&& var) {
    rxx::var::impl::VisitDispatcher<void, V, Ts...>::funcs[var.index() + 1](rxx::forward<V>(v), var.storage());
}

namespace var {

template<class... Args, class T>
inline auto make(T&& value) -> Var<rxx::remove_reference_t<Args>...> {
    return Var<rxx::remove_reference_t<Args>...>(rxx::forward<T>(value));
}

} // namespace var

} // namespace rxx

#endif /* end of include guard: __RXX_VAR_HPP__ */

#ifndef __RXX_OPTION_DEF_HPP__
#define __RXX_OPTION_DEF_HPP__

#include <cstdio>
#include <cassert>
#include "rxx/invoke.hpp"
#include "rxx/type_traits.hpp"
#include "rxx/utility.hpp"
#include "rxx/str.hpp"

namespace rxx {

namespace option {
namespace impl {

struct None {
    struct init {};
    explicit constexpr None(init) {}
};

constexpr struct TrivialInit {} trivial_init {};

template<class T>
union Storage
{
    unsigned char m_dummy;
    T m_value;

    constexpr Storage(TrivialInit) noexcept : m_dummy{} {};

    template<class... Args>
    constexpr Storage(Args&&... args) : m_value{static_forward<Args>(args)...} {}

    ~Storage(){}
};


template<class T>
union ConstExprStorage
{
    unsigned char m_dummy;
    T m_value;

    constexpr ConstExprStorage(TrivialInit) noexcept : m_dummy{} {};

    template <class... Args>
    constexpr ConstExprStorage(Args&&... args) : m_value{static_forward<Args>(args)...} {}

    ~ConstExprStorage() = default;
};

template <class T>
struct OptionBase
{
    bool m_inited;
    Storage<T> m_storage;

    constexpr OptionBase() noexcept : m_inited{false}, m_storage{trivial_init} {};

    explicit constexpr OptionBase(const T& v) : m_inited{true}, m_storage{v} {}

    explicit constexpr OptionBase(T&& v) : m_inited{true}, m_storage{static_move(v)} {}

    template<class... Args>
    explicit OptionBase(InPlace, Args&&... args)
        : m_inited{true}, m_storage{static_forward<Args>(args)...} {}

    template <class U, class... Args, RXX_REQUIRES(std::is_constructible<T, std::initializer_list<U>>)>
    explicit OptionBase(InPlace, std::initializer_list<U> il, Args&&... args)
        : m_inited{true}, m_storage{il, std::forward<Args>(args)...} {}

    ~OptionBase() { if (m_inited) m_storage.m_value.T::~T(); }
};


template <class T>
struct ConstExprOptionBase
{
    bool m_inited;
    ConstExprStorage<T> m_storage;

    constexpr ConstExprOptionBase() noexcept : m_inited{false}, m_storage{trivial_init} {};

    explicit constexpr ConstExprOptionBase(const T& v) : m_inited{true}, m_storage{v} {}

    explicit constexpr ConstExprOptionBase(T&& v) : m_inited{true}, m_storage{static_move(v)} {}

    template<class... Args>
    explicit constexpr ConstExprOptionBase(InPlace, Args&&... args)
        : m_inited{true}, m_storage{static_forward<Args>(args)...} {}

    template<class U, class... Args, RXX_REQUIRES(std::is_constructible<T, std::initializer_list<U>>)>
    explicit constexpr ConstExprOptionBase(InPlace, std::initializer_list<U> il, Args&&... args)
        : m_inited{true}, m_storage{il, std::forward<Args>(args)...} {}

    ~ConstExprOptionBase() = default;
};

template <class T>
using Base = typename std::conditional<
    std::is_trivially_destructible<T>::value,                   // if possible
    ConstExprOptionBase<typename std::remove_const<T>::type>,   // use base with trivial destructor
    OptionBase<typename std::remove_const<T>::type>
>::type;

}
}

constexpr option::impl::None None{option::impl::None::init {}};

template<typename T, typename E> class Result;
template<typename T, typename E> class Result<T&, E>;
template<typename T, typename E> class Result<T, E&>;
template<typename T, typename E> class Result<T&, E&>;
template<typename T> class Option;
template<typename T> class Option<T&>;

template<typename T>
inline auto Some(T&& value) -> Option<typename std::decay<T>::type>;

template<typename T>
class Option : private option::impl::Base<T> {
    static_assert( !std::is_same<typename std::decay<T>::type, option::impl::None>::value, "bad T" );
    static_assert( !std::is_same<typename std::decay<T>::type, InPlace>::value, "bad T" );

    constexpr bool inited() const noexcept {
        return option::impl::Base<T>::m_inited;
    }

    void set_inited(bool inited) noexcept {
        option::impl::Base<T>::m_inited = inited;
    }

    auto valptr() noexcept -> typename std::remove_const<T>::type* {
        return std::addressof(option::impl::Base<T>::m_storage.m_value);
    }

    constexpr auto valptr() const noexcept -> T const* {
        return static_addressof(option::impl::Base<T>::m_storage.m_value);
    }

    constexpr const T& val() const& {
        return option::impl::Base<T>::m_storage.m_value;
    }

    T&& val() && {
        return std::move(option::impl::Base<T>::m_storage.m_value);
    }

    T& val() & {
        return std::move(option::impl::Base<T>::m_storage.m_value);
    }

    template<class... Args>
    void init(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) {
        assert(!inited());
        ::new (static_cast<void*>(valptr())) T{std::forward<Args>(args)...};
        set_inited(true);
    }

    template<class U, class... Args>
    void init(std::initializer_list<U> il, Args&&... args) noexcept(noexcept(T(il, std::forward<Args>(args)...))) {
        assert(!inited());
        ::new (static_cast<void*>(valptr())) T{il, std::forward<Args>(args)...};
        set_inited(true);
    }

    void clear() noexcept {
        if (inited()) {
            valptr()->T::~T();
            set_inited(false);
        }
    }

public:
    constexpr Option() noexcept : option::impl::Base<T>{} {}
    constexpr Option(option::impl::None) noexcept : option::impl::Base<T>{} {}

    Option(Option const& that) noexcept(std::is_nothrow_copy_constructible<T>::value)
        : option::impl::Base<T>{} {
        if (that.inited()) {
            ::new (static_cast<void*>(valptr())) T{that.val()};
            set_inited(true);
        }
    }

    Option(Option&& that) noexcept(std::is_nothrow_move_constructible<T>::value)
        : option::impl::Base<T>{} {
        if (that.inited()) {
            ::new (static_cast<void*>(valptr())) T{std::move(that.val())};
            set_inited(true);
        }
    }

    explicit constexpr Option(T const& value) noexcept(std::is_nothrow_copy_constructible<T>::value)
        : option::impl::Base<T>{value} {}

    explicit constexpr Option(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
        : option::impl::Base<T>{static_move(value)} {}

    template<class... Args>
    explicit constexpr Option(InPlace, Args&&... args)
        : option::impl::Base<T>{InPlace {}, static_forward<Args>(args)...} {}

    template<class U, class... Args, RXX_REQUIRES(std::is_constructible<T, std::initializer_list<U>>)>
    explicit constexpr Option(InPlace, std::initializer_list<U> il, Args&&... args)
        : option::impl::Base<T>{InPlace{}, il, static_forward<Args>(args)...} {}

    ~Option() = default;

    auto operator=(option::impl::None) noexcept -> Option& {
        clear();
        return *this;
    }

    auto operator=(Option const& that) noexcept(std::is_nothrow_copy_assignable<T>::value && std::is_nothrow_copy_constructible<T>::value) -> Option& {
        if      (inited() && !that.inited())    clear();
        else if (!inited() && that.inited())    init(that.val());
        else if (inited() && that.inited())     val() = that.val();
        return *this;
    }

    auto operator=(Option&& that) noexcept(std::is_nothrow_move_assignable<T>::value && std::is_nothrow_move_constructible<T>::value) -> Option& {
        if      (inited() && !that.inited())    clear();
        else if (!inited() && that.inited())    init(std::move(that.val()));
        else if (inited() && that.inited())     val() = std::move(that.val());
        return *this;
    }

    explicit constexpr operator bool() const noexcept { return inited(); }
    
    constexpr auto is_some() const noexcept -> bool {
        return inited();
    }

    constexpr auto is_none() const noexcept -> bool {
        return !inited();
    }

    auto as_ref() noexcept -> Option<T&> {
        return Option<T&>(val());
    }

    auto as_const() const noexcept -> Option<T const&> {
        return Option<T const&>(val());
    }

    auto expect(Str msg) noexcept -> T {
        if (!inited()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return std::move(val());
    }

    auto expect(Str msg) const noexcept -> T {
        if (!inited()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return val();
    }

    auto unwrap() -> T {
        assert(inited());
        return std::move(val());
    }

    auto unwrap() const -> T {
        assert(inited());
        return val();
    }

    auto unwrap_or(T&& def) noexcept -> T {
        if (inited()) {
            return std::move(val());
        } else {
            return std::move(def);
        }
    }

    auto unwrap_or(T const& def) noexcept -> T {
        if (inited()) {
            return std::move(val());
        } else {
            return def;
        }
    }

    auto unwrap_or(T&& def) const noexcept -> T {
        if (inited()) {
            return val();
        } else {
            return std::move(def);
        }
    }

    auto unwrap_or(T const& def) const noexcept -> T {
        if (inited()) {
            return val();
        } else {
            return def;
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& f) -> T {
        if (inited()) {
            return std::move(val());
        } else {
            return invoke(std::forward<F>(f));
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& f) const -> T {
        if (inited()) {
            return val();
        } else {
            return invoke(std::forward<F>(f));
        }
    }

    template<typename F>
    auto map(F&& f) -> Option<typename invoke_result<typename std::decay<F>::type, T&&>::type> {
        if (inited()) {
            return Some(invoke(std::forward<F>(f), std::move(val())));
        } else {
            return None;
        }
    }

    template<typename F>
    auto map(F&& f) const -> Option<typename invoke_result<typename std::decay<F>::type, T const&>::type> {
        if (inited()) {
            return Some(invoke(std::forward<F>(f), val()));
        } else {
            return None;
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F&& f) noexcept -> U {
        if (inited()) {
            return invoke(std::forward<F>(f), std::move(val()));
        } else {
            return std::forward<U>(def);
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F&& f) const noexcept -> U {
        if (inited()) {
            return invoke(std::forward<F>(f), val());
        } else {
            return std::forward<U>(def);
        }
    }

    template<typename D, typename F>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<F>::type, T&&>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type
    >::type
    map_or_else(D&& def, F&& f) {
        if (inited()) {
            return invoke(std::forward<F>(f), std::move(val()));
        } else {
            return invoke(std::forward<D>(def));
        }
    }

    template<typename D, typename F>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<F>::type, T const&>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type
    >::type
    map_or_else(D&& def, F&& f) {
        if (inited()) {
            return invoke(std::forward<F>(f), val());
        } else {
            return invoke(std::forward<D>(def));
        }
    }

    template<typename E>
    auto ok_or(E&& err) noexcept -> Result<T, E>;

    template<typename E>
    auto ok_or(E&& err) const noexcept -> Result<T, E>;

    template<typename F>
    auto ok_or_else(F&& err) -> Result<T, typename invoke_result<typename std::decay<F>::type>::type>;

    template<typename F>
    auto ok_or_else(F&& err) const -> Result<T, typename invoke_result<typename std::decay<F>::type>::type>;

    template<typename U>
    auto and_(Option<U> optb) const -> Option<U> {
        if (!inited()) return None;
        else return optb;
    }

    template<typename F>
    auto and_then(F&& f) const -> Option<typename invoke_result<typename std::decay<F>::type>::type> {
        if (!inited()) return None;
        else return invoke(std::forward<F>(f));
    }

    template<typename P>
    auto filter(P&& predicate) -> Option<T> {
        if (!inited()) {
            return None;
        } else {
            if (invoke(std::forward<P>(predicate), val())) {
                return std::move(*this);
            } else {
                return None;
            }
        }
    }

    template<typename P>
    auto filter(P&& predicate) const -> Option<T> {
        if (!inited()) {
            return None;
        } else {
            if (invoke(std::forward<P>(predicate), val())) {
                return *this;
            } else {
                return None;
            }
        }
    }

    auto or_(Option<T> optb) -> Option<T> {
        if (inited()) {
            return std::move(*this);
        } else {
            return optb;
        }
    }

    auto or_(Option<T> optb) const -> Option<T> {
        if (inited()) {
            return *this;
        } else {
            return optb;
        }
    }

    template<typename F>
    auto or_else(F&& f) -> Option<T> {
        if (inited()) {
            return std::move(*this);
        } else {
            return invoke(std::forward<F>(f));
        }
    }

    template<typename F>
    auto or_else(F&& f) const -> Option<T> {
        if (inited()) {
            return *this;
        } else {
            return invoke(std::forward<F>(f));
        }
    }

    auto xor_(Option<T> optb) -> Option<T> {
        if (inited() && !optb.inited()) {
            return std::move(*this);
        } else if (!inited() && optb.inited()) {
            return optb;
        } else {
            return None;
        }
    }

    auto xor_(Option<T> optb) const -> Option<T> {
        if (inited() && !optb.inited()) {
            return *this;
        } else if (!inited() && optb.inited()) {
            return optb;
        } else {
            return None;
        }
    }

    auto get_or_insert(T const& v) -> T& {
        if (!inited()) {
            init(v);
        } else {
            return val();
        }
    }

    auto get_or_insert(T&& v) -> T& {
        if (!inited()) {
            init(std::move(v));
        } else {
            return val();
        }
    }

    template<class F>
    auto get_or_insert_with(F&& f) -> T& {
        if (!inited()) {
            init(invoke(std::forward<F>(f)));
        } else {
            return val();
        }
    }

    auto take() -> Option<T> {
        if (inited()) {
            set_inited(false);
            return Some(std::move(val()));
        } else {
            return None;
        }
    }

    auto replace(T const& value) -> Option<T> {
        if (inited()) {
            auto old_value = std::move(val());
            val() = value;
            return Some(std::move(old_value));
        } else {
            init(value);
            return None;
        }
    }

    auto replace(T&& value) -> Option<T> {
        if (inited()) {
            auto old_value = std::move(val());
            val() = std::move(value);
            return Some(std::move(old_value));
        } else {
            init(std::move(value));
            return None;
        }
    }

    auto unwrap_or_default() -> T {
        if (inited()) {
            return std::move(val());
        } else {
            return T {};
        }
    }

    friend constexpr bool operator==(const Option<T>& x, const Option<T>& y);
    friend constexpr bool operator<(const Option<T>& x, const Option<T>& y);

    friend constexpr bool operator==(const Option<T>& x, const T& v);
    friend constexpr bool operator==(const T& v, const Option<T>& x);
    friend constexpr bool operator!=(const Option<T>& x, const T& v);
    friend constexpr bool operator!=(const T& v, const Option<T>& x);
    friend constexpr bool operator<(const Option<T>& x, const T& v);
    friend constexpr bool operator>(const T& v, const Option<T>& x);
    friend constexpr bool operator>(const Option<T>& x, const T& v);
    friend constexpr bool operator<(const T& v, const Option<T>& x);
    friend constexpr bool operator>=(const Option<T>& x, const T& v);
    friend constexpr bool operator<=(const T& v, const Option<T>& x);
    friend constexpr bool operator<=(const Option<T>& x, const T& v);
    friend constexpr bool operator>=(const T& v, const Option<T>& x);
};

template<typename T>
class Option<T&> {
    static_assert(!std::is_same<T, option::impl::None>::value, "bad T" );
    static_assert(!std::is_same<T, InPlace>::value, "bad T" );

    T* m_ptr;

    constexpr bool inited() const noexcept { return m_ptr != nullptr; }
    T& val() noexcept { return *m_ptr; }
    constexpr T const& val() const noexcept { return *m_ptr; }

public:
    constexpr Option() noexcept  : m_ptr{nullptr} {}

    constexpr Option(option::impl::None) noexcept : m_ptr{nullptr} {}

    explicit constexpr Option(T& t) noexcept : m_ptr{static_addressof(t)} {}

    Option(T&&) = delete;

    constexpr Option(Option const& that) noexcept : m_ptr{that.m_ptr} {}
    constexpr Option(Option&& that) noexcept : m_ptr{that.m_ptr} {}

    auto operator=(Option const& that) noexcept -> Option& {
        m_ptr = that.m_ptr;
        return *this;
    }

    auto operator=(Option&& that) noexcept -> Option& {
        m_ptr = that.m_ptr;
        return *this;
    }

    auto operator=(option::impl::None) noexcept -> Option& {
        m_ptr = nullptr;
        return *this;
    }

    explicit constexpr operator bool() const noexcept { return inited(); }

    constexpr auto is_some() const noexcept -> bool { return inited(); }
    constexpr auto is_none() const noexcept -> bool { return !inited(); }

    auto as_ref() noexcept -> Option<T&> {
        return Option<T&> { *m_ptr };
    }

    constexpr auto as_const() const noexcept -> Option<T const&> {
        return Option<T const&> { *m_ptr };
    }

    auto expect(Str msg) noexcept -> T& {
        if (!inited()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return std::move(val());
    }

    auto expect(Str msg) const noexcept -> T const& {
        if (!inited()) {
            std::fprintf(stderr, "%s\n", msg.c_str());
            std::abort();
        }
        return val();
    }

    auto unwrap() noexcept -> T& {
        assert(inited());
        return val();
    }

    auto unwrap() const noexcept -> T const& {
        assert(inited());
        return val();
    }

    auto unwrap_or(T& def) noexcept -> T& {
        if (inited()) {
            return val();
        } else {
            return def;
        }
    }

    auto unwrap_or(T const& def) noexcept -> T const& {
        if (inited()) {
            return val();
        } else {
            return def;
        }
    }

    auto unwrap_or(T& def) const noexcept -> T const& {
        if (inited()) {
            return val();
        } else {
            return def;
        }
    }

    auto unwrap_or(T const& def) const noexcept -> T const& {
        if (inited()) {
            return val();
        } else {
            return def;
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& f) -> T& {
        if (inited()) {
            return val();
        } else {
            return invoke(std::forward<F>(f));
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& f) const -> T const& {
        if (inited()) {
            return val();
        } else {
            return invoke(std::forward<F>(f));
        }
    }

    template<typename F>
    auto map(F&& f) -> Option<typename invoke_result<typename std::decay<F>::type, T&&>::type> {
        if (inited()) {
            return Some(invoke(std::forward<F>(f), std::move(val())));
        } else {
            return None;
        }
    }

    template<typename F>
    auto map(F&& f) const -> Option<typename invoke_result<typename std::decay<F>::type, T const&>::type> {
        if (inited()) {
            return Some(invoke(std::forward<F>(f), val()));
        } else {
            return None;
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F&& f) -> U {
        if (inited()) {
            return invoke(std::forward<F>(f), std::move(val()));
        } else {
            return std::forward<U>(def);
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F&& f) const -> U {
        if (inited()) {
            return invoke(std::forward<F>(f), val());
        } else {
            return std::forward<U>(def);
        }
    }

    template<typename D, typename F>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<F>::type, T&&>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type
    >::type
    map_or_else(D&& def, F&& f) {
        if (inited()) {
            return invoke(std::forward<F>(f), std::move(val()));
        } else {
            return invoke(std::forward<D>(def));
        }
    }

    template<typename D, typename F>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<F>::type, T const&>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type
    >::type
    map_or_else(D&& def, F&& f) {
        if (inited()) {
            return invoke(std::forward<F>(f), val());
        } else {
            return invoke(std::forward<D>(def));
        }
    }

    // template<typename E>
    // auto ok_or(E&& err) noexcept -> Result<T&, E> {
    //     if (inited()) {
    //         return Ok(std::move(val()));
    //     } else {
    //         return Err(std::forward<E>(err));
    //     }
    // }

    // template<typename F>
    // auto ok_or_else(F&& err) -> Result<T&, typename invoke_result<typename std::decay<F>::type>::type> {
    //     if (inited()) {
    //         return Ok(std::move(val()));
    //     } else {
    //         return Err(invoke(std::forward<F>(err)));
    //     }
    // }

    template<typename U>
    auto and_(Option<U> optb) const -> Option<U> {
        if (!inited()) return None;
        else return optb;
    }

    template<typename F>
    auto and_then(F&& f) const -> Option<typename invoke_result<typename std::decay<F>::type>::type> {
        if (!inited()) return None;
        else return invoke(std::forward<F>(f));
    }

    template<typename P>
    auto filter(P&& predicate) -> Option<T&> {
        if (!inited()) {
            return None;
        } else {
            if (invoke(std::forward<P>(predicate), val())) {
                return *this;
            } else {
                return None;
            }
        }
    }

    template<typename P>
    auto filter(P&& predicate) const -> Option<T const&> {
        if (!inited()) {
            return None;
        } else {
            if (invoke(std::forward<P>(predicate), val())) {
                return *this;
            } else {
                return None;
            }
        }
    }

    auto or_(Option<T&> optb) -> Option<T&> {
        if (inited()) {
            return *this;
        } else {
            return optb;
        }
    }

    auto or_(Option<T&> optb) const -> Option<T const&> {
        if (inited()) {
            return *this;
        } else {
            return optb;
        }
    }

    template<typename F>
    auto or_else(F&& f) -> Option<T&> {
        if (inited()) {
            return *this;
        } else {
            return invoke(std::forward<F>(f));
        }
    }

    template<typename F>
    auto or_else(F&& f) const -> Option<T const&> {
        if (inited()) {
            return *this;
        } else {
            return invoke(std::forward<F>(f));
        }
    }

    auto xor_(Option<T&> optb) -> Option<T&> {
        if (inited() && !optb.inited()) {
            return *this;
        } else if (!inited() && optb.inited()) {
            return optb;
        } else {
            return None;
        }
    }

    auto xor_(Option<T&> optb) const -> Option<T const&> {
        if (inited() && !optb.inited()) {
            return *this;
        } else if (!inited() && optb.inited()) {
            return optb;
        } else {
            return None;
        }
    }

    auto get_or_insert(T& v) -> T& {
        if (!inited()) {
            m_ptr = &v;
        } else {
            return val();
        }
    }

    template<class F>
    auto get_or_insert_with(F&& f) -> T& {
        if (!inited()) {
            m_ptr = std::addressof(invoke(std::forward<F>(f)));
        } else {
            return val();
        }
    }

    auto take() -> Option<T&> {
        if (inited()) {
            T* ptr = m_ptr;
            m_ptr = nullptr;
            return Option<T&>(*ptr);
        } else {
            return None;
        }
    }

    auto replace(T& value) -> Option<T&> {
        if (inited()) {
            T* old_ptr = m_ptr;
            m_ptr = std::addressof(value);
            return Option<T&>(*old_ptr);
        } else {
            init(value);
            return None;
        }
    }

    friend constexpr bool operator==(const Option<T&>& x, const Option<T&>& y);
    friend constexpr bool operator<(const Option<T&>& x, const Option<T&>& y);

    friend constexpr bool operator==(const Option<T&>& x, const T& v);
    friend constexpr bool operator==(const T& v, const Option<T&>& x);
    friend constexpr bool operator!=(const Option<T&>& x, const T& v);
    friend constexpr bool operator!=(const T& v, const Option<T&>& x);
    friend constexpr bool operator<(const Option<T&>& x, const T& v);
    friend constexpr bool operator>(const T& v, const Option<T&>& x);
    friend constexpr bool operator>(const Option<T&>& x, const T& v);
    friend constexpr bool operator<(const T& v, const Option<T&>& x);
    friend constexpr bool operator>=(const Option<T&>& x, const T& v);
    friend constexpr bool operator<=(const T& v, const Option<T&>& x);
    friend constexpr bool operator<=(const Option<T&>& x, const T& v);
    friend constexpr bool operator>=(const T& v, const Option<T&>& x);
    friend constexpr bool operator==(const Option<const T&>& x, const T& v);
    friend constexpr bool operator==(const T& v, const Option<const T&>& x);
    friend constexpr bool operator!=(const Option<const T&>& x, const T& v);
    friend constexpr bool operator!=(const T& v, const Option<const T&>& x);
    friend constexpr bool operator<(const Option<const T&>& x, const T& v);
    friend constexpr bool operator>(const T& v, const Option<const T&>& x);
    friend constexpr bool operator>(const Option<const T&>& x, const T& v);
    friend constexpr bool operator<(const T& v, const Option<const T&>& x);
    friend constexpr bool operator>=(const Option<const T&>& x, const T& v);
    friend constexpr bool operator<=(const T& v, const Option<const T&>& x);
    friend constexpr bool operator<=(const Option<const T&>& x, const T& v);
    friend constexpr bool operator>=(const T& v, const Option<const T&>& x);
};

template <class T>
class Option<T&&>
{
    static_assert(sizeof(T) == 0, "Option rvalue references disallowed");
};

// Relational operators
template <class T> constexpr bool operator==(const Option<T>& x, const Option<T>& y)
{
    return bool(x) != bool(y) ? false : bool(x) == false ? true : x.val() == y.val();
}

template <class T> constexpr bool operator!=(const Option<T>& x, const Option<T>& y)
{
    return !(x == y);
}

template <class T> constexpr bool operator<(const Option<T>& x, const Option<T>& y)
{
    return (!y) ? false : (!x) ? true : x.val() < y.val();
}

template <class T> constexpr bool operator>(const Option<T>& x, const Option<T>& y)
{
    return (y < x);
}

template <class T> constexpr bool operator<=(const Option<T>& x, const Option<T>& y)
{
    return !(y < x);
}

template <class T> constexpr bool operator>=(const Option<T>& x, const Option<T>& y)
{
    return !(x < y);
}

// Comparison with nullopt
template <class T> constexpr bool operator==(const Option<T>& x, option::impl::None) noexcept
{
    return (!x);
}

template <class T> constexpr bool operator==(option::impl::None, const Option<T>& x) noexcept
{
    return (!x);
}

template <class T> constexpr bool operator!=(const Option<T>& x, option::impl::None) noexcept
{
    return bool(x);
}

template <class T> constexpr bool operator!=(option::impl::None, const Option<T>& x) noexcept
{
    return bool(x);
}

template <class T> constexpr bool operator<(const Option<T>&, option::impl::None) noexcept
{
    return false;
}

template <class T> constexpr bool operator<(option::impl::None, const Option<T>& x) noexcept
{
    return bool(x);
}

template <class T> constexpr bool operator<=(const Option<T>& x, option::impl::None) noexcept
{
    return (!x);
}

template <class T> constexpr bool operator<=(option::impl::None, const Option<T>&) noexcept
{
    return true;
}

template <class T> constexpr bool operator>(const Option<T>& x, option::impl::None) noexcept
{
    return bool(x);
}

template <class T> constexpr bool operator>(option::impl::None, const Option<T>&) noexcept
{
    return false;
}

template <class T> constexpr bool operator>=(const Option<T>&, option::impl::None) noexcept
{
    return true;
}

template <class T> constexpr bool operator>=(option::impl::None, const Option<T>& x) noexcept
{
    return (!x);
}

// Comparison with T
template <class T> constexpr bool operator==(const Option<T>& x, const T& v)
{
    return bool(x) ? x.val() == v : false;
}

template <class T> constexpr bool operator==(const T& v, const Option<T>& x)
{
    return bool(x) ? v == x.val() : false;
}

template <class T> constexpr bool operator!=(const Option<T>& x, const T& v)
{
    return bool(x) ? x.val() != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const Option<T>& x)
{
    return bool(x) ? v != x.val() : true;
}

template <class T> constexpr bool operator<(const Option<T>& x, const T& v)
{
    return bool(x) ? x.val() < v : true;
}

template <class T> constexpr bool operator>(const T& v, const Option<T>& x)
{
    return bool(x) ? v > x.val() : true;
}

template <class T> constexpr bool operator>(const Option<T>& x, const T& v)
{
    return bool(x) ? x.val() > v : false;
}

template <class T> constexpr bool operator<(const T& v, const Option<T>& x)
{
    return bool(x) ? v < x.val() : false;
}

template <class T> constexpr bool operator>=(const Option<T>& x, const T& v)
{
    return bool(x) ? x.val() >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const Option<T>& x)
{
    return bool(x) ? v <= x.val() : false;
}

template <class T> constexpr bool operator<=(const Option<T>& x, const T& v)
{
    return bool(x) ? x.val() <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const Option<T>& x)
{
    return bool(x) ? v >= x.val() : true;
}

// Comparison of Option<T&> with T
template <class T> constexpr bool operator==(const Option<T&>& x, const T& v)
{
    return bool(x) ? x.val() == v : false;
}

template <class T> constexpr bool operator==(const T& v, const Option<T&>& x)
{
    return bool(x) ? v == x.val() : false;
}

template <class T> constexpr bool operator!=(const Option<T&>& x, const T& v)
{
    return bool(x) ? x.val() != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const Option<T&>& x)
{
    return bool(x) ? v != x.val() : true;
}

template <class T> constexpr bool operator<(const Option<T&>& x, const T& v)
{
    return bool(x) ? x.val() < v : true;
}

template <class T> constexpr bool operator>(const T& v, const Option<T&>& x)
{
    return bool(x) ? v > x.val() : true;
}

template <class T> constexpr bool operator>(const Option<T&>& x, const T& v)
{
    return bool(x) ? x.val() > v : false;
}

template <class T> constexpr bool operator<(const T& v, const Option<T&>& x)
{
    return bool(x) ? v < x.val() : false;
}

template <class T> constexpr bool operator>=(const Option<T&>& x, const T& v)
{
    return bool(x) ? x.val() >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const Option<T&>& x)
{
    return bool(x) ? v <= x.val() : false;
}

template <class T> constexpr bool operator<=(const Option<T&>& x, const T& v)
{
    return bool(x) ? x.val() <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const Option<T&>& x)
{
    return bool(x) ? v >= x.val() : true;
}

// Comparison of Option<T const&> with T
template <class T> constexpr bool operator==(const Option<const T&>& x, const T& v)
{
    return bool(x) ? x.val() == v : false;
}

template <class T> constexpr bool operator==(const T& v, const Option<const T&>& x)
{
    return bool(x) ? v == x.val() : false;
}

template <class T> constexpr bool operator!=(const Option<const T&>& x, const T& v)
{
    return bool(x) ? x.val() != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const Option<const T&>& x)
{
    return bool(x) ? v != x.val() : true;
}

template <class T> constexpr bool operator<(const Option<const T&>& x, const T& v)
{
    return bool(x) ? x.val() < v : true;
}

template <class T> constexpr bool operator>(const T& v, const Option<const T&>& x)
{
    return bool(x) ? v > x.val() : true;
}

template <class T> constexpr bool operator>(const Option<const T&>& x, const T& v)
{
    return bool(x) ? x.val() > v : false;
}

template <class T> constexpr bool operator<(const T& v, const Option<const T&>& x)
{
    return bool(x) ? v < x.val() : false;
}

template <class T> constexpr bool operator>=(const Option<const T&>& x, const T& v)
{
    return bool(x) ? x.val() >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const Option<const T&>& x)
{
    return bool(x) ? v <= x.val() : false;
}

template <class T> constexpr bool operator<=(const Option<const T&>& x, const T& v)
{
    return bool(x) ? x.val() <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const Option<const T&>& x)
{
    return bool(x) ? v >= x.val() : true;
}

template<typename T>
inline auto Some(T&& value) -> Option<typename std::decay<T>::type> {
    return Option<typename std::decay<T>::type>{std::forward<typename std::decay<T>::type>(value)};
}

}

#endif /* end of include guard: __RXX_OPTION_DEF_HPP__ */

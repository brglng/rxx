#ifndef __RXX_OPTION_DEF_HPP__
#define __RXX_OPTION_DEF_HPP__

#include <cstdio>
#include <cassert>
#include "rxx/rxx::invoke.hpp"
#include "rxx/type_traits.hpp"
#include "rxx/utility.hpp"
#include "rxx/str.hpp"
#include "rxx/option/proto.hpp"
#include "rxx/result/proto.hpp"

namespace rxx {

namespace option {
namespace impl {

constexpr struct TrivialInit {} trivial_init {};

template<class T>
union Storage
{
    unsigned char m_dummy;
    T m_value;

    constexpr Storage(TrivialInit) noexcept : m_dummy{} {};

    template<class... Args>
    constexpr Storage(Args&&... args) : m_value(rxx:forward<Args>(args)...) {}

    ~Storage(){}
};

template<class T>
union ConstExprStorage
{
    unsigned char m_dummy;
    T m_value;

    constexpr ConstExprStorage(TrivialInit) noexcept : m_dummy{} {};

    template <class... Args>
    constexpr ConstExprStorage(Args&&... args) : m_value(rxx:forward<Args>(args)...) {}

    ~ConstExprStorage() = default;
};

template <class T>
struct OptionBase
{
    bool m_inited;
    Storage<T> m_storage;

    constexpr OptionBase() noexcept : m_inited(false), m_storage(trivial_init) {};

    explicit constexpr OptionBase(const T& v) : m_inited(true), m_storage(v) {}

    explicit constexpr OptionBase(T&& v) : m_inited(true), m_storage(static_move(v)) {}

    template<class... Args>
    explicit OptionBase(in_place_t, Args&&... args)
        : m_inited(true), m_storage(rxx:forward<Args>(args)...) {}

    template <class U, class... Args, RXX_REQUIRES(std::is_constructible<T, std::initializer_list<U>>)>
    explicit OptionBase(in_place_t, std::initializer_list<U> il, Args&&... args)
        : m_inited(true), m_storage(il, rxx:forward<Args>(args)...) {}

    ~OptionBase() { if (m_inited) m_storage.m_value.T::~T(); }
};


template <class T>
struct ConstExprOptionBase
{
    bool m_inited;
    ConstExprStorage<T> m_storage;

    constexpr ConstExprOptionBase() noexcept : m_inited(false), m_storage(trivial_init) {};

    explicit constexpr ConstExprOptionBase(const T& v) : m_inited(true), m_storage(v) {}

    explicit constexpr ConstExprOptionBase(T&& v) : m_inited(true), m_storage(static_move(v)) {}

    template<class... Args>
    explicit constexpr ConstExprOptionBase(in_place_t, Args&&... args)
        : m_inited(true), m_storage(rxx:forward<Args>(args)...) {}

    template<class U, class... Args, RXX_REQUIRES(std::is_constructible<T, std::initializer_list<U>>)>
    explicit constexpr ConstExprOptionBase(in_place_t, std::initializer_list<U> il, Args&&... args)
        : m_inited(true), m_storage(il, rxx:forward<Args>(args)...) {}

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

template<typename T>
class Option : private option::impl::Base<T> {
    static_assert(!std::is_same<typename std::decay<T>::type, option::None>::value, "bad T");
    static_assert(!std::is_same<typename std::decay<T>::type, in_place_t>::value, "bad T");

    constexpr bool inited() const noexcept {
        return option::impl::Base<T>::m_inited;
    }

    void set_inited(bool inited) noexcept {
        option::impl::Base<T>::m_inited = inited;
    }

    auto valptr() noexcept -> typename std::remove_const<T>::type* {
        return rxx::addressof(option::impl::Base<T>::m_storage.m_value);
    }

    constexpr auto valptr() const noexcept -> T const* {
        return rxx::addressof(option::impl::Base<T>::m_storage.m_value);
    }

    constexpr const T& val() const& {
        return option::impl::Base<T>::m_storage.m_value;
    }

    T&& val() && {
        return static_move(option::impl::Base<T>::m_storage.m_value);
    }

    T& val() & {
        return option::impl::Base<T>::m_storage.m_value;
    }

    template<class... Args>
    void init(Args&&... args) noexcept(noexcept(T(rxx:forward<Args>(args)...))) {
        assert(!inited());
        ::new (static_cast<void*>(valptr())) T(rxx:forward<Args>(args)...);
        set_inited(true);
    }

    template<class U, class... Args>
    void init(std::initializer_list<U> il, Args&&... args) noexcept(noexcept(T(il, rxx:forward<Args>(args)...))) {
        assert(!inited());
        ::new (static_cast<void*>(valptr())) T(il, rxx:forward<Args>(args)...);
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
    constexpr Option(option::None) noexcept : option::impl::Base<T>{} {}

    Option(Option const& that) noexcept(std::is_nothrow_copy_constructible<T>::value)
        : option::impl::Base<T>() {
        if (that.inited()) {
            ::new (static_cast<void*>(valptr())) T(that.val());
            set_inited(true);
        }
    }

    Option(Option&& that) noexcept(std::is_nothrow_move_constructible<T>::value)
        : option::impl::Base<T>() {
        if (that.inited()) {
            ::new (static_cast<void*>(valptr())) T(static_move(that.val()));
            set_inited(true);
        }
    }

    explicit constexpr Option(T const& value) noexcept(std::is_nothrow_copy_constructible<T>::value)
        : option::impl::Base<T>(value) {}

    explicit constexpr Option(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
        : option::impl::Base<T>(static_move(value)) {}

    template<class... Args>
    explicit constexpr Option(in_place_t, Args&&... args)
        : option::impl::Base<T>(in_place_t(), rxx:forward<Args>(args)...) {}

    template<class U, class... Args, RXX_REQUIRES(std::is_constructible<T, std::initializer_list<U>>)>
    explicit constexpr Option(in_place_t, std::initializer_list<U> il, Args&&... args)
        : option::impl::Base<T>(in_place_t(), il, rxx:forward<Args>(args)...) {}

    ~Option() = default;

    auto operator=(option::None) noexcept -> Option& {
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
        else if (!inited() && that.inited())    init(static_move(that.val()));
        else if (inited() && that.inited())     val() = static_move(that.val());
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
        return static_move(val());
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
        return static_move(val());
    }

    auto unwrap() const -> T {
        assert(inited());
        return val();
    }

    auto unwrap_or(T&& def) noexcept -> T {
        if (inited()) {
            return static_move(val());
        } else {
            return static_move(def);
        }
    }

    auto unwrap_or(T const& def) noexcept -> T {
        if (inited()) {
            return static_move(val());
        } else {
            return def;
        }
    }

    auto unwrap_or(T&& def) const noexcept -> T {
        if (inited()) {
            return val();
        } else {
            return static_move(def);
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
            return static_move(val());
        } else {
            return rxx::invoke(rxx:forward<F>(f));
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& f) const -> T {
        if (inited()) {
            return val();
        } else {
            return rxx::invoke(rxx:forward<F>(f));
        }
    }

    template<typename F>
    enable_if_t<!std::is_void<invoke_result_t<decay_t<F>, T&>>::value,
                Option<invoke_result_t<decay_t<F>, T&>>>
    map(F&& f) {
        if (inited()) {
            return Some(rxx::invoke(rxx:forward<F>(f), val()));
        } else {
            return None;
        }
    }

    template<typename F>
    enable_if_t<std::is_void<invoke_result_t<decay_t<F>, T&>>::value, Option<void>>
    map(F&& f) {
        if (inited()) {
            rxx::invoke(rxx:forward<F>(f), val());
            return None;
        } else {
            return None;
        }
    }

    template<typename F>
    enable_if_t<!std::is_void<invoke_result_t<decay_t<F>, T const&>>::value,
                Option<invoke_result_t<decay_t<F>, T const&>>>
    map(F&& f) const {
        if (inited()) {
            return Some(rxx::invoke(rxx:forward<F>(f), val()));
        } else {
            return None;
        }
    }

    template<typename F>
    enable_if_t<std::is_void<invoke_result_t<decay_t<F>, T const&>>::value, Option<void>>
    map(F&& f) const {
        if (inited()) {
            rxx::invoke(rxx:forward<F>(f), val());
            return None;
        } else {
            return None;
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F&& f) -> U {
        if (inited()) {
            return rxx::invoke(rxx:forward<F>(f), val());
        } else {
            return rxx:forward<U>(def);
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F&& f) const -> U {
        if (inited()) {
            return rxx::invoke(rxx:forward<F>(f), val());
        } else {
            return rxx:forward<U>(def);
        }
    }

    template<typename D, typename F>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<F>::type, T&>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type
    >::type
    map_or_else(D&& def, F&& f) {
        if (inited()) {
            return rxx::invoke(rxx:forward<F>(f), val());
        } else {
            return rxx::invoke(rxx:forward<D>(def));
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
            return rxx::invoke(rxx:forward<F>(f), val());
        } else {
            return rxx::invoke(rxx:forward<D>(def));
        }
    }

    template<typename E>
    auto ok_or(E&& err) noexcept -> Result<T, E>;

    template<typename E>
    auto ok_or(E&& err) const noexcept -> Result<T, E>;

    template<typename F>
    enable_if_t<!std::is_void<invoke_result_t<decay_t<F>>>::value,
                Result<T, invoke_result_t<decay_t<F>>>>
    ok_or_else(F&& err);

    template<typename F>
    enable_if_t<std::is_void<invoke_result_t<decay_t<F>>>::value, Result<T, void>>
    ok_or_else(F&& err);

    template<typename F>
    enable_if_t<!std::is_void<invoke_result_t<decay_t<F>>>::value,
                Result<T, invoke_result_t<decay_t<F>>>>
    ok_or_else(F&& err) const;

    template<typename F>
    enable_if_t<std::is_void<invoke_result_t<decay_t<F>>>::value, Result<T, void>>
    ok_or_else(F&& err) const;

    template<typename U>
    auto and_(Option<U> optb) const -> Option<U> {
        if (!inited()) return None;
        else return optb;
    }

    template<typename F>
    auto and_then(F&& f) const -> Option<typename invoke_result<typename std::decay<F>::type>::type> {
        if (!inited()) return None;
        else return rxx::invoke(rxx:forward<F>(f));
    }

    template<typename P>
    auto filter(P&& predicate) -> Option<T> {
        if (!inited()) {
            return None;
        } else {
            if (rxx::invoke(rxx:forward<P>(predicate), val())) {
                return static_move(*this);
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
            if (rxx::invoke(rxx:forward<P>(predicate), val())) {
                return *this;
            } else {
                return None;
            }
        }
    }

    auto or_(Option<T> optb) -> Option<T> {
        if (inited()) {
            return static_move(*this);
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
            return static_move(*this);
        } else {
            return rxx::invoke(rxx:forward<F>(f));
        }
    }

    template<typename F>
    auto or_else(F&& f) const -> Option<T> {
        if (inited()) {
            return *this;
        } else {
            return rxx::invoke(rxx:forward<F>(f));
        }
    }

    auto xor_(Option<T> optb) -> Option<T> {
        if (inited() && !optb.inited()) {
            return static_move(*this);
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
            init(static_move(v));
        } else {
            return val();
        }
    }

    template<class F>
    auto get_or_insert_with(F&& f) -> T& {
        if (!inited()) {
            init(rxx::invoke(rxx:forward<F>(f)));
        } else {
            return val();
        }
    }

    auto take() -> Option<T> {
        if (inited()) {
            set_inited(false);
            return Some(static_move(val()));
        } else {
            return None;
        }
    }

    auto replace(T const& value) -> Option<T> {
        if (inited()) {
            auto old_value = static_move(val());
            val() = value;
            return Some(static_move(old_value));
        } else {
            init(value);
            return None;
        }
    }

    auto replace(T&& value) -> Option<T> {
        if (inited()) {
            auto old_value = static_move(val());
            val() = static_move(value);
            return Some(static_move(old_value));
        } else {
            init(static_move(value));
            return None;
        }
    }

    auto unwrap_or_default() -> T {
        if (inited()) {
            return static_move(val());
        } else {
            return T {};
        }
    }

    constexpr bool operator==(const Option& rhs) const {
        return inited() != rhs.inited() ? false : inited() == false ? true : val() == rhs.val();
    }

    constexpr bool operator<(const Option& rhs) const {
        return !inited() ? false : !rhs.inited() ? true : val() < rhs.val();
    }
};

template<>
class Option<void> {
public:
    constexpr Option() noexcept {}
    constexpr Option(option::None) noexcept {}

    Option(Option const&) noexcept {}

    Option(Option&&) noexcept {}

    explicit constexpr Option(in_place_t) noexcept {}

    ~Option() = default;

    auto operator=(option::None) noexcept -> Option& { return *this; }

    auto operator=(Option const&) noexcept -> Option& { return *this; }

    auto operator=(Option&&) -> Option& { return *this; }

    explicit constexpr operator bool() const noexcept { return false; }
    
    constexpr auto is_some() const noexcept -> bool { return false; }

    constexpr auto is_none() const noexcept -> bool { return true; }

    void expect(Str msg) const noexcept {
        std::fprintf(stderr, "%s\n", msg.c_str());
        std::terminate();
    }

    void unwrap_or() const noexcept {}

    template<typename U, typename F>
    auto map_or(U&& def) const noexcept -> U {
        return rxx:forward<U>(def);
    }

    template<typename D, typename F>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<F>::type>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type
    >::type
    map_or_else(D&& def) const {
        return rxx::invoke(rxx:forward<D>(def));
    }

    template<typename E>
    auto ok_or(E&& err) noexcept -> Result<void, E>;

    template<typename E>
    auto ok_or(E&& err) const noexcept -> Result<void, E>;

    template<typename F>
    auto ok_or_else(F&& err) -> Result<void, typename invoke_result<typename std::decay<F>::type>::type>;

    template<typename F>
    auto ok_or_else(F&& err) const -> Result<void, typename invoke_result<typename std::decay<F>::type>::type>;

    template<typename U>
    auto and_(Option<U>) const -> Option<U> {
        return None;
    }

    auto or_(Option<void> optb) const -> Option<void> { return optb; }

    template<typename F>
    auto or_else(F&& f) const -> Option<void> {
        return rxx::invoke(rxx:forward<F>(f));
    }

    auto xor_(Option<void>) const -> Option<void> {
        return None;
    }

    constexpr bool operator==(const Option&) const { return true; }
    constexpr bool operator<(const Option&) const { return false; }
};

template<typename T>
class Option<T&> {
    static_assert(!std::is_same<T, option::None>::value, "bad T" );
    static_assert(!std::is_same<T, in_place_t>::value, "bad T" );

    T* m_ptr;

    constexpr bool inited() const noexcept { return m_ptr != nullptr; }
    T& val() noexcept { return *m_ptr; }
    constexpr T const& val() const noexcept { return *m_ptr; }

public:
    constexpr Option() noexcept  : m_ptr{nullptr} {}

    constexpr Option(option::None) noexcept : m_ptr{nullptr} {}

    explicit constexpr Option(T& t) noexcept : m_ptr{rxx::addressof(t)} {}

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

    auto operator=(option::None) noexcept -> Option& {
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
        return static_move(val());
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
            return rxx::invoke(rxx:forward<F>(f));
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& f) const -> T const& {
        if (inited()) {
            return val();
        } else {
            return rxx::invoke(rxx:forward<F>(f));
        }
    }

    template<typename F>
    enable_if_t<!std::is_void<invoke_result_t<decay_t<F>, T&>>::value,
                Option<invoke_result_t<decay_t<F>, T&>>>
    map(F&& f) {
        if (inited()) {
            return Some(rxx::invoke(rxx:forward<F>(f), val()));
        } else {
            return None;
        }
    }

    template<typename F>
    enable_if_t<std::is_void<invoke_result_t<decay_t<F>, T&>>::value, Option<void>>
    map(F&& f) {
        if (inited()) {
            rxx::invoke(rxx:forward<F>(f), val());
            return None;
        } else {
            return None;
        }
    }

    template<typename F>
    enable_if_t<!std::is_void<invoke_result_t<decay_t<F>, T const&>>::value,
                Option<invoke_result_t<decay_t<F>, T&>>>
    map(F&& f) const {
        if (inited()) {
            return Some(rxx::invoke(rxx:forward<F>(f), val()));
        } else {
            return None;
        }
    }

    template<typename F>
    enable_if_t<std::is_void<invoke_result_t<decay_t<F>, T const&>>::value, Option<void>>
    map(F&& f) const {
        if (inited()) {
            rxx::invoke(rxx:forward<F>(f), val());
            return None;
        } else {
            return None;
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F&& f) -> U {
        if (inited()) {
            return rxx::invoke(rxx:forward<F>(f), static_move(val()));
        } else {
            return rxx:forward<U>(def);
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F&& f) const -> U {
        if (inited()) {
            return rxx::invoke(rxx:forward<F>(f), val());
        } else {
            return rxx:forward<U>(def);
        }
    }

    template<typename D, typename F>
    typename std::enable_if<
        std::is_same<
            typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type,
            typename std::decay<typename invoke_result<typename std::decay<F>::type, T&>::type>::type
        >::value,
        typename std::decay<typename invoke_result<typename std::decay<D>::type>::type>::type
    >::type
    map_or_else(D&& def, F&& f) {
        if (inited()) {
            return rxx::invoke(rxx:forward<F>(f), val());
        } else {
            return rxx::invoke(rxx:forward<D>(def));
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
            return rxx::invoke(rxx:forward<F>(f), val());
        } else {
            return rxx::invoke(rxx:forward<D>(def));
        }
    }

    // template<typename E>
    // auto ok_or(E&& err) noexcept -> Result<T&, E> {
    //     if (inited()) {
    //         return Ok(static_move(val()));
    //     } else {
    //         return Err(std::rxx:forward<E>(err));
    //     }
    // }

    // template<typename F>
    // auto ok_or_else(F&& err) -> Result<T&, typename invoke_result<typename std::decay<F>::type>::type> {
    //     if (inited()) {
    //         return Ok(static_move(val()));
    //     } else {
    //         return Err(rxx::invoke(std::rxx:forward<F>(err)));
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
        else return rxx::invoke(rxx:forward<F>(f));
    }

    template<typename P>
    auto filter(P&& predicate) -> Option<T&> {
        if (!inited()) {
            return None;
        } else {
            if (rxx::invoke(rxx:forward<P>(predicate), val())) {
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
            if (rxx::invoke(rxx:forward<P>(predicate), val())) {
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
            return rxx::invoke(rxx:forward<F>(f));
        }
    }

    template<typename F>
    auto or_else(F&& f) const -> Option<T const&> {
        if (inited()) {
            return *this;
        } else {
            return rxx::invoke(rxx:forward<F>(f));
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
            m_ptr = rxx::addressof(rxx::invoke(rxx:forward<F>(f)));
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
            m_ptr = rxx::addressof(value);
            return Option<T&>(*old_ptr);
        } else {
            init(value);
            return None;
        }
    }

    constexpr bool operator==(const Option& rhs) const {
        return inited() != rhs.inited() ? false : inited() == false ? true : val() == rhs.val();
    }

    constexpr bool operator<(const Option& rhs) const {
        return !inited() ? false : !rhs.inited() ? true : val() < rhs.val();
    }
};

template <class T>
class Option<T&&>
{
    static_assert(sizeof(T) == 0, "Option rvalue references disallowed");
};

// Relational operators
template <class T> constexpr bool operator!=(const Option<T>& x, const Option<T>& y)
{
    return !(x == y);
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

// Comparison with None
template <class T> constexpr bool operator==(const Option<T>& x, option::None) noexcept
{
    return (!x);
}

template <class T> constexpr bool operator==(option::None, const Option<T>& x) noexcept
{
    return (!x);
}

template <class T> constexpr bool operator!=(const Option<T>& x, option::None) noexcept
{
    return bool(x);
}

template <class T> constexpr bool operator!=(option::None, const Option<T>& x) noexcept
{
    return bool(x);
}

template <class T> constexpr bool operator<(const Option<T>&, option::None) noexcept
{
    return false;
}

template <class T> constexpr bool operator<(option::None, const Option<T>& x) noexcept
{
    return bool(x);
}

template <class T> constexpr bool operator<=(const Option<T>& x, option::None) noexcept
{
    return (!x);
}

template <class T> constexpr bool operator<=(option::None, const Option<T>&) noexcept
{
    return true;
}

template <class T> constexpr bool operator>(const Option<T>& x, option::None) noexcept
{
    return bool(x);
}

template <class T> constexpr bool operator>(option::None, const Option<T>&) noexcept
{
    return false;
}

template <class T> constexpr bool operator>=(const Option<T>&, option::None) noexcept
{
    return true;
}

template <class T> constexpr bool operator>=(option::None, const Option<T>& x) noexcept
{
    return (!x);
}

template<typename T>
inline auto Some(T&& value) -> Option<rxx::remove_reference_t<T>> {
    return Option<rxx::remove_reference_t<T>>(rxx:forward<T>(value));
}

}

#endif /* end of include guard: __RXX_OPTION_DEF_HPP__ */

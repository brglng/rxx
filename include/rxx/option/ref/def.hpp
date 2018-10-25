#ifndef __RXX_OPTION_REF_DEF_HPP__
#define __RXX_OPTION_REF_DEF_HPP__

#include <memory>
#include "rxx/option/none.hpp"
#include "rxx/macros.hpp"

namespace rxx {

class Bool;
class conststr;
template<typename T, typename E> class Result;
template<typename T> class Option;

template<typename T>
class Option<T&> {
    T* m_ptr;

public:
    explicit Option(T& t) : m_ptr { std::addressof(t) } {}

    Option(option::impl::None const&) : m_ptr { nullptr } {}

    Option(Option&& that) : m_ptr { that.m_ptr } {
        that.m_ptr = nullptr;
    }

    auto operator=(Option&& that) -> Option& {
        if (this != &that) {
            m_ptr = that.m_ptr;
            that.m_ptr = nullptr;
        }
        return *this;
    }

    auto operator=(option::impl::None const&) -> Option& {
        m_ptr = nullptr;
        return *this;
    }

    auto operator=(T& that) -> Option& {
        m_ptr = std::addressof(that);
        return *this;
    }

    auto is_some() const -> Bool;
    auto is_none() const -> Bool;

    auto as_ref() -> Option<T&> {
        return Option<T&> { *m_ptr };
    }

    auto as_const() const -> Option<T const&> {
        return Option<T const&> { *m_ptr };
    }

    auto expect(conststr msg) -> T&;

    auto unwrap() -> T& {
        RXX_ASSERT(is_some());
        return *m_ptr;
    }

    auto unwrap_or(T& def) noexcept -> T& {
        if (is_some()) {
            return *m_ptr;
        } else {
            return def;
        }
    }

    template<typename F>
    auto unwrap_or_else(F&& f) noexcept -> T& {
        if (is_some()) {
            return *m_ptr;
        } else {
            return std::forward<F>(f)();
        }
    }

    template<typename U, typename F>
    auto map(F&& f) noexcept -> Option<U> {
        if (is_some()) {
            return Some(std::move(std::forward<F>(f)(*m_ptr)));
        } else {
            return None;
        }
    }

    template<typename U, typename F>
    auto map_or(U&& def, F&& f) noexcept -> U&& {
        if (is_some()) {
            return std::forward<F>(f)(*m_ptr);
        } else {
            return std::forward<U>(def);
        }
    }

    template<typename U, typename D, typename F>
    auto map_or_else(D&& def, F&& f) noexcept -> U {
        if (is_some()) {
            return std::forward<F>(f)(*m_ptr);
        } else {
            return std::forward<U>(std::forward<D>(def)());
        }
    }

    template<typename E>
    auto ok_or(E&& err) -> Result<T&, E>;

    auto operator==(Option const& that) const -> Bool;
    auto operator==(option::impl::None const&) const -> Bool;
};

}

#endif /* end of include guard: __RXX_OPTION_REF_DEF_HPP__ */

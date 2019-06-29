#ifndef __RXX_MATCH_HPP__
#define __RXX_MATCH_HPP__

#include "rxx/utility.hpp"

namespace rxx {

template<typename... Ts> struct Overloaded;

template<> struct Overloaded<> { void operator()() const; };

template<typename T> struct Overloaded<T> : T {
    constexpr Overloaded(T const& t) : T(t) {}
    constexpr Overloaded(T&& t) : T(rxx::move(t)) {}
    using T::operator();
};

template<typename T, typename... Ts>
struct Overloaded<T, Ts...> : T, Overloaded<Ts...> {
    constexpr Overloaded(T const& t, Ts const&... ts) : T(t), Overloaded<Ts...>(ts...) {}
    constexpr Overloaded(T&& t, Ts&&... ts) :
        T(rxx::move(t)),
        Overloaded<Ts...>(rxx::move(ts)...)
    {}

    using T::operator();
    using Overloaded<Ts...>::operator();
};

template<typename... Ts>
inline constexpr auto overloaded(Ts&&... ts) -> Overloaded<Ts...> {
    return Overloaded<Ts...>(rxx::forward<Ts>(ts)...);
}

#define RXX_MATCH(v, ...) rxx::visit(overloaded(__VA_ARGS__), v)

} // namespace rxx

#endif /* end of include guard: __RXX_MATCH_HPP__ */

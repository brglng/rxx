#ifndef __RXX_PANIC_IMPL_HPP__
#define __RXX_PANIC_IMPL_HPP__

#include <exception>
#include <cstdio>
#include <cstdlib>
#include <fmt/core.h>
#include "rxx/as/def.hpp"
#include "rxx/panic/def.hpp"
#include "rxx/str/def.hpp"

namespace rxx {

/* should update to our new API after the wrapper around fmtlib is finished */
template<typename... Args>
inline void panic(conststr msg, Args const&... args) {
    std::fputs("panicked at ", stderr);
    ::fmt::print(stderr,
                 ::fmt::string_view {
                     reinterpret_cast<char const*>(
                         msg.as_bytes_const().as_const_ptr()),
                     as<std::size_t>(msg.len())
                 },
                 args...);
    std::terminate();
}

inline void panic() {
    std::fputs("panicked", stderr);
    std::terminate();
}

}

#endif /* end of include guard: __RXX_CORE_PANIC_HPP__ */

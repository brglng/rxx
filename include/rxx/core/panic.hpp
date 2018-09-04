#ifndef __RXX_PANIC_HPP__
#define __RXX_PANIC_HPP__

#include <exception>
#include <cstdio>
#include <cstdlib>
#include <fmt/core.h>
#include "rxx/core/str.hpp"

namespace rxx {

/* should update to our new API after the wrapper around fmtlib is finished */
template<typename... Args>
void panic(const str msg, Args const&... args) {
    ::std::fputs("panicked at ", stderr);
    ::fmt::print(stderr,
                 ::fmt::string_view {
                     reinterpret_cast<char const*>(
                         msg.as_bytes_const().as_const_ptr()),
                     msg.len().as_size_t()
                 },
                 args...);
    ::std::terminate();
}

void panic() {
    ::std::fputs("panicked", stderr);
    ::std::terminate();
}

}

#endif /* end of include guard: __RXX_PANIC_HPP__ */

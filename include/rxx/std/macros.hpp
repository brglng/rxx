#ifndef __RXX_STD_MACROS_HPP__
#define __RXX_STD_MACROS_HPP__

#include "rxx/core/panic.hpp"

#define RXX_STRINGIFY(__x) #__x

#define RXX_ASSERT(__cond, ...) do {    \
    if (!(__cond)) {                    \
        ::std::fputs(__FILE__ ":" RXX_STRINGIFY(__LINE__) ": Assertion `" RXX_STRINGIFY(__cond) "' failed.\n", stderr); \
        ::rxx::panic(__VA_ARGS__);      \
    }                                   \
} while (0)

#endif /* end of include guard: __RXX_STD_MACROS_HPP__ */

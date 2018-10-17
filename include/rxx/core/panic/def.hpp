#ifndef __RXX_COER_PANIC_DEF_HPP__
#define __RXX_COER_PANIC_DEF_HPP__

namespace rxx {

class str;

template<typename... Args>
inline void panic(const str msg, Args const&... args);

inline void panic();

}

#endif /* end of include guard: __RXX_COER_PANIC_DEF_HPP__ */

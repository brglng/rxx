#ifndef __RXX_PANIC_DEF_HPP__
#define __RXX_PANIC_DEF_HPP__

namespace rxx {

class conststr;

template<typename... Args>
inline void panic(conststr msg, Args const&... args);

inline void panic();

}

#endif /* end of include guard: __RXX_PANIC_DEF_HPP__ */

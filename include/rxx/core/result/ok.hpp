#ifndef __RXX_CORE_RESULT_OK_HPP__
#define __RXX_CORE_RESULT_OK_HPP__

#include <utility>

namespace rxx {
namespace result {
namespace impl {

template<typename T>
struct Ok {
    T m_ok;
};

}
}
}

#endif /* end of include guard: __RXX_CORE_RESULT_OK_HPP__ */

#ifndef __RXX_CORE_RESULT_ERR_HPP__
#define __RXX_CORE_RESULT_ERR_HPP__

#include <utility>

namespace rxx {
namespace result {
namespace impl {

template<typename E>
struct Err {
    E m_err;
};

}
}
}

#endif /* end of include guard: __RXX_CORE_RESULT_ERR_HPP__ */

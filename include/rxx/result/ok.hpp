#ifndef __RXX_RESULT_OK_HPP__
#define __RXX_RESULT_OK_HPP__

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

#endif /* end of include guard: __RXX_RESULT_OK_HPP__ */

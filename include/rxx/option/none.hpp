#ifndef __RXX_OPTION_NONE_HPP__
#define __RXX_OPTION_NONE_HPP__

namespace rxx {

namespace option {
namespace impl {

struct None {
    explicit constexpr None(int) {}
};

}
}

constexpr option::impl::None None{0};

}

#endif /* end of include guard: __RXX_OPTION_NONE_HPP__ */

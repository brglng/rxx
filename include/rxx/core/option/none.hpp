#ifndef __RXX_CORE_OPTION_NONE_HPP__
#define __RXX_CORE_OPTION_NONE_HPP__

namespace rxx {

struct OptionNone {
  explicit constexpr OptionNone(int) {}
};
constexpr OptionNone None{0};

}

#endif /* end of include guard: __RXX_CORE_OPTION_NONE_HPP__ */

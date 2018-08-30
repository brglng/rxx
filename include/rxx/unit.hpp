#ifndef __RXX_UNIT_HPP__
#define __RXX_UNIT_HPP__

namespace rxx {

class unit {
public:
  unit() {}
  unit(unit const&) {}
  auto operator=(unit const&) -> unit& {
    return *this;
  }
};

}

#endif /* end of include guard: __RXX_UNIT_HPP__ */

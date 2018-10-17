#ifndef __RXX_STR_IMPL_HPP__
#define __RXX_STR_IMPL_HPP__

#include "rxx/core/bool/def.hpp"
#include "rxx/core/str/def.hpp"

namespace rxx {

constexpr auto str::is_empty() const -> Bool {
    return m_bytes.len() == usize{0};
}

}

#include "rxx/core/str/const/impl.hpp"

#endif /* end of include guard: __RXX_STR_IMPL_HPP__ */

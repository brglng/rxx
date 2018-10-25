#ifndef __RXX_STR_CONST_IMPL_HPP__
#define __RXX_STR_CONST_IMPL_HPP__

#include "rxx/bool/def.hpp"
#include "rxx/str/const/def.hpp"
#include "rxx/str/def.hpp"

namespace rxx {

constexpr conststr::conststr(str s) :
    m_bytes { s.as_bytes_const() }
{}

constexpr auto conststr::is_empty() const -> Bool {
    return m_bytes.len() == 0;
}

}

#endif /* end of include guard: __RXX_STR_IMPL_HPP__ */

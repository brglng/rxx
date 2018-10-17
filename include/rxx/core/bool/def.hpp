#ifndef __RXX_CORE_BOOL_DEF_HPP__
#define __RXX_CORE_BOOL_DEF_HPP__

namespace rxx {

class Bool {
    unsigned char m_value;

public:
    constexpr Bool(bool value) noexcept : m_value{value} {}

    constexpr operator bool() const noexcept {
        return m_value;
    }

    auto constexpr operator==(Bool const& that) const noexcept -> Bool {
        return this->m_value == that.m_value;
    }

    auto constexpr operator!=(Bool const& that) const noexcept -> Bool {
        return this->m_value != that.m_value;
    }

    auto constexpr operator&&(Bool const& that) const noexcept -> Bool {
        return this->m_value && that.m_value;
    }

    auto constexpr operator||(Bool const& that) const noexcept -> Bool {
        return this->m_value || that.m_value;
    }
};

}

#endif /* end of include guard: __RXX_CORE_BOOL_DEF_HPP__ */

#ifndef __RXX_IO_SEEK_FROM_HPP__
#define __RXX_IO_SEEK_FROM_HPP__

#include <cstdint>
#include "rxx/var.hpp"

namespace rxx {
namespace io {

class SeekFrom {
public:
    class Start {
        std::uint64_t m_offset;
    public:
        explicit constexpr Start(std::uint64_t offset) : m_offset(offset) {}
        constexpr std::uint64_t offset() const { return m_offset; }
    };

    class End {
        std::int64_t m_offset;
    public:
        explicit constexpr End(std::int64_t offset) : m_offset(offset) {}
        constexpr std::int64_t offset() const { return m_offset; }
    };

    class Current {
        std::int64_t m_offset;
    public:
        explicit constexpr Current(std::int64_t offset) : m_offset(offset) {}
        constexpr std::int64_t offset() const { return m_offset; }
    };

    SeekFrom(Start s) : m_var(rxx::move(s)) {}
    SeekFrom(End e) : m_var(rxx::move(e)) {}
    SeekFrom(Current c) : m_var(rxx::move(c)) {}

private:
    rxx::Var<Start, End, Current> m_var;

public:
    rxx::Var<Start, End, Current>& as_var() & { return m_var; }
    constexpr rxx::Var<Start, End, Current> const& as_var() const& { return m_var; }
    rxx::Var<Start, End, Current>&& as_var() && { return rxx::move(m_var); }
};

} // namespace io
} // namespace rxx

#endif /* end of include guard: __RXX_IO_SEEK_FROM_HPP__ */

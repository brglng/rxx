# rxx

`rxx` is a C++ library who aims to bring Rust goodies to C++.

## Prerequisites

- Compiler with C++11 support
- I/O related modules only support Unix-like OSes (Windows support can be added in the future)

## Differences from Rust

- All types are by default mutable.
- STL style iterators are provided to be compatible with `for (auto&& elem : container)` syntax.
- Macros are in all upper case and prefixed with `RXX_`.
- Names of all types are in CamelCase.
- No literal support (can be added in the futrue).

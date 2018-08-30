# rxx
Rust-flavored C++ template library, aiming to be a practical alternative and addition to STL.

## Prerequisites
- Compiler with C++11 support
- [fmtlib](http://fmtlib.net) for formatting support
- Filesystem and network related modules only support Unix-like OSes (Windows support can be added in the future)

## Differences from STL
- None of the types is default constructable.
- STL uses `operator=(T const&)` for copying, and `operator=(T&&)` for move. rxx does not use `operator=(T const&)`, but defines `operator=(T&)`, and it does the same as `operator(T&&)` which performs moving. `.clone()` is used for copying.
- rxx does not use exceptions. Instead, it uses `Option<T>` and `Result<T, E>`.
- rxx does not use `constexpr` or `noexcept`.

## Differences from Rust
- All types are by default mutable.
- There is no borrow checker.
- There is no trait (can be added later).
- There is no trait object (can be added later).
- There is no lifetime.
- Macros are in all upper case and prefixed with `RXX_`.
- Names of primitive types are in CamelCase if it conflicts with the name of a C++ built-in type.
- No literal support for primitive types (can be added in the futrue if we update to C++17).

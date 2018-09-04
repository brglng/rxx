# rxx
Rust-flavored C++ template library, aiming to be a practical alternative and addition to STL.

## Prerequisites
- Compiler with C++11 support
- [fmtlib](http://fmtlib.net) for formatting support
- I/O related modules only support Unix-like OSes (Windows support can be added in the future)

## Differences from STL
- None of the types are default constructible. `Option<T>` should be used when you need something that are optional.
- rxx does not use exceptions. Instead, it uses `Option<T>` and `Result<T, E>`.
- rxx does not use `noexcept`. `constexpr` is used only when necessary.

## Differences from Rust
- Rust uses move sementic by default. rxx implements standard copy construtor, move constructor, copy assignment operator and move assignment operator. That means, `std::move()` must be used for moving, otherwise a deep copy is performed.
- All types are by default mutable. Functions like `.as_mut()` in Rust correspond to `.as_const()` in rxx.
- STL style iterators are provided to be compatible with `for (auto&& elem : container)` syntax.
- Macros are in all upper case and prefixed with `RXX_`.
- Names of primitive types are in CamelCase if it conflicts with the name of a C++ built-in type.
- No literal support (can be added in the futrue).
- There is no borrow checker.
- There is no lifetime.
- There is no trait (can be added in the future with the help of [Tick](https://github.com/pfultz2/Tick)).
- There is no trait object (can be added in the future with some template magic).

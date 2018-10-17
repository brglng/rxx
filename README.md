# rxx
Aggresive Rust-flavored C++ template library, aiming to bring Rust experience to C++.

## Prerequisites
- Compiler with C++11 support
- [fmtlib](http://fmtlib.net) for formatting support
- I/O related modules only support Unix-like OSes (Windows support can be added in the future)

## Differences from STL
- None of the types are default constructible. `Option<T>` should be used when you need something that are optional.
- rxx does not implement standard copy construtor or copy assignment operator. Instead, it uses move sementic by default by implementing "mutable reference constructor" and "mutable reference copy assignment operator" (i.e. `T::T(T&)` and `T::operator=(T&)`). Standard move constructor and move assignment operator are also implemented. For non-trivially copyable types, `.clone()` must be used to perform a deep copy, otherwise, copy constructors are only used for trivially copyable types.
- rxx does not use exceptions. Instead, it uses `Option<T>` and `Result<T, E>`.

## Differences from Rust
- All types are by default mutable. Functions like `.as_mut()` in Rust correspond to `.as_const()` in rxx.
- STL style iterators are provided to be compatible with `for (auto&& elem : container)` syntax.
- Macros are in all upper case and prefixed with `RXX_`.
- Names of primitive types are in CamelCase if it conflicts with the name of a C++ built-in type.
- No literal support (can be added in the futrue).
- There is no borrow checker.
- There is no lifetime.
- There is no trait (can be added in the future with the help of [Tick](https://github.com/pfultz2/Tick)).
- There is no trait object (can be added in the future with some template magic).

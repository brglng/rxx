# rxx

`rxx` is a C++ library who aims to bring Rust goodies to C++.

## Prerequisites

- Compiler with C++11 support
- I/O related modules only support Unix-like OSes at the moment

## Differences from Rust

- All types are by default mutable.
- STL style iterators are provided to be compatible with
  `for (auto&& elem : container)` syntax.
- Macros are in all upper case and prefixed with `RXX_`.
- Names of all types are in CamelCase.
- No literal support (can be added in the futrue).

## Usage

`rxx` uses [CMake](https://cmake.org). You can either include `rxx` in your
build tree:

    ```cmake
    add_subdirectory(rxx)
    target_link_library(YourProject PUBLIC Rxx::Rxx)
    ```
 
or install `rxx` somewhere and use `find_package` to find it:

    ```cmake
    find_package(rxx)
    target_link_library(YourProject PUBLIC Rxx::Rxx)
    ```

[modeline]: # ( vim: cc=80 tw=80 )

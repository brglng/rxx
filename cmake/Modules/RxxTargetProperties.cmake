set(rxx_compile_features
  c_variadic_macros
  cxx_alignof
  cxx_attributes
  cxx_auto_type
  cxx_constexpr
  cxx_decltype
  cxx_defaulted_functions
  cxx_defaulted_move_initializers
  cxx_delegating_constructors
  cxx_deleted_functions
  cxx_final
  cxx_func_identifier
  cxx_generalized_initializers
  cxx_lambdas
  cxx_noexcept
  cxx_nullptr
  cxx_override
  cxx_range_for
  cxx_right_angle_brackets
  cxx_rvalue_references
  cxx_static_assert
  cxx_variadic_macros
  cxx_variadic_templates
  )

set(rxx_compile_definitions
  _GNU_SOURCE
  __STDC_FORMAT_MACROS
  __STDC_LIMIT_MACROS
  __STDC_CONSTANT_MACROS
  _POSIX_C_SOURCE=200809L
  )

set(rxx_compile_options "")
include(CheckCXXCompilerFlag)
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "^(GNU|.*Clang)$")
  foreach(flag -Wall
               -Wcast-align
               -Wduplicated-branches
               -Wduplicated-cond
               -Wextra
               -Wformat=2
               -Wmissing-include-dirs
               -Wnarrowing
               -Wpointer-arith
               -Wshadow
               -Wuninitialized
               -Wwrite-strings
               -Werror=c++11-compat
               -Werror=init-self
               -Werror=return-type
               )
    check_cxx_compiler_flag(${flag} rxx_has_cxx_flag_${flag})
    if(rxx_has_cxx_flag_${flag})
      list(APPEND rxx_compile_options ${flag})
    endif()
  endforeach()
endif()

set(rxx_compile_options_release -fomit-frame-pointer)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
  list(APPEND rxx_compile_options_release -march=sandybridge)
endif()

set(rxx_debug_postfix "-debug")
set(rxx_relwithdebinfo_postfix "-relwithdebinfo")

# CMake Options
cmake_minimum_required(VERSION 2.8)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

# Project Infomation
project(rxx CXX)
set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(PROJECT_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR})
set(${PROJECT_NAME}_VERSION_MINOR 0)
set(PROJECT_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR})
set(${PROJECT_NAME}_VERSION_PATCH 0)
set(PROJECT_VERSION_PATCH ${${PROJECT_NAME}_VERSION_PATCH})
set(${PROJECT_NAME}_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_VERSION ${${PROJECT_NAME}_VERSION})

option(CMAKE_BUILD_TYPE "Debug/Release/RelWithDebInfo/MinSizeRel" Release)
option(BUILD_SHARED_LIBS "build shared library instead of static library" OFF)
option(CMAKE_POSITION_INDEPENDENT_CODE "build position independent code" ON)
option(ENABLE_TESTING "enable testing" ON)
if(ENABLE_TESTING)
  enable_testing()
endif()

# Global Compiler Options
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)

add_definitions(-D_GNU_SOURCE -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -D_POSIX_C_SOURCE=200809L)

if(${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror=implicit -std=gnu99")
  if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" AND ${CMAKE_C_COMPILER_VERSION} VERSION_GREATER "6.0.0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wnarrowing -Werror=discarded-qualifiers -Werror=incompatible-pointer-types")
  elseif(${CMAKE_C_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wnarrowing -Werror=ignored-qualifiers -Werror=incompatible-pointer-types")
  endif()
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fomit-frame-pointer -march=native")
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror=implicit -std=gnu++11")
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "6.0.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnarrowing -Werror=discarded-qualifiers -Werror=incompatible-pointer-types")
  elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnarrowing -Werror=ignored-qualifiers -Werror=incompatible-pointer-types")
  endif()
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fomit-frame-pointer -march=native")
endif()

set(CMAKE_C_EXTENSIONS      ON)
set(CMAKE_C_STANDARD        99)
set(CMAKE_CXX_EXTENSIONS    ON)
set(CMAKE_CXX_STANDARD      11)

include(GNUInstallDirs)

install(FILES README.md CHANGELOG.md DESTINATION .)

set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_SOURCE_IGNORE_FILES "/\\\\.git/;\\\\.git.*;/build/;/rapshow_songs/;/python/;/cmake-build-.*/;/\\\\.idea/;/\\\\.ycm_extra_conf\\\\..*;/GPATH$;/GRTAGS$;/GSYMS$;/GTAGS$;\\\\.swp$;\\\\.swo$;\\\\.DS_Store$")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

# Build rules
find_package(fmt REQUIRED)

# Install rules
install(
  FILES include
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

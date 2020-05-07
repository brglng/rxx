# CMake Options
cmake_minimum_required(VERSION 3.14)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()
if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

# Project Infomation
project(rxx VERSION 0.1.0 LANGUAGES C CXX)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
  endif()

  if(NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "enable building of shared libraries instead of static ones" FORCE)
  endif()

  if(NOT CMAKE_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "enable position independent code" FORCE)
  endif()

  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE)
  endif()
endif()

option(RXX_ENABLE_TESTING "enable testing" OFF)

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_POSITION_INDEPENDENT_CODE: ${CMAKE_POSITION_INDEPENDENT_CODE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
message(STATUS "RXX_ENABLE_TESTING: ${RXX_ENABLE_TESTING}")

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  if(RXX_ENABLE_TESTING)
    enable_testing()
  endif()
endif()

set(${PROJECT_NAME}_full_configuration "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}")

include(GNUInstallDirs)
include(rxxTargetProperties)

add_library(rxx INTERFACE)
target_include_directories(rxx INTERFACE
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )
target_compile_features(rxx INTERFACE ${rxx_compile_features})
target_compile_definitions(rxx INTERFACE ${rxx_compile_definitions})
target_link_libraries(rxx INTERFACE
  $<$<PLATFORM_ID:Linux>:rt>
  $<$<PLATFORM_ID:Linux>:dl>
  $<$<PLATFORM_ID:Linux>:pthread>
  $<$<PLATFORM_ID:Linux>:m>
  )
add_library(rxx::rxx ALIAS rxx)

if(RXX_ENABLE_TESTING)
  add_subdirectory(tests)
endif()

export(TARGETS rxx NAMESPACE rxx FILE rxxTargets.cmake)
export(PACKAGE rxx)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  install(FILES README.md CHANGELOG.md LICENSE DESTINATION .)

  install(TARGETS rxx EXPORT rxxTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    )

  install(DIRECTORY include DESTINATION .)

  install(EXPORT rxxTargets
    FILE rxxTargets.cmake
    NAMESPACE rxx::
    DESTINATION lib/cmake/rxx
    )

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    rxxConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

  configure_file(cmake/rxxConfig.cmake.in rxxConfig.cmake @ONLY)
  install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/rxxConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/rxxConfigVersion.cmake"
    DESTINATION lib/cmake/rxx
    )

  include(rxxCPackConfig)
endif()

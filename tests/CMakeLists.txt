include(FetchContent)
FetchContent_Declare(catch
  GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
  GIT_SHALLOW ON
  )
FetchContent_GetProperties(catch)
if(NOT catch_POPULATED)
  message(STATUS "Populating Catch2 from https://github.com/catchorg/Catch2.git...")
  FetchContent_Populate(catch)
  add_subdirectory(${catch_SOURCE_DIR} ${catch_BINARY_DIR})
endif()

add_library(rxx-test-main STATIC main.cpp)

target_compile_features(rxx-test-main PRIVATE ${rxx_compile_features})

target_compile_definitions(rxx-test-main PRIVATE ${rxx_compile_definitions})

target_compile_options(rxx-test-main PRIVATE
  ${rxx_compile_options}
  $<$<CONFIG:RELEASE>:${rxx_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${rxx_compile_options_release}>
  )

target_link_libraries(rxx-test-main PUBLIC
  Catch2::Catch2
  rxx::rxx
  )

add_subdirectory(io)
add_subdirectory(result)
add_subdirectory(slice)
add_subdirectory(str)
add_subdirectory(var)

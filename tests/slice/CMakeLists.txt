add_executable(slice-test
  test.cpp
  )

target_compile_features(slice-test PRIVATE ${rxx_compile_features})

target_compile_definitions(slice-test PRIVATE ${rxx_compile_definitions})

target_compile_options(slice-test PRIVATE
  ${rxx_compile_options}
  $<$<CONFIG:RELEASE>:${rxx_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${rxx_compile_options_release}>
  )

target_link_libraries(slice-test rxx-test-main)

add_test(NAME Slice.Test COMMAND $<TARGET_FILE:slice-test>)

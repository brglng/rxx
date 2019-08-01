add_executable(str-test
  test.cpp
  )

target_compile_features(str-test PRIVATE ${rxx_compile_features})

target_compile_definitions(str-test PRIVATE ${rxx_compile_definitions})

target_compile_options(str-test PRIVATE
  ${rxx_compile_options}
  $<$<CONFIG:RELEASE>:${rxx_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${rxx_compile_options_release}>
  )

target_link_libraries(str-test rxx-test-main)

add_test(NAME Str.Test COMMAND $<TARGET_FILE:str-test>)

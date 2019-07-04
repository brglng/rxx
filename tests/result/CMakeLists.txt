add_executable(result-test
  test.cpp
  )

target_compile_features(result-test PRIVATE ${rxx_compile_features})

target_compile_definitions(result-test PRIVATE ${rxx_compile_definitions})

target_compile_options(result-test PRIVATE
  ${rxx_compile_options}
  $<$<CONFIG:RELEASE>:${rxx_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${rxx_compile_options_release}>
  )

target_link_libraries(result-test rxx-test-main)

add_test(NAME Result.Test COMMAND $<TARGET_FILE:result-test>)

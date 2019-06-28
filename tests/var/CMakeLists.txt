add_executable(var-test
  var_test.cpp
  )

target_compile_features(var-test PRIVATE ${rxx_compile_features})

target_compile_definitions(var-test PRIVATE ${rxx_compile_definitions})

target_compile_options(var-test PRIVATE
  ${rxx_compile_options}
  $<$<CONFIG:RELEASE>:${rxx_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${rxx_compile_options_release}>
  )

target_link_libraries(var-test rxx-test-main)

add_test(NAME Var.Test COMMAND $<TARGET_FILE:var-test>)

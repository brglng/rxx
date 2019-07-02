add_executable(io-seek-from-test
  test.cpp
  )

target_compile_features(io-seek-from-test PRIVATE ${rxx_compile_features})

target_compile_definitions(io-seek-from-test PRIVATE ${rxx_compile_definitions})

target_compile_options(io-seek-from-test PRIVATE
  ${rxx_compile_options}
  $<$<CONFIG:RELEASE>:${rxx_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${rxx_compile_options_release}>
  )

target_link_libraries(io-seek-from-test rxx-test-main)

add_test(NAME io.SeekFrom.Test COMMAND $<TARGET_FILE:io-seek-from-test>)

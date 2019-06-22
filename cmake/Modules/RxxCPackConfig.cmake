set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Rust flavored C++ library")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_IGNORE_FILES
  /\\\\.git/
  \\\\.git.*
  /build/
  /cmake-build-.*/
  /\\\\.idea/
  /\\\\.ycm_extra_conf\\\\..*
  /GPATH$
  /GRTAGS$
  /GSYMS$
  /GTAGS$
  \\\\.swp$
  \\\\.swo$
  \\\\.DS_Store$
  \\\\.ccls
  \\\\.ccls-cache
  \\\\.clang-format
  )
set(CPACK_SOURCE_GENERATOR "TXZ")
include(CPack)

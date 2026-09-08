sourcemeta_library(NAMESPACE sourcemeta PROJECT core NAME openapi
  SOURCES frame.cc)

if(SOURCEMETA_CORE_INSTALL)
  sourcemeta_library_install(NAMESPACE sourcemeta PROJECT core NAME openapi)
endif()

target_link_libraries(sourcemeta_core_openapi PUBLIC sourcemeta::core::json)
target_link_libraries(sourcemeta_core_openapi PUBLIC
  sourcemeta::core::jsonpointer)
target_link_libraries(sourcemeta_core_openapi PUBLIC sourcemeta::core::memory)

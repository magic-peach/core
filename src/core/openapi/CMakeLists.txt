sourcemeta_library(NAMESPACE sourcemeta PROJECT core NAME openapi
  PRIVATE_HEADERS error.h
  SOURCES helpers.h external_documentation.h info.h server.h
          frame.cc version.cc)

if(SOURCEMETA_CORE_INSTALL)
  sourcemeta_library_install(NAMESPACE sourcemeta PROJECT core NAME openapi)
endif()

target_link_libraries(sourcemeta_core_openapi PUBLIC sourcemeta::core::json)
target_link_libraries(sourcemeta_core_openapi PUBLIC
  sourcemeta::core::jsonpointer)
target_link_libraries(sourcemeta_core_openapi PUBLIC sourcemeta::core::memory)
target_link_libraries(sourcemeta_core_openapi PRIVATE sourcemeta::core::uri)
target_link_libraries(sourcemeta_core_openapi PRIVATE sourcemeta::core::email)

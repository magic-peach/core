sourcemeta_test(NAMESPACE sourcemeta PROJECT core NAME openapi
  SOURCES openapi_frame_error_test.cc)

target_link_libraries(sourcemeta_core_openapi_unit
  PRIVATE sourcemeta::core::openapi)
target_link_libraries(sourcemeta_core_openapi_unit
  PRIVATE sourcemeta::core::json)
target_link_libraries(sourcemeta_core_openapi_unit
  PRIVATE sourcemeta::core::jsonpointer)

# Complete frame tests, expressed as JSON fixtures
sourcemeta_test(NAMESPACE sourcemeta PROJECT core NAME openapi_frame_suite
  SOURCES framesuite.cc)
target_compile_definitions(sourcemeta_core_openapi_frame_suite_unit
  PRIVATE FRAME_SUITE_PATH="${PROJECT_SOURCE_DIR}/test/openapi/frame")
target_link_libraries(sourcemeta_core_openapi_frame_suite_unit
  PRIVATE sourcemeta::core::openapi)
target_link_libraries(sourcemeta_core_openapi_frame_suite_unit
  PRIVATE sourcemeta::core::json)

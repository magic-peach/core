#include <sourcemeta/core/json.h>
#include <sourcemeta/core/jsonpointer.h>
#include <sourcemeta/core/openapi.h>

#include <sourcemeta/core/test.h>

TEST(document_null) {
  const auto document{sourcemeta::core::parse_json("null")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI Description must be an object");
    EXPECT_EQ(error.location(), sourcemeta::core::EMPTY_POINTER);
  }
}

TEST(document_boolean) {
  const auto document{sourcemeta::core::parse_json("true")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI Description must be an object");
    EXPECT_EQ(error.location(), sourcemeta::core::EMPTY_POINTER);
  }
}

TEST(document_integer) {
  const auto document{sourcemeta::core::parse_json("3")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI Description must be an object");
    EXPECT_EQ(error.location(), sourcemeta::core::EMPTY_POINTER);
  }
}

TEST(document_real) {
  const auto document{sourcemeta::core::parse_json("3.1")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI Description must be an object");
    EXPECT_EQ(error.location(), sourcemeta::core::EMPTY_POINTER);
  }
}

TEST(document_string) {
  const auto document{sourcemeta::core::parse_json("\"3.1.1\"")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI Description must be an object");
    EXPECT_EQ(error.location(), sourcemeta::core::EMPTY_POINTER);
  }
}

TEST(document_array) {
  const auto document{sourcemeta::core::parse_json(R"JSON([
    { "openapi": "3.1.1" }
  ])JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI Description must be an object");
    EXPECT_EQ(error.location(), sourcemeta::core::EMPTY_POINTER);
  }
}

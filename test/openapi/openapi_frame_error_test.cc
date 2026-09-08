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

TEST(missing_version) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The OpenAPI Description must declare its version");
    EXPECT_EQ(error.location(), sourcemeta::core::EMPTY_POINTER);
  }
}

TEST(version_is_null) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": null,
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI version must be a string");
    EXPECT_EQ(error.location(), sourcemeta::core::Pointer{"openapi"});
  }
}

TEST(version_is_a_number) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": 3.1,
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI version must be a string");
    EXPECT_EQ(error.location(), sourcemeta::core::Pointer{"openapi"});
  }
}

TEST(version_without_patch_component) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "Unsupported OpenAPI Specification version");
    EXPECT_EQ(error.location(), sourcemeta::core::Pointer{"openapi"});
  }
}

TEST(version_earlier_minor) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.0.4",
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "Unsupported OpenAPI Specification version");
    EXPECT_EQ(error.location(), sourcemeta::core::Pointer{"openapi"});
  }
}

TEST(version_later_minor) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.2.0",
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "Unsupported OpenAPI Specification version");
    EXPECT_EQ(error.location(), sourcemeta::core::Pointer{"openapi"});
  }
}

TEST(swagger_document) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "swagger": "2.0",
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The OpenAPI Description must declare its version");
    EXPECT_EQ(error.location(), sourcemeta::core::EMPTY_POINTER);
  }
}

TEST(info_missing) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The OpenAPI Description must provide an Info Object");
    EXPECT_EQ(error.location(), sourcemeta::core::EMPTY_POINTER);
  }
}

TEST(info_is_null) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": null,
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object must be an object");
    const sourcemeta::core::Pointer location{"info"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_is_an_array) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": [],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object must be an object");
    const sourcemeta::core::Pointer location{"info"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_is_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": "Example",
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object must be an object");
    const sourcemeta::core::Pointer location{"info"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_missing_title) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "version": "1.0.0" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object must declare a title");
    const sourcemeta::core::Pointer location{"info"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_missing_version) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object must declare a version");
    const sourcemeta::core::Pointer location{"info"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_title_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": 1, "version": "1.0.0" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object title must be a string");
    const sourcemeta::core::Pointer location{"info", "title"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_version_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": 1 },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object version must be a string");
    const sourcemeta::core::Pointer location{"info", "version"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_summary_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "summary": [] },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object summary must be a string");
    const sourcemeta::core::Pointer location{"info", "summary"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_description_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "description": false },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object description must be a string");
    const sourcemeta::core::Pointer location{"info", "description"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_terms_of_service_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "termsOfService": 1 },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Info Object terms of service must be a string");
    const sourcemeta::core::Pointer location{"info", "termsOfService"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_terms_of_service_is_not_a_uri_reference) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "termsOfService": "https://example.com/ terms" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Info Object terms of service must be a URI reference");
    const sourcemeta::core::Pointer location{"info", "termsOfService"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_unknown_field) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "licence": {} },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object does not define this field");
    const sourcemeta::core::Pointer location{"info", "licence"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(info_extension_without_the_prefix) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "internal-id": 1 },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Info Object does not define this field");
    const sourcemeta::core::Pointer location{"info", "internal-id"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(contact_is_not_an_object) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "contact": "support@example.com" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Contact Object must be an object");
    const sourcemeta::core::Pointer location{"info", "contact"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(contact_name_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "contact": { "name": 1 } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Contact Object name must be a string");
    const sourcemeta::core::Pointer location{"info", "contact", "name"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(contact_url_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "contact": { "url": 1 } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Contact Object URI must be a string");
    const sourcemeta::core::Pointer location{"info", "contact", "url"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(contact_url_is_not_a_uri_reference) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "contact": { "url": "https://example.com/a b" } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Contact Object URI must be a URI reference");
    const sourcemeta::core::Pointer location{"info", "contact", "url"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(contact_email_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "contact": { "email": 1 } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Contact Object email must be a string");
    const sourcemeta::core::Pointer location{"info", "contact", "email"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(contact_email_is_not_an_email_address) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "contact": { "email": "support" } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Contact Object email must be an email address");
    const sourcemeta::core::Pointer location{"info", "contact", "email"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(contact_unknown_field) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "contact": { "phone": "555" } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Contact Object does not define this field");
    const sourcemeta::core::Pointer location{"info", "contact", "phone"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(license_is_not_an_object) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "license": "MIT" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The License Object must be an object");
    const sourcemeta::core::Pointer location{"info", "license"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(license_missing_name) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "license": { "identifier": "MIT" } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The License Object must declare a name");
    const sourcemeta::core::Pointer location{"info", "license"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(license_name_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "license": { "name": 1 } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The License Object name must be a string");
    const sourcemeta::core::Pointer location{"info", "license", "name"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(license_identifier_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "license": { "name": "MIT", "identifier": 1 } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The License Object identifier must be a string");
    const sourcemeta::core::Pointer location{"info", "license", "identifier"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(license_url_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "license": { "name": "MIT", "url": 1 } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The License Object URI must be a string");
    const sourcemeta::core::Pointer location{"info", "license", "url"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(license_url_is_not_a_uri_reference) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "license": { "name": "MIT", "url": "https://example.com/a b" } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The License Object URI must be a URI reference");
    const sourcemeta::core::Pointer location{"info", "license", "url"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(license_identifier_and_url_together) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "license": { "name": "MIT", "identifier": "MIT", "url": "https://example.com" } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(
        error.what(),
        "The License Object identifier and URI are mutually exclusive");
    const sourcemeta::core::Pointer location{"info", "license"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(license_unknown_field) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "license": { "name": "MIT", "spdx": "MIT" } },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The License Object does not define this field");
    const sourcemeta::core::Pointer location{"info", "license", "spdx"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(json_schema_dialect_is_null) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "jsonSchemaDialect": null,
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI dialect must be a string");
    const sourcemeta::core::Pointer location{"jsonSchemaDialect"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(json_schema_dialect_is_a_number) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "jsonSchemaDialect": 1,
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI dialect must be a string");
    const sourcemeta::core::Pointer location{"jsonSchemaDialect"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(json_schema_dialect_is_an_object) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "jsonSchemaDialect": {},
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI dialect must be a string");
    const sourcemeta::core::Pointer location{"jsonSchemaDialect"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(json_schema_dialect_is_an_array) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "jsonSchemaDialect": [],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI dialect must be a string");
    const sourcemeta::core::Pointer location{"jsonSchemaDialect"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(json_schema_dialect_with_a_space) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "jsonSchemaDialect": "https://spec.openapis.org/oas/3.1/dialect /base",
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI dialect must be a URI reference");
    const sourcemeta::core::Pointer location{"jsonSchemaDialect"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(json_schema_dialect_without_a_scheme_body) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "jsonSchemaDialect": "://base",
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The OpenAPI dialect must be a URI reference");
    const sourcemeta::core::Pointer location{"jsonSchemaDialect"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(servers_is_not_an_array) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": "https://example.com",
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The OpenAPI Description servers must be an array");
    const sourcemeta::core::Pointer location{"servers"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(servers_is_an_object) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": { "url": "/v1" },
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The OpenAPI Description servers must be an array");
    const sourcemeta::core::Pointer location{"servers"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_is_not_an_object) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ "https://example.com" ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Server Object must be an object");
    const sourcemeta::core::Pointer location{"servers", 0};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_missing_url) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "description": "Production" } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Server Object must declare a URL");
    const sourcemeta::core::Pointer location{"servers", 0};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_url_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": 1 } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Server Object URL must be a string");
    const sourcemeta::core::Pointer location{"servers", 0, "url"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_description_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/v1", "description": 1 } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Server Object description must be a string");
    const sourcemeta::core::Pointer location{"servers", 0, "description"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_unknown_field) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/v1", "variable": {} } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Server Object does not define this field");
    const sourcemeta::core::Pointer location{"servers", 0, "variable"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_second_entry_is_invalid) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/v1" }, { "description": "No URL" } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Server Object must declare a URL");
    const sourcemeta::core::Pointer location{"servers", 1};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_variables_is_not_an_object) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/{version}", "variables": [] } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Server Object variables must be an object");
    const sourcemeta::core::Pointer location{"servers", 0, "variables"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_variable_is_not_an_object) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/{version}", "variables": { "version": "v1" } } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(), "The Server Variable Object must be an object");
    const sourcemeta::core::Pointer location{"servers", 0, "variables",
                                             "version"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_variable_missing_default) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/{version}", "variables": { "version": {} } } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Server Variable Object must declare a default");
    const sourcemeta::core::Pointer location{"servers", 0, "variables",
                                             "version"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_variable_default_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/{version}", "variables": { "version": { "default": 1 } } } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Server Variable Object default must be a string");
    const sourcemeta::core::Pointer location{"servers", 0, "variables",
                                             "version", "default"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_variable_description_is_not_a_string) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/{version}", "variables": { "version": { "default": "v1", "description": 1 } } } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Server Variable Object description must be a string");
    const sourcemeta::core::Pointer location{"servers", 0, "variables",
                                             "version", "description"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_variable_enum_is_not_an_array) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/{version}", "variables": { "version": { "default": "v1", "enum": "v1" } } } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Server Variable Object enumeration must be an array");
    const sourcemeta::core::Pointer location{"servers", 0, "variables",
                                             "version", "enum"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_variable_enum_is_empty) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/{version}", "variables": { "version": { "default": "v1", "enum": [] } } } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Server Variable Object enumeration must not be empty");
    const sourcemeta::core::Pointer location{"servers", 0, "variables",
                                             "version", "enum"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_variable_enum_holds_a_number) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/{version}", "variables": { "version": { "default": "v1", "enum": [ "v1", 2 ] } } } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(
        error.what(),
        "The Server Variable Object enumeration must only hold strings");
    const sourcemeta::core::Pointer location{"servers", 0,      "variables",
                                             "version", "enum", 1};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_variable_default_outside_enum) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/{version}", "variables": { "version": { "default": "v3", "enum": [ "v1", "v2" ] } } } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(
        error.what(),
        "The Server Variable Object default must exist in its enumeration");
    const sourcemeta::core::Pointer location{"servers", 0, "variables",
                                             "version", "default"};
    EXPECT_EQ(error.location(), location);
  }
}

TEST(server_variable_unknown_field) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "servers": [ { "url": "/{version}", "variables": { "version": { "default": "v1", "examples": [] } } } ],
    "paths": {}
  })JSON")};

  try {
    [[maybe_unused]] const sourcemeta::core::OpenAPIFrame frame{document,
                                                                nullptr};
    FAIL();
  } catch (const sourcemeta::core::OpenAPIError &error) {
    EXPECT_STREQ(error.what(),
                 "The Server Variable Object does not define this field");
    const sourcemeta::core::Pointer location{"servers", 0, "variables",
                                             "version", "examples"};
    EXPECT_EQ(error.location(), location);
  }
}

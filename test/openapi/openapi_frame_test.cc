#include <sourcemeta/core/json.h>
#include <sourcemeta/core/openapi.h>

#include <sourcemeta/core/test.h>

TEST(version_patch_zero) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.0",
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.version(), sourcemeta::core::OpenAPIVersion::OPENAPI_3_1);
}

TEST(version_patch_one) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.version(), sourcemeta::core::OpenAPIVersion::OPENAPI_3_1);
}

TEST(version_patch_two) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.2",
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.version(), sourcemeta::core::OpenAPIVersion::OPENAPI_3_1);
}

TEST(version_pre_release) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.0-rc0",
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.version(), sourcemeta::core::OpenAPIVersion::OPENAPI_3_1);
}

TEST(version_with_components_and_no_paths) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "components": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.version(), sourcemeta::core::OpenAPIVersion::OPENAPI_3_1);
}

TEST(version_with_webhooks_and_no_paths) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "webhooks": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.version(), sourcemeta::core::OpenAPIVersion::OPENAPI_3_1);
}

TEST(version_agrees_with_json_export) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.version(), sourcemeta::core::OpenAPIVersion::OPENAPI_3_1);
  EXPECT_EQ(frame.to_json().at("version"), sourcemeta::core::JSON{"3.1"});
}

TEST(info_required_fields_only) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0" },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.info().title, "Example");
  EXPECT_EQ(frame.info().version, "1.0.0");
  EXPECT_FALSE(frame.info().summary.has_value());
  EXPECT_FALSE(frame.info().description.has_value());
  EXPECT_FALSE(frame.info().terms_of_service.has_value());
  EXPECT_FALSE(frame.info().contact.has_value());
  EXPECT_FALSE(frame.info().license.has_value());
}

TEST(info_every_field) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": {
      "title": "Example Pet Store App",
      "summary": "A pet store manager.",
      "description": "This is an example server for a pet store.",
      "termsOfService": "https://example.com/terms/",
      "contact": {
        "name": "API Support",
        "url": "https://www.example.com/support",
        "email": "support@example.com"
      },
      "license": {
        "name": "Apache 2.0",
        "url": "https://www.apache.org/licenses/LICENSE-2.0.html"
      },
      "version": "1.0.1"
    },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.info().title, "Example Pet Store App");
  EXPECT_EQ(frame.info().version, "1.0.1");
  EXPECT_EQ(frame.info().summary.value(), "A pet store manager.");
  EXPECT_EQ(frame.info().description.value(),
            "This is an example server for a pet store.");
  EXPECT_EQ(frame.info().terms_of_service.value(),
            "https://example.com/terms/");
  EXPECT_EQ(frame.info().contact.value().name.value(), "API Support");
  EXPECT_EQ(frame.info().contact.value().url.value(),
            "https://www.example.com/support");
  EXPECT_EQ(frame.info().contact.value().email.value(), "support@example.com");
  EXPECT_EQ(frame.info().license.value().name, "Apache 2.0");
  EXPECT_FALSE(frame.info().license.value().identifier.has_value());
  EXPECT_EQ(frame.info().license.value().url.value(),
            "https://www.apache.org/licenses/LICENSE-2.0.html");
}

TEST(info_empty_string_values_are_kept) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "", "version": "", "summary": "" },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.info().title, "");
  EXPECT_EQ(frame.info().version, "");
  EXPECT_TRUE(frame.info().summary.has_value());
  EXPECT_EQ(frame.info().summary.value(), "");
}

TEST(info_relative_terms_of_service) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": {
      "title": "Example",
      "version": "1.0.0",
      "termsOfService": "/terms"
    },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.info().terms_of_service.value(), "/terms");
}

TEST(info_empty_contact) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": { "title": "Example", "version": "1.0.0", "contact": {} },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_TRUE(frame.info().contact.has_value());
  EXPECT_FALSE(frame.info().contact.value().name.has_value());
  EXPECT_FALSE(frame.info().contact.value().url.has_value());
  EXPECT_FALSE(frame.info().contact.value().email.has_value());
}

TEST(info_license_with_spdx_identifier) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": {
      "title": "Example",
      "version": "1.0.0",
      "license": { "name": "Apache 2.0", "identifier": "Apache-2.0" }
    },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.info().license.value().name, "Apache 2.0");
  EXPECT_EQ(frame.info().license.value().identifier.value(), "Apache-2.0");
  EXPECT_FALSE(frame.info().license.value().url.has_value());
}

TEST(info_license_identifier_is_not_validated) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": {
      "title": "Example",
      "version": "1.0.0",
      "license": { "name": "Nonsense", "identifier": "not an SPDX expression" }
    },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.info().license.value().identifier.value(),
            "not an SPDX expression");
}

TEST(info_extensions_are_accepted) {
  const auto document{sourcemeta::core::parse_json(R"JSON({
    "openapi": "3.1.1",
    "info": {
      "title": "Example",
      "version": "1.0.0",
      "x-internal-id": 42,
      "x-oai-reserved": null,
      "x-oas-reserved": [ 1, 2 ],
      "x-": "the prefix alone is a legal field name",
      "contact": { "name": "Support", "x-slack": "#support" },
      "license": { "name": "MIT", "x-approved": true }
    },
    "paths": {}
  })JSON")};

  const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
  EXPECT_EQ(frame.info().title, "Example");
  EXPECT_EQ(frame.info().contact.value().name.value(), "Support");
  EXPECT_EQ(frame.info().license.value().name, "MIT");
}

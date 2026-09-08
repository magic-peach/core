#include <sourcemeta/core/openapi.h>

#include "info.h"
#include "server.h"

#include <memory>      // std::make_unique
#include <string_view> // std::string_view
#include <utility>     // std::unreachable

namespace {
using namespace std::string_view_literals;

constexpr auto HASH_OPENAPI{sourcemeta::core::JSON::Object::hash("openapi"sv)};
constexpr auto HASH_JSON_SCHEMA_DIALECT{
    sourcemeta::core::JSON::Object::hash("jsonSchemaDialect"sv)};

auto version_string(const sourcemeta::core::OpenAPIVersion version)
    -> sourcemeta::core::JSON::StringView {
  switch (version) {
    // OpenAPI Specification 3.1.1, Section 4.1: "The `major`.`minor` portion
    // of the version string (for example `3.1`) SHALL designate the OAS
    // feature set"
    case sourcemeta::core::OpenAPIVersion::OPENAPI_3_1:
      return "3.1"sv;
  }

  std::unreachable();
}

auto detect_version(const sourcemeta::core::JSON &document)
    -> sourcemeta::core::OpenAPIVersion {
  // OpenAPI Specification 3.1.1, Section 4.2: "An OpenAPI Document that
  // conforms to the OpenAPI Specification is itself a JSON object, which may
  // be represented either in JSON or YAML format"
  if (!document.is_object()) {
    throw sourcemeta::core::OpenAPIError{
        sourcemeta::core::EMPTY_POINTER,
        "The OpenAPI Description must be an object"};
  }

  // OpenAPI Specification 3.1.1, Section 4.8.1: "openapi | string | REQUIRED.
  // This string MUST be the version number of the OpenAPI Specification that
  // the OpenAPI Document uses"
  const auto *version{document.try_at("openapi", HASH_OPENAPI)};
  if (version == nullptr) {
    throw sourcemeta::core::OpenAPIError{
        sourcemeta::core::EMPTY_POINTER,
        "The OpenAPI Description must declare its version"};
  }

  if (!version->is_string()) {
    throw sourcemeta::core::OpenAPIError{
        sourcemeta::core::Pointer{"openapi"},
        "The OpenAPI version must be a string"};
  }

  const auto result{sourcemeta::core::openapi_version(document)};
  if (!result.has_value()) {
    throw sourcemeta::core::OpenAPIError{
        sourcemeta::core::Pointer{"openapi"},
        "Unsupported OpenAPI Specification version"};
  }

  return result.value();
}

// The default dialect is not resolved here, only checked. Applying it to the
// Schema Objects that a document contains is what framing them will do
auto check_json_schema_dialect(const sourcemeta::core::JSON &document) -> void {
  const auto *dialect{
      document.try_at("jsonSchemaDialect", HASH_JSON_SCHEMA_DIALECT)};
  if (dialect == nullptr) {
    return;
  }

  // OpenAPI Specification 3.1.1, Section 4.8.1: "jsonSchemaDialect | string |
  // The default value for the `$schema` keyword within Schema Objects
  // contained within this OAS document. This MUST be in the form of a URI"
  [[maybe_unused]] const auto value{
      sourcemeta::core::openapi_expect_uri_reference(
          *dialect, sourcemeta::core::EMPTY_POINTER, "jsonSchemaDialect"sv,
          "The OpenAPI dialect must be a string",
          "The OpenAPI dialect must be a URI reference")};
}

} // namespace

namespace sourcemeta::core {

struct OpenAPIFrame::Internal {
  OpenAPIVersion version;
  OpenAPIInfo info;
};

OpenAPIFrame::OpenAPIFrame(const JSON &document, const OpenAPIResolver &,
                           const std::string_view)
    : internal_{std::make_unique<Internal>(detect_version(document),
                                           openapi_parse_info(document))} {
  check_json_schema_dialect(document);
  openapi_check_servers(document);
}

OpenAPIFrame::~OpenAPIFrame() = default;

auto OpenAPIFrame::version() const noexcept -> OpenAPIVersion {
  return this->internal_->version;
}

auto OpenAPIFrame::info() const noexcept -> const OpenAPIInfo & {
  return this->internal_->info;
}

auto OpenAPIFrame::to_json(const std::optional<PointerPositionTracker> &) const
    -> JSON {
  // Read through the accessors rather than the internal state, so that what
  // this reports and what a caller can observe cannot drift apart
  auto result{JSON::make_object()};
  result.assign_assume_new("version", JSON{version_string(this->version())});
  return result;
}

} // namespace sourcemeta::core

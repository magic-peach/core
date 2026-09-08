#include <sourcemeta/core/openapi.h>

#include <memory> // std::make_unique

namespace sourcemeta::core {

struct OpenAPIFrame::Internal {};

OpenAPIFrame::OpenAPIFrame(const JSON &document, const OpenAPIResolver &,
                           const std::string_view)
    : internal_{std::make_unique<Internal>()} {
  // OpenAPI Specification 3.1.1, Section 4.2: "An OpenAPI Document that
  // conforms to the OpenAPI Specification is itself a JSON object, which may
  // be represented either in JSON or YAML format"
  if (!document.is_object()) {
    throw OpenAPIError{EMPTY_POINTER,
                       "The OpenAPI Description must be an object"};
  }
}

OpenAPIFrame::~OpenAPIFrame() = default;

auto OpenAPIFrame::to_json(const std::optional<PointerPositionTracker> &) const
    -> JSON {
  return JSON::make_object();
}

} // namespace sourcemeta::core

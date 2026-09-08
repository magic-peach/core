#include <sourcemeta/core/openapi.h>

#include <memory> // std::make_unique

namespace sourcemeta::core {

struct OpenAPIFrame::Internal {};

OpenAPIFrame::OpenAPIFrame(const JSON &, const OpenAPIResolver &,
                           const std::string_view)
    : internal_{std::make_unique<Internal>()} {}

OpenAPIFrame::~OpenAPIFrame() = default;

auto OpenAPIFrame::to_json(const std::optional<PointerPositionTracker> &) const
    -> JSON {
  return JSON::make_object();
}

} // namespace sourcemeta::core

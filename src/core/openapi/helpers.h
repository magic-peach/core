#ifndef SOURCEMETA_CORE_OPENAPI_HELPERS_H_
#define SOURCEMETA_CORE_OPENAPI_HELPERS_H_

#include <sourcemeta/core/openapi.h>

#include <sourcemeta/core/uri.h>

#include <algorithm>   // std::ranges::find
#include <array>       // std::array
#include <cstddef>     // std::size_t
#include <string_view> // std::string_view

namespace sourcemeta::core {

using namespace std::string_view_literals;

// OpenAPI Specification 3.1.1, Section 4.9: "The field name MUST begin with
// `x-`, for example, `x-internal-id`"
constexpr auto OPENAPI_EXTENSION_PREFIX{"x-"sv};

constexpr auto OPENAPI_HASH_DESCRIPTION{JSON::Object::hash("description"sv)};

inline auto openapi_child(const Pointer &base, const JSON::StringView field)
    -> Pointer {
  return base.concat(JSON::String{field});
}

inline auto openapi_child(const Pointer &base, const std::size_t index)
    -> Pointer {
  return base.concat(index);
}

// OpenAPI Specification 3.1.1, Section 4.2: "The schema exposes two types of
// fields: fixed fields, which have a declared name, and patterned fields,
// which have a declared pattern for the field name". These objects declare
// `^x-` as their only pattern, so a member that is neither is not a field that
// this specification defines
template <std::size_t Size>
auto openapi_reject_unknown_fields(
    const JSON &object, const std::array<JSON::StringView, Size> &fields,
    const Pointer &base, const char *message) -> void {
  for (const auto &entry : object.as_object()) {
    if (entry.first.starts_with(OPENAPI_EXTENSION_PREFIX) ||
        std::ranges::find(fields, entry.first) != fields.cend()) {
      continue;
    }

    throw OpenAPIError{openapi_child(base, entry.first), message};
  }
}

inline auto openapi_expect_string(const JSON &value, const Pointer &base,
                                  const JSON::StringView field,
                                  const char *message) -> JSON::StringView {
  if (!value.is_string()) {
    throw OpenAPIError{openapi_child(base, field), message};
  }

  return value.to_string();
}

// OpenAPI Specification 3.1.1, Section 4.6: "Unless specified otherwise, all
// fields that are URIs MAY be relative references as defined by RFC3986", so
// what these fields hold is a URI reference rather than an absolute URI
inline auto openapi_expect_uri_reference(const JSON &value, const Pointer &base,
                                         const JSON::StringView field,
                                         const char *type_message,
                                         const char *syntax_message)
    -> JSON::StringView {
  const auto result{openapi_expect_string(value, base, field, type_message)};
  if (!URI::is_uri_reference(result)) {
    throw OpenAPIError{openapi_child(base, field), syntax_message};
  }

  return result;
}

} // namespace sourcemeta::core

#endif

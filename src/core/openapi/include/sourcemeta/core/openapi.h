#ifndef SOURCEMETA_CORE_OPENAPI_H_
#define SOURCEMETA_CORE_OPENAPI_H_

#ifndef SOURCEMETA_CORE_OPENAPI_EXPORT
#include <sourcemeta/core/openapi_export.h>
#endif

#include <sourcemeta/core/json.h>
#include <sourcemeta/core/jsonpointer.h>
#include <sourcemeta/core/memory.h>

#include <cstdint>     // std::uint8_t
#include <functional>  // std::function
#include <memory>      // std::unique_ptr
#include <optional>    // std::optional, std::nullopt
#include <string_view> // std::string_view

/// @defgroup openapi OpenAPI
/// @brief A growing implementation of the OpenAPI Specification 3.1.
///
/// This module reports where an OpenAPI Description declares its JSON Schemas
/// and leaves what is inside them to a JSON Schema implementation.
///
/// This functionality is included as follows:
///
/// ```cpp
/// #include <sourcemeta/core/openapi.h>
/// ```

namespace sourcemeta::core {

/// @ingroup openapi
/// The OpenAPI Description versions that this module recognises
enum class OpenAPIVersion : std::uint8_t {
  /// The OpenAPI Specification 3.1 revision
  OPENAPI_3_1
};

/// @ingroup openapi
/// What a resolver hands back: either a document it owns or one that the
/// caller keeps alive. The root of the result may be an OpenAPI Object or a
/// Schema Object, as an OpenAPI Description may span both
using OpenAPIResolverResult = OwnedOrReference<JSON>;

/// @ingroup openapi
/// Resolve a URI to a document that forms part of an OpenAPI Description. A
/// resolver that never hands back a document confines framing to the entry
/// document alone
using OpenAPIResolver = std::function<OpenAPIResolverResult(std::string_view)>;

/// @ingroup openapi
/// A static analysis pass over an OpenAPI Description that computes the
/// locations it exposes, the references between them, the operations it
/// describes, and where its JSON Schemas begin. It does not look inside those
/// schemas. For example:
///
/// ```cpp
/// #include <sourcemeta/core/json.h>
/// #include <sourcemeta/core/openapi.h>
/// #include <iostream>
///
/// const auto document{sourcemeta::core::parse_json(R"({
///   "openapi": "3.1.1",
///   "info": { "title": "Example", "version": "1.0.0" },
///   "paths": {}
/// })")};
///
/// const sourcemeta::core::OpenAPIFrame frame{document, nullptr};
/// sourcemeta::core::prettify(frame.to_json(), std::cout);
/// std::cout << std::endl;
/// ```
///
/// A frame is analysed once, on construction, and is immutable afterwards.
class SOURCEMETA_CORE_OPENAPI_EXPORT OpenAPIFrame {
public:
  /// Frame an OpenAPI Description from a given entry document. The resulting
  /// locations point into the documents rather than copying from them, so
  /// every document, along with the given base, must outlive the frame.
  ///
  /// The base is the retrieval URI of the entry document. OpenAPI 3.1 offers
  /// a document no way of declaring an identity of its own, so this is the
  /// only way to give the description one. A referenced document whose root
  /// is a Schema Object may still override it through `$id`
  OpenAPIFrame(const JSON &document, const OpenAPIResolver &resolver,
               std::string_view default_base = "");

  ~OpenAPIFrame();

  // We rely on internal caches that would be dangling otherwise
  OpenAPIFrame(const OpenAPIFrame &) = delete;
  auto operator=(const OpenAPIFrame &) -> OpenAPIFrame & = delete;
  OpenAPIFrame(OpenAPIFrame &&) = delete;
  auto operator=(OpenAPIFrame &&) -> OpenAPIFrame & = delete;

  /// Export the frame as JSON. This is the complete state of the frame, and
  /// for now its only window
  [[nodiscard]] auto to_json(
      const std::optional<PointerPositionTracker> &tracker = std::nullopt) const
      -> JSON;

private:
// Exporting symbols that depends on the standard C++ library is considered
// safe.
// https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-2-c4275?view=msvc-170&redirectedfrom=MSDN
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  struct Internal;
  std::unique_ptr<Internal> internal_;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};

} // namespace sourcemeta::core

#endif

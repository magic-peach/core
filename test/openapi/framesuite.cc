#include <sourcemeta/core/json.h>
#include <sourcemeta/core/openapi.h>
#include <sourcemeta/core/test.h>

#include <algorithm>   // std::ranges::find
#include <cstddef>     // std::size_t
#include <filesystem>  // std::filesystem
#include <iostream>    // std::cerr
#include <optional>    // std::nullopt
#include <sstream>     // std::ostringstream
#include <string>      // std::string
#include <string_view> // std::string_view
#include <vector>      // std::vector

namespace {

// Every key a fixture may declare. Anything else is a mistake that would
// otherwise go unnoticed, as the runner would simply not read it
// NOLINTBEGIN(cert-err58-cpp,bugprone-throwing-static-initialization)
const std::vector<std::string> KNOWN_KEYS{"document", "defaultBase", "resolver",
                                          "frame"};
// NOLINTEND(cert-err58-cpp,bugprone-throwing-static-initialization)

auto make_resolver(const sourcemeta::core::JSON &test)
    -> sourcemeta::core::OpenAPIResolver {
  if (!test.defines("resolver")) {
    return nullptr;
  }

  const auto &registry{test.at("resolver")};
  return [registry](const std::string_view identifier)
             -> sourcemeta::core::OpenAPIResolverResult {
    const auto *match{
        registry.try_at(sourcemeta::core::JSON::String{identifier})};
    if (match != nullptr) {
      return *match;
    }

    return std::nullopt;
  };
}

// A frame keeps a view into the base it was given, so the base has to outlive
// it. The caller owns this, as anything built inside the analysis would dangle
// on return and only misbehave later, when the frame is read back
auto make_default_base(const sourcemeta::core::JSON &test)
    -> sourcemeta::core::JSON::String {
  const auto *base{test.try_at("defaultBase")};
  return base == nullptr ? sourcemeta::core::JSON::String{} : base->to_string();
}

auto run_frame_test(const sourcemeta::core::JSON &test) -> void {
  for (const auto &entry : test.as_object()) {
    EXPECT_TRUE(std::ranges::find(KNOWN_KEYS, entry.first) !=
                KNOWN_KEYS.cend());
  }

  EXPECT_TRUE(test.defines("document"));
  EXPECT_TRUE(test.defines("frame"));

  const auto resolver{make_resolver(test)};
  const auto default_base{make_default_base(test)};

  const sourcemeta::core::OpenAPIFrame frame{test.at("document"), resolver,
                                             default_base};
  EXPECT_EQ(frame.to_json(), test.at("frame"));
}

auto register_tests(const std::filesystem::path &directory) -> std::size_t {
  std::size_t count{0};
  for (const std::filesystem::directory_entry &entry :
       std::filesystem::recursive_directory_iterator{directory}) {
    if (!entry.is_regular_file() || entry.path().extension() != ".json") {
      continue;
    }

    const auto suite{entry.path().parent_path().filename().string()};
    std::ostringstream name;
    for (const auto character : entry.path().stem().string()) {
      name << (character == '-' ? '_' : character);
    }

    const auto test{sourcemeta::core::read_json(entry.path())};
    sourcemeta::core::test_register("OpenAPIFrameSuite_" + suite, name.str(),
                                    __FILE__, __LINE__,
                                    [test]() -> void { run_frame_test(test); });
    count += 1;
  }

  return count;
}

} // namespace

auto main(int argc, char **argv) -> int {
  const auto count{register_tests(std::filesystem::path{FRAME_SUITE_PATH})};
  // A fixture in the wrong place, or with the wrong extension, would otherwise
  // never run and nobody would notice
  if (count == 0) {
    std::cerr << "No frame fixtures found at " << FRAME_SUITE_PATH << "\n";
    return 1;
  }

  return sourcemeta::core::test_run(argc, argv);
}

#pragma once

#if SIMDJSON_EXCEPTIONS

#include "kostya.h"

namespace kostya {

using namespace simdjson;
using namespace simdjson::builtin;

struct simdjson_ondemand {
  ondemand::parser parser{};

  bool run(simdjson::padded_string &json, std::vector<point> &result) {
    auto doc = parser.iterate(json);
    for (ondemand::object point : doc.find_field("coordinates")) {
      result.emplace_back(kostya::point{point.find_field("x"), point.find_field("y"), point.find_field("z")});
    }
    return true;
  }
};

BENCHMARK_TEMPLATE(kostya, simdjson_ondemand)->UseManualTime();

} // namespace kostya

#endif // SIMDJSON_EXCEPTIONS

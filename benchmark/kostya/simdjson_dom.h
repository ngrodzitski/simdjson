#pragma once

#if SIMDJSON_EXCEPTIONS

#include "kostya.h"

namespace kostya {

using namespace simdjson;

struct simdjson_dom {
  dom::parser parser{};

  bool run(simdjson::padded_string &json, std::vector<point> &points) {
    for (auto point : parser.parse(json)["coordinates"]) {
      points.emplace_back(kostya::point{point["x"], point["y"], point["z"]});
    }
    return true;
  }
};

BENCHMARK_TEMPLATE(kostya, simdjson_dom);

} // namespace kostya

#endif // SIMDJSON_EXCEPTIONS
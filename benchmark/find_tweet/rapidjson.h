#pragma once

#ifdef SIMDJSON_COMPETITION_RAPIDJSON

#include "find_tweet.h"

namespace find_tweet {

using namespace rapidjson;

struct rapidjson_base {
  Document doc{};

  bool run(Document &root, uint64_t find_id, std::string_view &result) {
    if (root.HasParseError() || !root.IsObject()) { return false; }
    auto statuses = root.FindMember("statuses");
    if (statuses == root.MemberEnd() || !statuses->value.IsArray()) { return false; }
    for (auto &tweet : statuses->value.GetArray()) {
      if (!tweet.IsObject()) { return false; }
      auto id = tweet.FindMember("id");
      if (id == tweet.MemberEnd() || !id->value.IsUint64()) { return false; }
      if (id->value.GetUint64() == find_id) {
        auto text = tweet.FindMember("text");
        if (text == tweet.MemberEnd() || !text->value.IsString()) { return false; }
        result = { text->value.GetString(), text->value.GetStringLength() };
        return true;
      }
    }

    return false;
  }
};

struct rapidjson : rapidjson_base {
  bool run(simdjson::padded_string &json, uint64_t find_id, std::string_view &result) {
    return rapidjson_base::run(doc.Parse<kParseValidateEncodingFlag>(json.data()), find_id, result);
  }
};
BENCHMARK_TEMPLATE(find_tweet, rapidjson)->UseManualTime();

struct rapidjson_insitu : rapidjson_base {
  bool run(simdjson::padded_string &json, uint64_t find_id, std::string_view &result) {
    return rapidjson_base::run(doc.ParseInsitu<kParseValidateEncodingFlag>(json.data()), find_id, result);
  }
};
BENCHMARK_TEMPLATE(find_tweet, rapidjson_insitu)->UseManualTime();

} // namespace partial_tweets

#endif // SIMDJSON_COMPETITION_RAPIDJSON

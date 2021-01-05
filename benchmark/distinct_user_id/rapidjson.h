#pragma once

#ifdef SIMDJSON_COMPETITION_RAPIDJSON

#include "distinct_user_id.h"

namespace distinct_user_id {

using namespace rapidjson;

struct rapidjson_base {
  Document doc{};

  bool run(Document &root, std::vector<uint64_t> &ids) {
    if (root.HasParseError()) { printf("parse error\n"); return false; }
    if (!root.IsObject()) { printf("root is not an object\n"); return false; }
    auto statuses = root.FindMember("statuses");
    if (statuses == root.MemberEnd() || !statuses->value.IsArray()) { printf("statuses is not an array\n"); return false; }
    for (auto &tweet : statuses->value.GetArray()) {
      if (!tweet.IsObject()) { return false; }
      auto user = tweet.FindMember("user");
      if (user == tweet.MemberEnd() || !user->value.IsObject()) { printf("user is not an object\n"); return false; }
      auto id = user->value.FindMember("id");
      if (id == user->value.MemberEnd() || !id->value.IsUint64()) { printf("id is not an int\n"); return false; }
      ids.push_back(id->value.GetUint64());

      auto retweet = tweet.FindMember("retweeted_status");
      if (retweet != tweet.MemberEnd()) {
        if (!retweet->value.IsObject()) { printf("retweet is not an object\n"); return false; }
        user = retweet->value.FindMember("user");
        if (user == retweet->value.MemberEnd() || !user->value.IsObject()) { printf("rewtweet.user is not an object\n"); return false; }
        id = user->value.FindMember("id");
        if (id == user->value.MemberEnd() || !id->value.IsUint64()) { printf("retweet.id is not an int\n"); return false; }
        ids.push_back(id->value.GetUint64());
      }
    }

    return true;
  }
};

struct rapidjson : public rapidjson_base {
  bool run(simdjson::padded_string &json, std::vector<uint64_t> &ids) {
    return rapidjson_base::run(doc.Parse<kParseValidateEncodingFlag>(json.data()), ids);
  }
};
BENCHMARK_TEMPLATE(distinct_user_id, rapidjson);

struct rapidjson_insitu : public rapidjson_base {
  bool run(simdjson::padded_string &json, std::vector<uint64_t> &ids) {
    return rapidjson_base::run(doc.ParseInsitu<kParseValidateEncodingFlag>(json.data()), ids);
  }
};
BENCHMARK_TEMPLATE(distinct_user_id, rapidjson_insitu);

} // namespace partial_tweets

#endif // SIMDJSON_COMPETITION_RAPIDJSON

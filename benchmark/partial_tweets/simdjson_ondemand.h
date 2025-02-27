#pragma once

#if SIMDJSON_EXCEPTIONS

#include "partial_tweets.h"

namespace partial_tweets {

using namespace simdjson;
using namespace simdjson::builtin;

struct simdjson_ondemand {
  ondemand::parser parser{};

  simdjson_really_inline uint64_t nullable_int(ondemand::value value) {
    if (value.is_null()) { return 0; }
    return value;
  }

  simdjson_really_inline twitter_user read_user(ondemand::object user) {
    return { user.find_field("id"), user.find_field("screen_name") };
  }

  bool run(simdjson::padded_string &json, std::vector<tweet> &result) {
    // Walk the document, parsing the tweets as we go
    auto doc = parser.iterate(json);
    for (ondemand::object tweet : doc.find_field("statuses")) {
      result.emplace_back(partial_tweets::tweet{
        tweet.find_field("created_at"),
        tweet.find_field("id"),
        tweet.find_field("text"),
        nullable_int(tweet.find_field("in_reply_to_status_id")),
        read_user(tweet.find_field("user")),
        tweet.find_field("retweet_count"),
        tweet.find_field("favorite_count")
      });
    }

    return true;
  }
};

BENCHMARK_TEMPLATE(partial_tweets, simdjson_ondemand)->UseManualTime();

} // namespace partial_tweets

#endif // SIMDJSON_EXCEPTIONS

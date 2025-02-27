#include "simdjson.h"

SIMDJSON_PUSH_DISABLE_ALL_WARNINGS

#ifdef SIMDJSON_COMPETITION_YYJSON
#include "yyjson.h"
#endif

#ifdef SIMDJSON_COMPETITION_RAPIDJSON
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#endif

// This has to be last, for reasons I don't yet understand
#include <benchmark/benchmark.h>

SIMDJSON_POP_DISABLE_WARNINGS

#include "partial_tweets/simdjson_dom.h"
#include "partial_tweets/simdjson_ondemand.h"
#include "partial_tweets/yyjson.h"
#include "partial_tweets/rapidjson.h"

#include "large_random/simdjson_dom.h"
#include "large_random/simdjson_ondemand.h"
#include "large_random/simdjson_ondemand_unordered.h"
#include "large_random/yyjson.h"
#include "large_random/rapidjson.h"

#include "kostya/simdjson_dom.h"
#include "kostya/simdjson_ondemand.h"
#include "kostya/yyjson.h"
#include "kostya/rapidjson.h"

#include "distinct_user_id/simdjson_dom.h"
#include "distinct_user_id/simdjson_ondemand.h"
#include "distinct_user_id/yyjson.h"
#include "distinct_user_id/rapidjson.h"

#include "find_tweet/simdjson_dom.h"
#include "find_tweet/simdjson_ondemand.h"
#include "find_tweet/yyjson.h"
#include "find_tweet/rapidjson.h"

BENCHMARK_MAIN();

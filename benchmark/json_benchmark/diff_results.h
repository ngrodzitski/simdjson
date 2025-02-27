#pragma once

#include <vector>
#include <sstream>

template<typename T>
static bool diff_results(benchmark::State &state, const T &result, const T &reference);

template<typename T>
struct result_differ {
  static bool diff(benchmark::State &state, const T &result, const T &reference) {
    if (result != reference) {
      std::stringstream str;
      str << "result incorrect: " << result << " ... reference: " << reference;
      state.SkipWithError(str.str().data());
      return false;
    }
    return true;
  }
};

template<typename T>
struct result_differ<std::vector<T>> {
  static bool diff(benchmark::State &state, const std::vector<T> &result, const std::vector<T> &reference) {
    auto result_iter = result.begin();
    auto reference_iter = reference.begin();
    while (result_iter != result.end() && reference_iter != reference.end()) {
      if (!diff_results(state, *result_iter, *reference_iter)) { return false; }
      result_iter++;
      reference_iter++;
    }
    if (result_iter != result.end()) {
      std::stringstream str;
      str << "extra results (got " << result.size() << ", expected " << reference.size() << "): first extra element: " << *result_iter;
      state.SkipWithError(str.str().data());
      return false;
    } else if (reference_iter != reference.end()) {
      std::stringstream str;
      str << "missing results (got " << result.size() << ", expected " << reference.size() << "): first missing element: " << *reference_iter;
      state.SkipWithError(str.str().data());
      return false;
    }
    return true;
  }
};

template<typename T>
static bool diff_results(benchmark::State &state, const T &result, const T &reference) {
  return result_differ<T>::diff(state, result, reference);
}


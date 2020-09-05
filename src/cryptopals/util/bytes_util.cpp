#include "cryptopals/util/bytes_util.h"

namespace cryptopals::util {

std::vector<Bytes> SplitBytes(const Bytes& input, size_t n) {
  std::vector<Bytes> result;

  for (int i = 0; i < input.size(); i += n) {
    auto last = std::min(input.size(), i + n);
    result.emplace_back(input.begin() + i, input.begin() + last);
  }

  return result;
}

std::vector<Bytes> SplitAndTransposeBytes(const Bytes& input, size_t n) {
  std::vector<Bytes> result(n);

  for (int i = 0; i < input.size(); i += n) {
    for (int j = 0; j < n; j++) {
      if (i + j >= input.size()) {
        break;
      }
      result[j].push_back(input.at(i + j));
    }
  }

  return result;
}

Bytes JoinAndTransposeBytes(const std::vector<Bytes>& input) {
  Bytes result;

  for (int i = 0; i < input.at(0).size(); ++i) {
    for (int j = 0; j < input.size(); ++j) {
      if (i >= input.at(j).size()) {
        break;
      }
      result.push_back(input.at(j).at(i));
    }
  }
  return result;
}

struct KeysizeResult {
  double score;
  size_t size;
};

}  // namespace cryptopals::util

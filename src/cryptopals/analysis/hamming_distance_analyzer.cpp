#include "cryptopals/analysis/hamming_distance_analyzer.h"

#include <bitset>
#include <functional>
#include <numeric>

#include "cryptopals/util/logging.h"

namespace cryptopals::analysis {
namespace {

struct HammingDistance {
  inline int operator()(uint8_t lhs, uint8_t rhs) {
    return std::bitset<8>(lhs ^ rhs).count();
  }
};

}  // namespace

double HammingDistanceAnalyzer::CompareBytes(
    const cryptopals::util::Bytes& lhs, const cryptopals::util::Bytes& rhs) {
  auto min_size = std::min(lhs.size(), rhs.size());
  auto diff_size = std::max(lhs.size(), rhs.size()) - min_size;

  return std::inner_product(lhs.begin(), lhs.begin() + min_size, rhs.begin(),
                            /*init=*/0, std::plus<int>(), HammingDistance()) +
         8 * diff_size;
}

}  // namespace cryptopals::analysis

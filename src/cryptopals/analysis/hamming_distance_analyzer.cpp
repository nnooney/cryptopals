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
  if (lhs.size() != rhs.size()) {
    LOG(ERROR)
        << "Cannot compute hamming distance for bytes of different length ("
        << lhs.size() << " & " << rhs.size() << ")";
  }

  return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), /*init=*/0,
                            std::plus<int>(), HammingDistance());
}

}  // namespace cryptopals::analysis

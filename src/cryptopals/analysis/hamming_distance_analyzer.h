#ifndef CRYPTOPALS_ANALYSIS_HAMMING_DISTANCE_ANALYZER_H_
#define CRYPTOPALS_ANALYSIS_HAMMING_DISTANCE_ANALYZER_H_

#include "cryptopals/analysis/analyzer.h"

namespace cryptopals::analysis {

// An analyzer that calculates the hamming distance between two Bytes objects.
// The hamming distance is the number of bits that are different between two
// strings.
class HammingDistanceAnalyzer : public AnalyzerInterface {
 public:
  // Implements CompareBytes from AnalyzerInterface.
  double CompareBytes(const cryptopals::util::Bytes& lhs,
                      const cryptopals::util::Bytes& rhs) override;
};

}  // namespace cryptopals::analysis

#endif  // CRYPTOPALS_ANALYSIS_HAMMING_DISTANCE_ANALYZER_H_

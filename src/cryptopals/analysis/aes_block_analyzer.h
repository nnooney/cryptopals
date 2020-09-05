#ifndef CRYPTOPALS_ANALYSIS_AES_BLOCK_ANALYZER_H_
#define CRYPTOPALS_ANALYSIS_AES_BLOCK_ANALYZER_H_

#include "cryptopals/analysis/analyzer.h"

namespace cryptopals::analysis {

// An analyzer that detects AES encryption in ECB mode.
class AesBlockAnalyzer : public AnalyzerInterface {
 public:
  // Implements AnalyzeBytes from AnalyzerInterface. The score returned is the
  // number of pairs of AES blocks that are identical, scaled to the range
  // [0-1].
  double AnalyzeBytes(const cryptopals::util::Bytes& input) override;
};

}  // namespace cryptopals::analysis

#endif  // CRYPTOPALS_ANALYSIS_AES_BLOCK_ANALYZER_H_

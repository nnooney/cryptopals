#ifndef CRYPTOPALS_ANALYSIS_ANALYZER_H_
#define CRYPTOPALS_ANALYSIS_ANALYZER_H_

#include "cryptopals/util/bytes.h"

namespace cryptopals::analysis {

// The AnalyzerInterface class describes the public interface required for an
// analyzer. Default implementations are provided that return a zero score for
// all inputs and log an error.
class AnalyzerInterface {
 public:
  AnalyzerInterface() = default;
  virtual ~AnalyzerInterface() {}

  // Calculate the match of `input` and return a score, represented as a double.
  // The derived class is responsible for determining the meaning of the score.
  virtual double AnalyzeBytes(const cryptopals::util::Bytes& input);

  // Calculate the comparison between `lhs` and `rhs` and return a score,
  // represented as a double. The derived class is responsbile for determining
  // the meaning of the score.
  virtual double CompareBytes(const cryptopals::util::Bytes& lhs,
                              const cryptopals::util::Bytes& rhs);
};

}  // namespace cryptopals::analysis

#endif  // CRYPTOPALS_ANALYSIS_ANALYZER_H_

#ifndef CRYPTOPALS_FREQUENCY_ANALYZER_H_
#define CRYPTOPALS_FREQUENCY_ANALYZER_H_

#include "cryptopals/util/bytes.h"

namespace cryptopals::frequency {

// The AnalyzerInterface class describes the public interface required for an
// analyzer.
class AnalyzerInterface {
 public:
  virtual ~AnalyzerInterface() {}

  // Calculate the match of `input` and return a score, represented as a double.
  // The larger the number, the higher the match.
  virtual double AnalyzeBytes(const cryptopals::util::Bytes& input) = 0;
};

}  // namespace cryptopals::frequency

#endif  // CRYPTOPALS_FREQUENCY_ANALYZER_H_

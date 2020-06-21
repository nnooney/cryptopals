#ifndef CRYPTOPALS_ANALYSIS_ENGLISH_ASCII_FREQUENCY_ANALYZER_H_
#define CRYPTOPALS_ANALYSIS_ENGLISH_ASCII_FREQUENCY_ANALYZER_H_

#include "cryptopals/analysis/analyzer.h"

namespace cryptopals::analysis {

// A frequency analyzer that calculates a score based on English ASCII text.
class EnglishAsciiFrequencyAnalyzer : public AnalyzerInterface {
 public:
  // Implements AnalyzeBytes from AnalyzerInterface.
  double AnalyzeBytes(const cryptopals::util::Bytes& input) override;
};

}  // namespace cryptopals::analysis

#endif  // CRYPTOPALS_ANALYSIS_ENGLISH_ASCII_FREQUENCY_ANALYZER_H_

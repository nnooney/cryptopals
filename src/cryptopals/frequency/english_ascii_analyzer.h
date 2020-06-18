#ifndef CRYPTOPALS_FREQUENCY_ENGLISH_ASCII_ANALYZER_H_
#define CRYPTOPALS_FREQUENCY_ENGLISH_ASCII_ANALYZER_H_

#include "cryptopals/frequency/analyzer.h"

namespace cryptopals::frequency {

// A frequency analyzer that calculates a score based on English ASCII text.
class EnglishAsciiAnalyzer : public AnalyzerInterface {
  public:

    // Implements AnalyzeBytes from AnalyzerInterface.
    double AnalyzeBytes(const cryptopals::util::Bytes& input) override;
};

}  // namespace cryptopals::frequency

#endif  // CRYPTOPALS_FREQUENCY_ENGLISH_ASCII_ANALYZER_H_

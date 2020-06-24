#ifndef CRYPTOPALS_ANALYSIS_FREQUENCY_ANALYZER_H_
#define CRYPTOPALS_ANALYSIS_FREQUENCY_ANALYZER_H_

#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>

#include "absl/algorithm/container.h"
#include "cryptopals/analysis/analyzer.h"
#include "cryptopals/encoding/encoding.h"
#include "cryptopals/util/algorithm.h"

template <class K, class V>
std::ostream& operator<<(std::ostream& os,
                         const absl::flat_hash_map<K, V>& map) {
  os << "[ ";
  for (auto item : map) {
    os << "(" << std::hex << std::setw(2) << static_cast<int>(item.first)
       << std::dec << ", " << std::fixed << item.second << ") ";
  }
  os << "]";
  return os;
}

namespace cryptopals::analysis {

// A frequency analyzer that calculates the Chi-Squared statistic for data based
// on `frequency_data_`.
template <class CodePointType>
class FrequencyAnalyzer : public AnalyzerInterface {
 public:
  FrequencyAnalyzer(
      std::unique_ptr<cryptopals::encoding::EncodingInterface<CodePointType>>
          encoding_interface,
      std::vector<std::pair<CodePointType, double>>&& frequency_data)
      : encoding_interface_(std::move(encoding_interface)),
        frequency_data_(std::move(frequency_data)) {}

  template <typename InputIt>
  FrequencyAnalyzer(
      std::unique_ptr<cryptopals::encoding::EncodingInterface<CodePointType>>
          encoding_interface,
      InputIt first1, InputIt last1)
      : encoding_interface_(std::move(encoding_interface)),
        frequency_data_(first1, last1) {}

  // Implements AnalyzeBytes from AnalyzerInterface. The score returned here is
  // the chi-squared statistic. A lower number indicates a better match to
  // `frequency_data_`.
  double AnalyzeBytes(const cryptopals::util::Bytes& input) override {
    typedef absl::flat_hash_map<CodePointType, double> map_type;
    typedef std::pair<const CodePointType, double> element_type;

    // Generate a histogram from `input`.
    auto histogram = encoding_interface_->GenerateHistogram(input);

    // Calculate the expected number of observations from `frequency_data_`.
    map_type expected_frequencies(frequency_data_.begin(),
                                  frequency_data_.end());
    auto multiply_by_observations = [&](element_type& codepoint) {
      codepoint.second *= input.size();
    };
    absl::c_for_each(expected_frequencies, multiply_by_observations);

    // Determine the residuals.
    map_type residuals = cryptopals::util::MapCombine(
        expected_frequencies, histogram, std::minus<double>());
    auto square = [](element_type& codepoint) {
      codepoint.second = std::pow(codepoint.second, 2);
    };
    absl::c_for_each(residuals, square);

    // Normalize the result.
    auto safe_divides = [](double lhs, double rhs) {
      return (rhs != 0) ? lhs / rhs : lhs;
    };
    map_type normalized_results = cryptopals::util::MapCombine(
        residuals, expected_frequencies, safe_divides);

    // Accumulate the result.
    auto pair_second_sum = [](double accumulator, const element_type& rhs) {
      return accumulator + rhs.second;
    };
    return absl::c_accumulate(normalized_results, 0.0, pair_second_sum);
  }

 private:
  FrequencyAnalyzer() = delete;

  std::unique_ptr<cryptopals::encoding::EncodingInterface<CodePointType>>
      encoding_interface_;
  std::vector<std::pair<CodePointType, double>> frequency_data_;
};

}  // namespace cryptopals::analysis

#endif  // CRYPTOPALS_ANALYSIS_FREQUENCY_ANALYZER_H_

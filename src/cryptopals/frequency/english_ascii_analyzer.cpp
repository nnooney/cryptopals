#include "cryptopals/frequency/english_ascii_analyzer.h"

#include <cctype>
#include <cmath>
#include <vector>

#include "cryptopals/frequency/data/english.h"

namespace cryptopals::frequency {
namespace {

// Calculates the number of occurrences of each English letter in `input`,
// returning a vector of 26 elements containing the number of occurrences of
// each letter.
std::vector<size_t> TabulateLetters(const cryptopals::util::Bytes& input) {
  const uint8_t UPPERCASE_A_OFFSET = 'A';
  const uint8_t LOWERCASE_A_OFFSET = 'a';
  const size_t ALPHABET_LENGTH = 26;

  std::vector<size_t> letter_frequencies(ALPHABET_LENGTH);

  for (uint8_t byte : input) {
    if (byte - UPPERCASE_A_OFFSET < ALPHABET_LENGTH) {
      letter_frequencies[byte - UPPERCASE_A_OFFSET]++;
    } else if (byte - LOWERCASE_A_OFFSET < ALPHABET_LENGTH) {
      letter_frequencies[byte - LOWERCASE_A_OFFSET]++;
    }
    // If the byte doesn't represent a letter, then skip over it.
  }

  return letter_frequencies;
}

}  // namespace

double EnglishAsciiAnalyzer::AnalyzeBytes(
    const cryptopals::util::Bytes& input) {
  // Store a vector of the counts of each letter.
  std::vector<size_t> letter_frequencies = TabulateLetters(input);

  // Calculate the total score by measuring the error between each letter and
  // the average frequency. The highest possible score is 100, and the lowest
  // possible score is 0.
  double score = 100;
  for (int i = 0; i < letter_frequencies.size(); ++i) {
    score -= std::abs(letter_frequencies[i] - english::alphabet[i]);
  }

  // Subtract points for non-printable characters.
  for (uint8_t byte : input) {
    if (!std::isprint(byte)) {
      score -= 1;
    }
  }
  return score;
}

}  // namespace cryptopals::frequency

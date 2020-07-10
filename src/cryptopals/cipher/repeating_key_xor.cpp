#include "cryptopals/cipher/repeating_key_xor.h"

#include <limits>
#include <numeric>
#include <vector>

#include "cryptopals/analysis/data/oanc_english.h"
#include "cryptopals/analysis/frequency_analyzer.h"
#include "cryptopals/analysis/hamming_distance_analyzer.h"
#include "cryptopals/cipher/decryption_result.h"
#include "cryptopals/cipher/single_byte_xor.h"
#include "cryptopals/encoding/ascii.h"
#include "cryptopals/util/logging.h"

namespace cryptopals::cipher {
namespace {

using cryptopals::util::Bytes;

// The maximum length of a key to try to decode using repeating key xor.
constexpr size_t CONFIG_KEYSIZE_LIMIT = 40;

// Splits `input` into a vector of bytes, each of length `n`. For example,
// SplitBytes("ABCDEFGHIJ", 4) returns {"ABCD", "EFGH", "IJ"}.
std::vector<Bytes> SplitBytes(const Bytes& input, size_t n) {
  std::vector<Bytes> result;

  for (int i = 0; i < input.size(); i += n) {
    auto last = std::min(input.size(), i + n);
    result.emplace_back(input.begin() + i, input.begin() + last);
  }

  return result;
}

// Splits `input` into a vector of bytes, where each new bytes object contains
// every `n`th byte. For example, SplitAndTransposeBytes("ABCDEFGHIJ", 4)
// returns {"AEI", "BFJ", "CG", "DH"}.
std::vector<Bytes> SplitAndTransposeBytes(const Bytes& input, size_t n) {
  std::vector<Bytes> result(n);

  for (int i = 0; i < input.size(); i += n) {
    for (int j = 0; j < n; j++) {
      if (i + j >= input.size()) {
        break;
      }
      result[j].push_back(input.at(i + j));
    }
  }

  return result;
}

// Performs the reverse operation of SplitAndTransposeBytes. Note that this
// algorithm assumes that the first element of the vector is the longest.
Bytes JoinAndTransposeBytes(const std::vector<Bytes>& input) {
  Bytes result;

  for (int i = 0; i < input.at(0).size(); ++i) {
    for (int j = 0; j < input.size(); ++j) {
      if (i >= input.at(j).size()) {
        break;
      }
      result.push_back(input.at(j).at(i));
    }
  }
  return result;
}

struct KeysizeResult {
  double score;
  size_t size;
};

// Determines the likely keysize for `ciphertext`, assuming that it was
// encrypted with repeating key xor.
std::vector<KeysizeResult> CrackKeysize(const Bytes& ciphertext) {
  std::vector<KeysizeResult> results;
  cryptopals::analysis::HammingDistanceAnalyzer hamming_distance_analyzer;
  for (size_t i = 2; i < ciphertext.size() / 2 && i < CONFIG_KEYSIZE_LIMIT;
       ++i) {
    std::vector<Bytes> split_ciphertext = SplitBytes(ciphertext, i);
    std::vector<double> scores;

    // Determine the hamming distance for all segments of the text.
    for (auto c1 = split_ciphertext.begin(); c1 != split_ciphertext.end();
         ++c1) {
      for (auto c2 = c1 + 1; c2 != split_ciphertext.end(); ++c2) {
        scores.push_back(hamming_distance_analyzer.CompareBytes(*c1, *c2));
      }
    }

    // The score is the average hamming distance of each pair of segments
    // normalized by the length of the segments.
    double score = std::accumulate(scores.begin(), scores.end(), /*init=*/0.0) /
                   scores.size() / i;
    results.push_back({.score = score, .size = i});
  }

  // Return the 4 most likely results.
  std::sort(results.begin(), results.end(),
            [](auto lhs, auto rhs) { return lhs.score < rhs.score; });
  results.resize(4);

  return results;
}

}  // namespace

Bytes RepeatingKeyXor::Encrypt(const Bytes& plaintext, const Bytes& key) const {
  return plaintext ^ key;
}

Bytes RepeatingKeyXor::Decrypt(const Bytes& ciphertext,
                               const Bytes& key) const {
  return ciphertext ^ key;
}

RepeatingKeyXor::DecryptionResultType RepeatingKeyXor::Crack(
    const Bytes& ciphertext) {
  LOG(INFO) << "Cracking: " << ciphertext.ToHex();

  DecryptionResultType decryption_result = {
      .score = std::numeric_limits<double>::max()};

  SingleByteXor single_byte_xor;
  using cryptopals::analysis::data::oanc_english::code_point_frequency;
  cryptopals::analysis::FrequencyAnalyzer<uint8_t> frequency_analyzer(
      absl::make_unique<cryptopals::encoding::AsciiEncoding>(),
      code_point_frequency.begin(), code_point_frequency.end());

  std::vector<KeysizeResult> possible_keysizes = CrackKeysize(ciphertext);
  for (const KeysizeResult keysize_result : possible_keysizes) {
    LOG(INFO) << "Attempting to crack key length = " << keysize_result.size
              << " (score = " << keysize_result.score << ")";

    std::vector<Bytes> split_ciphertext =
        SplitAndTransposeBytes(ciphertext, keysize_result.size);
    std::vector<Bytes> split_plaintext;
    Bytes possible_key;

    for (const Bytes& single_byte_ciphertext : split_ciphertext) {
      SingleByteXor::DecryptionResultType partial_decryption_result =
          single_byte_xor.Crack(single_byte_ciphertext);
      split_plaintext.push_back(partial_decryption_result.decrypted_text);
      possible_key.push_back(partial_decryption_result.key);
    }

    Bytes decrypted_text = JoinAndTransposeBytes(split_plaintext);
    double score = frequency_analyzer.AnalyzeBytes(decrypted_text);

    LOG(INFO) << "Decrypted text score = " << score;

    if (score < decryption_result.score) {
      decryption_result = {.score = score,
                           .decrypted_text = decrypted_text,
                           .key = possible_key};
    }
  }

  return decryption_result;
}

}  // namespace cryptopals::cipher

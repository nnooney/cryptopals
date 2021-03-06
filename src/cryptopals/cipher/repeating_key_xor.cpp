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
#include "cryptopals/util/bytes_util.h"
#include "cryptopals/util/logging.h"

namespace cryptopals::cipher {
namespace {

using cryptopals::util::Bytes;

// The maximum length of a key to try to decode using repeating key xor.
constexpr size_t CONFIG_KEYSIZE_LIMIT = 40;

// The number of key sizes to attempt to decrypt with.
constexpr size_t CONFIG_KEYSIZE_ATTEMPTS = 4;

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

    // Determine the hamming distance for all pairwise segments of the text.
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

  // Return the most likely results.
  std::sort(results.begin(), results.end(),
            [](auto lhs, auto rhs) { return lhs.score < rhs.score; });
  results.resize(CONFIG_KEYSIZE_ATTEMPTS);

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

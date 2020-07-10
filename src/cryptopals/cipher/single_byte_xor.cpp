#include "cryptopals/cipher/single_byte_xor.h"

#include <limits>

#include "cryptopals/analysis/data/oanc_english.h"
#include "cryptopals/analysis/frequency_analyzer.h"
#include "cryptopals/cipher/decryption_result.h"
#include "cryptopals/encoding/ascii.h"

namespace cryptopals::cipher {

using cryptopals::util::Bytes;

Bytes SingleByteXor::Encrypt(const Bytes& plaintext, const uint8_t key) const {
  return plaintext ^ Bytes::CreateFromIntegral(key);
}

Bytes SingleByteXor::Decrypt(const Bytes& ciphertext, const uint8_t key) const {
  return ciphertext ^ Bytes::CreateFromIntegral(key);
}

SingleByteXor::DecryptionResultType SingleByteXor::Crack(
    const Bytes& ciphertext) {
  // Use frequency analysis to determine the most likely decryption.
  using cryptopals::analysis::data::oanc_english::code_point_frequency;

  DecryptionResultType decryption_result = {
      .score = std::numeric_limits<double>::max()};

  cryptopals::analysis::FrequencyAnalyzer<uint8_t> frequency_analyzer(
      absl::make_unique<cryptopals::encoding::AsciiEncoding>(),
      code_point_frequency.begin(), code_point_frequency.end());

  for (uint8_t possible_key = 0;
       possible_key < std::numeric_limits<uint8_t>::max(); ++possible_key) {
    Bytes decrypted_text = Decrypt(ciphertext, possible_key);
    double score = frequency_analyzer.AnalyzeBytes(decrypted_text);
    if (score < decryption_result.score) {
      decryption_result = {.score = score,
                           .decrypted_text = decrypted_text,
                           .key = possible_key};
    }
  }

  return decryption_result;
}

}  // namespace cryptopals::cipher

// Utility functions for implementing AES encryption/decryption. See
// https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf for the
// specification of the AES algorithm.
//
// DISCLAIMER: This algorithm is implemented for educational purposes only. By
// no means is it guaranteed to be secure.
#ifndef CRYPTOPALS_UTIL_AES_H_
#define CRYPTOPALS_UTIL_AES_H_

#include <array>
#include <cstdint>
#include <span>

#include "cryptopals/util/bytes.h"
#include "cryptopals/util/internal/status/statusor.h"

namespace cryptopals::util {

// The xtime() function, described in section 4.2.1 of the AES spec.
inline constexpr uint8_t xtime(const uint8_t input) {
  return (input << 1) ^ (input & 0x80 ? 0x1b : 0);
}

// A helper function to apply xtime() `n` times to `input`.
inline constexpr uint8_t apply_xtime(const uint8_t input, const uint8_t n) {
  uint8_t result = input;
  for (int i = 0; i < n; ++i) {
    result = xtime(result);
  }
  return result;
}

// The multiplication algorithm described in section 4.2.1 of the AES spec.
inline constexpr uint8_t mul(const uint8_t input, const uint8_t multiplier) {
  uint8_t result = 0;
  for (int i = 0; i < 8; ++i) {
    if (multiplier & (1 << i)) {
      result ^= apply_xtime(input, i);
    }
  }
  return result;
}

// The number of bytes in a word as considered by the AES spec.
inline constexpr size_t WORD_SIZE = 4;

// A struct that represents the internal state of the AES cipher.
struct AesState {
  static constexpr size_t SIZE_WORDS = 4;
  static constexpr size_t SIZE_BYTES = SIZE_WORDS * WORD_SIZE;

  // The bytes array stores the intermediate state of the AES cipher in
  // column-order as depicted in the diagram below. This matches the input and
  // output order as depicted in section 3.4 of the AES spec.
  //  00 04 08 12
  //  01 05 09 13
  //  02 06 10 14
  //  03 07 11 15
  std::array<uint8_t, SIZE_BYTES> bytes = {0};
};

typedef std::span<const uint8_t, AesState::SIZE_BYTES> aes_block_span;

// Encrypts a single `block` of bytes using the provided `key` according to the
// AES spec. Returns the encrypted block or an error status.
absl::StatusOr<cryptopals::util::Bytes> EncryptBlock(
    aes_block_span block, const cryptopals::util::Bytes& key);

// Modifies state to substitute the bytes according to section 5.1.1 of the
// AES spec.
void SubstituteBytes(AesState& state);

// Modifies state to shift the rows according to section 5.1.2 of the AES spec.
void ShiftRows(AesState& state);

// Modifies state to mix the columns according to section 5.1.3 of the AES spec.
void MixColumns(AesState& state);

// Modifies state to add the round key according to section 5.1.4 of the AES
// spec.
void AddRoundKey(AesState& state, aes_block_span round_key);

// Expands the key according to section 5.2 of the AES spec. Returns the
// expanded key schedule or an error status.
absl::StatusOr<cryptopals::util::Bytes> GenerateKeySchedule(
    const cryptopals::util::Bytes& key);

// Computes the substitution of `input` according to section 5.2 of the AES
// spec.
cryptopals::util::Bytes SubWord(const cryptopals::util::Bytes& input);

// Computes the rotation of `input` according to section 5.2 of the AES spec.
cryptopals::util::Bytes RotWord(const cryptopals::util::Bytes& input);

// Decrypts a single `block` of bytes using the provided `key` according to the
// AES spec. Returns the decrypted block or an error status.
absl::StatusOr<cryptopals::util::Bytes> DecryptBlock(
    aes_block_span block, const cryptopals::util::Bytes& key);

// Modifies state to shift the rows according to section 5.3.1 of the AES spec.
void InvShiftRows(AesState& state);

// Modifies state to substitute the bytes according to section 5.3.2 of the
// AES spec.
void InvSubstituteBytes(AesState& state);

// Modifies state to mix the columns according to section 5.3.3 of the AES spec.
void InvMixColumns(AesState& state);

}  // namespace cryptopals::util

// Performs the

#endif  // CRYPTOPALS_UTIL_AES_H_

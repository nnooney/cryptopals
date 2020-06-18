#include "cryptopals/cipher/single_byte_xor.h"

#include <queue>

#include "cryptopals/frequency/english_ascii_analyzer.h"

namespace cryptopals::cipher {

using cryptopals::util::Bytes;

Bytes SingleByteXor::Encrypt(const Bytes& plaintext, const uint8_t key) const {
  return plaintext ^ Bytes::CreateFromIntegral(key);
}

Bytes SingleByteXor::Decrypt(const Bytes& ciphertext, const uint8_t key) const {
  return ciphertext ^ Bytes::CreateFromIntegral(key);
}

}  // namespace cryptopals::cipher

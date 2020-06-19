#include "cryptopals/cipher/repeating_key_xor.h"

namespace cryptopals::cipher {

using cryptopals::util::Bytes;

Bytes RepeatingKeyXor::Encrypt(const Bytes& plaintext, const Bytes& key) const {
  return plaintext ^ key;
}

Bytes RepeatingKeyXor::Decrypt(const Bytes& ciphertext,
                               const Bytes& key) const {
  return ciphertext ^ key;
}

}  // namespace cryptopals::cipher

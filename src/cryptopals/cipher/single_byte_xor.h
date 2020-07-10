#ifndef CRYPTOPALS_CIPHER_SINGLE_BYTE_XOR_H_
#define CRYPTOPALS_CIPHER_SINGLE_BYTE_XOR_H_

#include <cstdint>

#include "cryptopals/cipher/cipher.h"
#include "cryptopals/util/bytes.h"

namespace cryptopals::cipher {

class SingleByteXor : public CipherInterface<uint8_t> {
 public:
  // Implements Encrypt from CipherInterface.
  cryptopals::util::Bytes Encrypt(const cryptopals::util::Bytes& plaintext,
                                  const uint8_t key) const override;

  // Implements Decrypt from CipherInterface.
  cryptopals::util::Bytes Decrypt(const cryptopals::util::Bytes& ciphertext,
                                  const uint8_t key) const override;

  // Cracks the cipher and returns the most likely decryption result for
  // `ciphertext`.
  DecryptionResultType Crack(const cryptopals::util::Bytes& ciphertext);
};

}  // namespace cryptopals::cipher

#endif  // CRYPTOPALS_CIPHER_SINGLE_BYTE_XOR_H_

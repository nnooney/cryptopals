#ifndef CRYPTOPALS_CIPHER_REPEATING_KEY_XOR_H_
#define CRYPTOPALS_CIPHER_REPEATING_KEY_XOR_H_

#include "cryptopals/cipher/cipher.h"
#include "cryptopals/util/bytes.h"

namespace cryptopals::cipher {

class RepeatingKeyXor : public CipherInterface<cryptopals::util::Bytes> {
 public:
  // Implements Encrypt from CipherInterface.
  cryptopals::util::Bytes Encrypt(
      const cryptopals::util::Bytes& plaintext,
      const cryptopals::util::Bytes& key) const override;

  // Implements Decrypt from CipherInterface.
  cryptopals::util::Bytes Decrypt(const cryptopals::util::Bytes& ciphertext,
                                  const cryptopals::util::Bytes& key) const;

  // Cracks the cipher and returns the most likely decryption result for
  // `ciphertext`.
  DecryptionResultType Crack(const cryptopals::util::Bytes& ciphertext);
};

}  // namespace cryptopals::cipher

#endif  // CRYPTOPALS_CIPHER_REPEATING_KEY_XOR_H_

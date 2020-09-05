// DISCLAIMER: This algorithm is implemented for educational purposes only. By
// no means is it guaranteed to be secure.

#ifndef CRYPTOPALS_CIPHER_AES_ECB_H_
#define CRYPTOPALS_CIPHER_AES_ECB_H_

#include "cryptopals/cipher/symmetric_cipher.h"

namespace cryptopals::cipher {

class AesEcb : public SymmetricCipherInterface<cryptopals::util::Bytes> {
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

  // Detect determines the probability (range [0-1]) that `ciphertext` was
  // encrypted using this cipher.
  double Detect(const cryptopals::util::Bytes& ciphertext);
};

}  // namespace cryptopals::cipher

#endif  // CRYPTOPALS_CIPHER_AES_ECB_H_

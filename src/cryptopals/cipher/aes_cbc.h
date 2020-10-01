// DISCLAIMER: This algorithm is implemented for educational purposes only. By
// no means is it guaranteed to be secure.

#ifndef CRYPTOPALS_CIPHER_AES_CBC_H_
#define CRYPTOPALS_CIPHER_AES_CBC_H_

#include "absl/status/status.h"
#include "cryptopals/cipher/symmetric_cipher.h"

namespace cryptopals::cipher {

class AesCbc : public SymmetricCipherInterface<cryptopals::util::Bytes> {
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

  // Sets the initialization vector to `iv`.
  absl::Status SetIv(const cryptopals::util::Bytes& iv);

 private:
  cryptopals::util::Bytes iv_;
};

}  // namespace cryptopals::cipher

#endif  // CRYPTOPALS_CIPHER_AES_CBC_H_

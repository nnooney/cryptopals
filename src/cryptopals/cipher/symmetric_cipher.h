#ifndef CRYPTOPALS_CIPHER_H_
#define CRYPTOPALS_CIPHER_H_

#include <type_traits>

#include "cryptopals/cipher/decryption_result.h"
#include "cryptopals/util/bytes.h"

namespace cryptopals::cipher {

// The CipherInterface class defines the public interface required for a cipher.
template <class KeyType>
class SymmetricCipherInterface {
 public:
  // KeyParamType resolves to a reference type if the instantiated template type
  // is not trivially copyable or is "expensive" to copy (in this case,
  // expensive is anything larger than a void*); otherwise, it resolves to a
  // value type. This allows the compiler to better optimize function calls when
  // a small KeyType is used.
  using KeyParamType =
      std::conditional_t<std::is_trivially_copyable_v<KeyType> &&
                             sizeof(KeyType) <= sizeof(void*),
                         const KeyType, const KeyType&>;

  // Provide a type for DecryptionResult objects that this Cipher implements.
  using DecryptionResultType = DecryptionResult<KeyType>;

  virtual ~SymmetricCipherInterface() {}

  // Encrypts `plaintext` using `key` and returns the encrypted ciphertext.
  virtual cryptopals::util::Bytes Encrypt(
      const cryptopals::util::Bytes& plaintext, KeyParamType key) const = 0;

  // Decrypts `ciphertext` using `key` and returns the decrypted plaintext.
  virtual cryptopals::util::Bytes Decrypt(
      const cryptopals::util::Bytes& ciphertext, KeyParamType key) const = 0;
};

}  // namespace cryptopals::cipher

#endif  // CRYPTOPALS_CIPHER_H_

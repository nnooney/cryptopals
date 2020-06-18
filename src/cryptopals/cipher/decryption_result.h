#ifndef CRYPTOPALS_CIPHER_DECRYPTION_RESULT_H_
#define CRYPTOPALS_CIPHER_DECRYPTION_RESULT_H_

#include <ostream>

#include "cryptopals/util/bytes.h"

namespace cryptopals::cipher {

// The DecryptionResult struct contains information related to decrypting some
// ciphertext.
template <class KeyType>
struct DecryptionResult {
  // A score associated with the decryption.
  double score;

  // The decrypted text.
  cryptopals::util::Bytes decrypted_text;

  // The key used to decrypt the text.
  KeyType key;

  // Support for comparing DecryptionResult objects.
  friend auto operator<=>(const DecryptionResult& lhs,
                          const DecryptionResult& rhs) = default;

  // Support for streamed printing of DecryptionResult objects.

  friend std::ostream& operator<<(std::ostream& os,
                                  const DecryptionResult& obj) {
    if constexpr (std::is_same_v<KeyType, unsigned char>) {
      os << obj.score << " - " << static_cast<int>(obj.key) << " - "
         << obj.decrypted_text.ToRaw();
    } else {
      os << obj.score << " - " << obj.key << " - "
         << obj.decrypted_text.ToRaw();
    }
    return os;
  }
};

}  // namespace cryptopals::cipher

#endif  // CRYPTOPALS_CIPHER_DECRYPTION_RESULT_H_

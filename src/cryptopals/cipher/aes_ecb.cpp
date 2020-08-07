// DISCLAIMER: This algorithm is implemented for educational purposes only. By
// no means is it guaranteed to be secure.

#include "cryptopals/cipher/aes_ecb.h"

#include <algorithm>
#include <span>

#include "cryptopals/util/aes.h"
#include "cryptopals/util/algorithm.h"
#include "cryptopals/util/internal/status_macros.h"
#include "cryptopals/util/internal/statusor.h"
#include "cryptopals/util/logging.h"

namespace cryptopals::cipher {
namespace {

using cryptopals::util::AesState;
using cryptopals::util::Bytes;
using cryptopals::util::FloorMultiple;

}  // namespace

Bytes AesEcb::Encrypt(const Bytes& plaintext, const Bytes& key) const {
  if (plaintext.size() % AesState::SIZE_BYTES != 0) {
    LOG(ERROR) << "Error: plaintext is not a multiple of AES block size ("
               << AesState::SIZE_BYTES << " bytes)";
    return Bytes();
  }

  Bytes ciphertext;

  for (size_t i = 0; i < plaintext.size(); i += AesState::SIZE_BYTES) {
    cryptopals::util::aes_block_span block(plaintext.begin() + i,
                                           AesState::SIZE_BYTES);

    absl::StatusOr<Bytes> encrypted_block =
        cryptopals::util::EncryptBlock(block, key);
    if (!encrypted_block.ok()) {
      LOG(ERROR) << "Error encrypting block at offset " << i << ": "
                 << encrypted_block.status();
      return Bytes();
    }
    ciphertext.Append(encrypted_block.value());
  }

  return ciphertext;
}

Bytes AesEcb::Decrypt(const Bytes& ciphertext, const Bytes& key) const {
  if (ciphertext.size() % AesState::SIZE_BYTES != 0) {
    LOG(ERROR) << "Error: ciphertext is not a multiple of AES block size ("
               << AesState::SIZE_BYTES << " bytes)";
    return Bytes();
  }

  Bytes plaintext;

  for (size_t i = 0; i < ciphertext.size(); i += AesState::SIZE_BYTES) {
    cryptopals::util::aes_block_span block(ciphertext.begin() + i,
                                           AesState::SIZE_BYTES);

    absl::StatusOr<Bytes> decrypted_block =
        cryptopals::util::DecryptBlock(block, key);
    if (!decrypted_block.ok()) {
      LOG(ERROR) << "Error decrypting block at offset " << i << ": "
                 << decrypted_block.status();
      return Bytes();
    }
    plaintext.Append(decrypted_block.value());
  }

  return plaintext;
}

AesEcb::DecryptionResultType AesEcb::Crack(const Bytes& ciphertext) {
  DecryptionResultType decryption_result = {
      .score = std::numeric_limits<double>::max()};

  return decryption_result;
}

}  // namespace cryptopals::cipher

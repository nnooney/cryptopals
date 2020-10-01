// DISCLAIMER: This algorithm is implemented for educational purposes only. By
// no means is it guaranteed to be secure.

#include "cryptopals/cipher/aes_cbc.h"

#include <algorithm>
#include <span>

#include "absl/status/status_macros.h"
#include "absl/status/statusor.h"
#include "cryptopals/analysis/aes_block_analyzer.h"
#include "cryptopals/util/aes.h"
#include "cryptopals/util/algorithm.h"
#include "cryptopals/util/logging.h"

namespace cryptopals::cipher {

using cryptopals::util::AesState;
using cryptopals::util::Bytes;

Bytes AesCbc::Encrypt(const Bytes& plaintext, const Bytes& key) const {
  if (plaintext.size() % AesState::SIZE_BYTES != 0) {
    LOG(ERROR) << "Error: plaintext is not a multiple of AES block size ("
               << AesState::SIZE_BYTES << " bytes)";
    return Bytes();
  }
  if (iv_.size() != AesState::SIZE_BYTES) {
    LOG(ERROR)
        << "Error: iv is not initialized (use SetIv() before encrypting)";
    return Bytes();
  }

  Bytes ciphertext;
  Bytes mixing_block = iv_;

  for (size_t i = 0; i < plaintext.size(); i += AesState::SIZE_BYTES) {
    mixing_block ^= Bytes(plaintext.begin() + i,
                          plaintext.begin() + i + AesState::SIZE_BYTES);

    absl::StatusOr<Bytes> encrypted_block = cryptopals::util::EncryptBlock(
        cryptopals::util::aes_block_span(mixing_block.begin(),
                                         AesState::SIZE_BYTES),
        key);
    if (!encrypted_block.ok()) {
      LOG(ERROR) << "Error encrypting block at offset " << i << ": "
                 << encrypted_block.status();
      return Bytes();
    }
    mixing_block = encrypted_block.value();
    ciphertext.Append(encrypted_block.value());
  }

  return ciphertext;
}

Bytes AesCbc::Decrypt(const Bytes& ciphertext, const Bytes& key) const {
  if (ciphertext.size() % AesState::SIZE_BYTES != 0) {
    LOG(ERROR) << "Error: ciphertext is not a multiple of AES block size ("
               << AesState::SIZE_BYTES << " bytes)";
    return Bytes();
  }
  if (iv_.size() != AesState::SIZE_BYTES) {
    LOG(ERROR)
        << "Error: iv is not initialized (use SetIv() before decrypting)";
    return Bytes();
  }

  Bytes plaintext;
  Bytes mixing_block = iv_;

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
    mixing_block ^= decrypted_block.value();
    plaintext.Append(mixing_block);

    mixing_block = Bytes::CreateFromRange(block.begin(), block.end());
  }

  return plaintext;
}

AesCbc::DecryptionResultType AesCbc::Crack(const Bytes& ciphertext) {
  DecryptionResultType decryption_result = {
      .score = std::numeric_limits<double>::max()};

  return decryption_result;
}

absl::Status AesCbc::SetIv(const Bytes& iv) {
  if (iv.size() != AesState::SIZE_BYTES) {
    return absl::InvalidArgumentErrorBuilder()
           << "iv is not " << AesState::SIZE_BYTES << " bytes";
  }
  iv_ = iv;
  return absl::OkStatus();
}

}  // namespace cryptopals::cipher

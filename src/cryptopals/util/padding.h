// Utilities to add/remove padding from Bytes objects.

#ifndef CRYPTOPALS_UTIL_PADDING_H_
#define CRYPTOPALS_UTIL_PADDING_H_

#include <cstdint>

#include "absl/status/status.h"
#include "cryptopals/util/bytes.h"

namespace cryptopals::util {

// Adds PKCS#7 padding to `input`, making the length a multiple of `block_size`.
void AddPkcs7Padding(Bytes& input, uint8_t block_size);

// Removes PKCS#7 padding from `input`.
absl::Status RemovePkcs7Padding(Bytes& input);

}  // namespace cryptopals::util

#endif

#ifndef CRYPTOPALS_UTIL_BYTES_UTIL_H_
#define CRYPTOPALS_UTIL_BYTES_UTIL_H_

#include <vector>

#include "cryptopals/util/bytes.h"

namespace cryptopals::util {

// Splits `input` into a vector of bytes, each of length `n`. For example,
// SplitBytes("ABCDEFGHIJ", 4) returns {"ABCD", "EFGH", "IJ"}.
std::vector<Bytes> SplitBytes(const Bytes& input, size_t n);

// Splits `input` into a vector of bytes, where each new bytes object contains
// every `n`th byte. For example, SplitAndTransposeBytes("ABCDEFGHIJ", 4)
// returns {"AEI", "BFJ", "CG", "DH"}.
std::vector<Bytes> SplitAndTransposeBytes(const Bytes& input, size_t n);

// Performs the reverse operation of SplitAndTransposeBytes. Note that this
// algorithm assumes that the first element of the vector is the longest.
Bytes JoinAndTransposeBytes(const std::vector<Bytes>& input);

}  // namespace cryptopals::util

#endif  // CRYPTOPALS_UTIL_BYTES_UTIL_H_

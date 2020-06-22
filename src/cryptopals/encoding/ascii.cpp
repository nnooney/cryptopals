#include "cryptopals/encoding/ascii.h"

namespace cryptopals::encoding {

using cryptopals::util::Bytes;

absl::flat_hash_map<uint8_t, size_t> AsciiEncoding::GenerateHistogram(
    const Bytes& input) const {
  absl::flat_hash_map<uint8_t, size_t> histogram;

  for (uint8_t byte : input) {
    if (histogram.contains(byte)) {
      histogram.at(byte)++;
    } else {
      histogram.insert(std::make_pair(byte, 1));
    }
  }
  return histogram;
}

}  // namespace cryptopals::encoding

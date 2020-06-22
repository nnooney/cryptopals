#ifndef CRYPTOPALS_ENCODING_ASCII_H_
#define CRYPTOPALS_ENCODING_ASCII_H_

#include "cryptopals/encoding/encoding.h"
#include "cryptopals/util/bytes.h"

namespace cryptopals::encoding {

class AsciiEncoding : public EncodingInterface<uint8_t> {
 public:
  // Implements GenerateHistogram from EncodingInterface.
  absl::flat_hash_map<uint8_t, size_t> GenerateHistogram(
      const cryptopals::util::Bytes& input) const override;
};

}  // namespace cryptopals::encoding

#endif  // CRYPTOPALS_ENCODING_ASCII_H_

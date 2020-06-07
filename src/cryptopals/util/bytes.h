#ifndef CRYPTOPALS_UTIL_BYTES_H_
#define CRYPTOPALS_UTIL_BYTES_H_

#include <cstdint>
#include <string_view>
#include <vector>

#include "cryptopals/proto/cryptopals_enums.pb.h"

namespace cryptopals::util {

class Bytes {
 public:
  // Constructs a Bytes object from a hex-encoded input `hex_str`. The Bytes
  // object is padded with 0s to the nearest byte.
  static Bytes CreateFromHex(const std::string_view hex_str);

  // Constructs a Bytes object from a base64-encoded input `base64_str`. The
  // Bytes object is padded with 0s to the nearest byte.
  static Bytes CreateFromBase64(const std::string_view base64_str);

  // Constructs a Bytes object from an encoded input `input_str` with the format
  // `format`. The Bytes object is padded with 0s to the nearest byte. If the
  // format is unrecognized, returns a default-constructed Bytes object.
  static Bytes CreateFromFormat(const std::string_view input_str,
                                cryptopals::BytesEncodedFormat format);

  // Returns a hex-encoded string representing the bytes of this object.
  std::string ToHex(void) const;

  // Returns a base64-encoded string representing the bytes of this object.
  std::string ToBase64(void) const;

  // Returns a `format` encoded string representing the bytes of this object. If
  // the format is unrecognized, returns the string "(unrecognized format)".
  std::string ToFormat(cryptopals::BytesEncodedFormat format) const;

  // Compares two Bytes objects for equality.
  friend inline bool operator==(const Bytes& lhs, const Bytes& rhs) {
    return lhs.data_ == rhs.data_;
  }

  // Applies the XOR operation to the bytes object. The operand is repeated so
  // that it is the same length as this object.
  Bytes& operator^=(const Bytes& rhs);

  // Returns the XOR of `lhs` and `rhs`. The length of the result is equal to
  // the length of lhs; the rhs may be truncated or repeated. For example,
  // ABCDEF ^ 1234 = B9F9FD.
  friend Bytes operator^(Bytes lhs, const Bytes& rhs);

 private:
  // A vector to hold the raw data contained in this class.
  std::vector<uint8_t> data_;
};

}  // namespace cryptopals::util

#endif  // CRYPTOPALS_UTIL_BYTES_H_

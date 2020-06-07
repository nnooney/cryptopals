#ifndef CRYPTOPALS_UTIL_BYTES_H_
#define CRYPTOPALS_UTIL_BYTES_H_

#include <cstdint>
#include <string_view>
#include <vector>

namespace cryptopals::util {

class Bytes {
 public:
  // Constructs a Bytes object from a hex-encoded input `hex_str`. The Bytes
  // object is padded with 0s to the nearest byte.
  static Bytes CreateFromHex(const std::string_view hex_str);

  // Constructs a Bytes object from a base64-encoded input `base64_str`. The
  // Bytes object is padded with 0s to the nearest byte.
  static Bytes CreateFromBase64(const std::string_view base64_str);

  // Returns a hex-encoded string representing the bytes of this object.
  std::string ToHex(void) const;

  // Returns a base64-encoded string representing the bytes of this object.
  std::string ToBase64(void) const;

 private:
  // A vector to hold the raw data contained in this class.
  std::vector<uint8_t> data_;
};

}  // namespace cryptopals::util

#endif  // CRYPTOPALS_UTIL_BYTES_H_

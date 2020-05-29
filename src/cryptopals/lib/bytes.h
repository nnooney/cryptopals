#include <cstdint>
#include <string_view>
#include <vector>

namespace cryptopals::lib::bytes {

// Takes a hex-encoded input `hex_str` and returns a vector of bytes
// representing its contents. The output is padded with 0s to the nearest byte.
std::vector<uint8_t> from_hex(const std::string_view hex_str);

// Takes a vector of bytes `bytes` and returns a hex-encoded string representing
// its contents.
std::string to_hex(const std::vector<uint8_t>& bytes);

// Takes a base64-encoded input `base64_str` and returns a vector of bytes
// representing its contents. The output is padded with 0s to the nearest byte.
std::vector<uint8_t> from_base64(const std::string_view base64_str);

// Takes a vector of bytes `bytes`  and returns a base64-encoded string
// representing its contents.
std::string to_base64(const std::vector<uint8_t>& bytes);

}  // namespace cryptopals::lib::bytes

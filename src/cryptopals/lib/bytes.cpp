#include "cryptopals/lib/bytes.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace cryptopals::lib::bytes {

constexpr std::string_view hex_chars = "0123456789abcdef";
constexpr std::string_view base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Calculates the number of bytes needed to decode a hex string of `length`
// chars.
inline constexpr size_t hex_to_bytes_size(size_t length) {
  return (length + 1) / 2;
}

// Calculates the number of chars needed to hex-encode a vector of `length`
// bytes.
inline constexpr size_t bytes_to_hex_size(size_t length) { return length * 2; }

// Calculates the number of bytes needed to decode a base64 string of `length`
// chars.
inline constexpr size_t base64_to_bytes_size(size_t length) {
  return (length + 3) / 4 * 3;
}

// Calculates the number of chars needed to base64-encode a vector of `length`
// bytes.
inline constexpr size_t bytes_to_base64_size(size_t length) {
  return length * 4 / 3;
}

std::vector<uint8_t> from_hex(const std::string_view hex_str) {
  std::vector<uint8_t> bytes;
  bytes.reserve(hex_to_bytes_size(hex_str.size()));

  for (size_t i = 0; i < hex_str.size(); i += 2) {
    uint8_t byte = hex_chars.find(hex_str.at(i)) << 4;
    if (i + 1 < hex_str.size()) {
      byte |= hex_chars.find(hex_str.at(i + 1));
    }
    bytes.push_back(byte);
  }

  return bytes;
}

std::string to_hex(const std::vector<uint8_t>& bytes) {
  std::string hex_str;
  hex_str.reserve(bytes_to_hex_size(bytes.size()));

  for (uint8_t byte : bytes) {
    hex_str.append(1, hex_chars.at(byte >> 4));
    hex_str.append(1, hex_chars.at(byte & 0x0F));
  }

  return hex_str;
}

std::vector<uint8_t> from_base64(const std::string_view base64_str) {
  std::vector<uint8_t> bytes;
  bytes.reserve(base64_to_bytes_size(base64_str.size()));

  for (size_t i = 0; i < base64_str.size(); i += 4) {
    uint32_t chunk = base64_chars.find(base64_str.at(i)) << 18;
    if (i + 1 < base64_str.size()) {
      chunk |= base64_chars.find(base64_str.at(i + 1)) << 12;
    }
    bytes.push_back(chunk >> 16 & 0xFF);

    if (i + 2 < base64_str.size()) {
      chunk |= base64_chars.find(base64_str.at(i + 2)) << 6;
    }
    if (i + 1 < base64_str.size()) {
      bytes.push_back(chunk >> 8 & 0xFF);
    }

    if (i + 3 < base64_str.size()) {
      chunk |= base64_chars.find(base64_str.at(i + 3));
    }
    if (i + 2 < base64_str.size()) {
      bytes.push_back(chunk & 0xFF);
    }
  }

  return bytes;
}

std::string to_base64(const std::vector<uint8_t>& bytes) {
  std::string base64_str;
  base64_str.reserve(bytes_to_base64_size(bytes.size()));

  for (size_t i = 0; i < bytes.size(); i += 3) {
    uint8_t chunk[4];

    chunk[0] = bytes[i] >> 2;
    base64_str.append(1, base64_chars.at(chunk[0]));

    chunk[1] = (bytes[i] & 0x03) << 4;
    if (i + 1 < bytes.size()) {
      chunk[1] |= bytes[i + 1] >> 4;
      chunk[2] = (bytes[i + 1] & 0x0F) << 2;
    }
    base64_str.append(1, base64_chars.at(chunk[1]));

    if (i + 2 < bytes.size()) {
      chunk[2] |= bytes[i + 2] >> 6;
      chunk[3] = bytes[i + 2] & 0x3F;
    }
    if (i + 1 < bytes.size()) {
      base64_str.append(1, base64_chars.at(chunk[2]));
    }

    if (i + 2 < bytes.size()) {
      base64_str.append(1, base64_chars.at(chunk[3]));
    }
  }

  return base64_str;
}

}  // namespace cryptopals::lib::bytes
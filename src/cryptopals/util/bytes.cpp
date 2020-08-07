#include "cryptopals/util/bytes.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "cryptopals/util/algorithm.h"

namespace cryptopals::util {
namespace {

constexpr std::string_view hex_chars = "0123456789abcdef";
constexpr std::string_view base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

// For each variable {1}_{2}_ratio, it represents the number of {1} required to
// form a length that is a multiple of {2}. A hex digit represents 4 bits, a
// base64 digit represents 6 bits, and a byte represents 8 bits.
constexpr size_t hex_bytes_ratio = 2;     // 4 * {2} == 8 == 1 mod 8
constexpr size_t bytes_base64_ratio = 3;  // 8 * {3} == 24 == 4 mod 6
constexpr size_t base64_bytes_ratio = 4;  // 6 * {4} == 24 == 3 mod 8

// Calculates the number of bytes needed to decode a hex string of `length`
// chars, including padding.
inline constexpr size_t HexToBytesSize(size_t length) {
  return CeilingMultiple(length, hex_bytes_ratio) / hex_bytes_ratio;
}

// Calculates the number of chars needed to hex-encode a vector of `length`
// bytes.
inline constexpr size_t BytesToHexSize(size_t length) { return length * 2; }

// Calculates the number of bytes needed to decode a base64 string of `length`
// chars, including padding.
inline constexpr size_t Base64ToBytesSize(size_t length) {
  return CeilingMultiple(length, base64_bytes_ratio) / base64_bytes_ratio;
}

// Calculates the number of chars needed to base64-encode a vector of `length`
// bytes, including padding.
inline constexpr size_t BytesToBase64Size(size_t length) {
  return CeilingMultiple(length, bytes_base64_ratio) / bytes_base64_ratio;
}

}  // namespace

Bytes Bytes::CreateFromFormat(const std::string_view input,
                              cryptopals::BytesEncodedFormat format) {
  switch (format) {
    case cryptopals::BytesEncodedFormat::BASE64:
      return cryptopals::util::Bytes::CreateFromBase64(input);
    case cryptopals::BytesEncodedFormat::HEX:
      return cryptopals::util::Bytes::CreateFromHex(input);
    case cryptopals::BytesEncodedFormat::RAW:
      return cryptopals::util::Bytes::CreateFromRaw(input);
    default: {
      Bytes bytes;
      return bytes;
    }
  }
}

Bytes Bytes::CreateFromBase64(const std::string_view input) {
  Bytes bytes;
  bytes.data_.reserve(Base64ToBytesSize(input.size()));
  bytes.format_ = cryptopals::BytesEncodedFormat::BASE64;

  // This mask ensures that any padding characters '=' get treated as all 0's.
  const uint32_t sextet_mask = 0x3F;

  for (size_t i = 0; i < input.size(); i += base64_bytes_ratio) {
    size_t chars_left = input.size() - i;
    // For each chunk of {4} encoded characters, compress the four characters
    // into three bytes. For example,
    //   AAAAAA BBBBBB CCCCCC DDDDDD becomes AAAAAABB BBBBCCCC CCDDDDDD
    uint32_t chunk = (base64_chars.find(input.at(i)) & sextet_mask) << 18;
    if (chars_left > 1) {
      chunk |= (base64_chars.find(input.at(i + 1)) & sextet_mask) << 12;
    }
    if (chars_left > 2) {
      chunk |= (base64_chars.find(input.at(i + 2)) & sextet_mask) << 6;
    }
    if (chars_left > 3) {
      chunk |= base64_chars.find(input.at(i + 3)) & sextet_mask;
    }

    bytes.data_.push_back(chunk >> 16 & 0xFF);
    if (chars_left > 1 &&
        (chars_left <= 2 || input.at(i + 2) != base64_chars.back())) {
      bytes.data_.push_back(chunk >> 8 & 0xFF);
    }
    if (chars_left > 2 &&
        (chars_left <= 3 || input.at(i + 3) != base64_chars.back())) {
      bytes.data_.push_back(chunk & 0xFF);
    }
  }

  return bytes;
}

Bytes Bytes::CreateFromHex(const std::string_view input) {
  Bytes bytes;
  bytes.data_.reserve(HexToBytesSize(input.size()));
  bytes.format_ = cryptopals::BytesEncodedFormat::HEX;

  for (size_t i = 0; i < input.size(); i += hex_bytes_ratio) {
    byte_type byte = hex_chars.find(input.at(i)) << 4;
    if (i + 1 < input.size()) {
      byte |= hex_chars.find(input.at(i + 1));
    }
    bytes.data_.push_back(byte);
  }

  return bytes;
}

Bytes Bytes::CreateFromRaw(const std::string_view input) {
  Bytes bytes(input.begin(), input.end());
  bytes.format_ = cryptopals::BytesEncodedFormat::RAW;
  return bytes;
}

std::string Bytes::ToFormat(cryptopals::BytesEncodedFormat format) const {
  switch (format) {
    case cryptopals::BytesEncodedFormat::BASE64:
      return ToBase64();
    case cryptopals::BytesEncodedFormat::HEX:
      return ToHex();
    case cryptopals::BytesEncodedFormat::RAW:
      return ToRaw();
    default:
      return "(unrecognized bytes format)";
  }
}

std::string Bytes::ToBase64() const {
  std::string result;
  result.reserve(BytesToBase64Size(data_.size()));

  for (size_t i = 0; i < data_.size(); i += bytes_base64_ratio) {
    size_t bytes_left = data_.size() - i;
    // For each chunk of {3} bytes, decompress the three 8-bit values into four
    // 6-bit values. For example,
    //   AAAAAAAA BBBBBBBB CCCCCCCC becomes 00AAAAAA 00AABBBB 00BBBBCC 00CCCCCC
    uint32_t chunk = (data_[i] >> 2) << 24;
    chunk |= (data_[i] & 0x03) << 20;
    if (bytes_left > 1) {
      chunk |= (data_[i + 1] >> 4) << 16;
      chunk |= (data_[i + 1] & 0x0F) << 10;
    }
    if (bytes_left > 2) {
      chunk |= (data_[i + 2] >> 6) << 8;
      chunk |= (data_[i + 2] & 0x3F);
    }

    result.append(1, base64_chars.at(chunk >> 24 & 0xFF));
    result.append(1, base64_chars.at(chunk >> 16 & 0xFF));
    if (bytes_left > 1) {
      result.append(1, base64_chars.at(chunk >> 8 & 0xFF));
    }

    if (bytes_left > 2) {
      result.append(1, base64_chars.at(chunk & 0xFF));
    }
  }

  // Add any padding chars if necessary.
  result.append((4 - result.size() % 4) % 4, base64_chars.back());

  return result;
}

std::string Bytes::ToHex() const {
  std::string result;
  result.reserve(BytesToHexSize(data_.size()));

  for (byte_type byte : data_) {
    result.append(1, hex_chars.at(byte >> 4));
    result.append(1, hex_chars.at(byte & 0x0F));
  }

  return result;
}

std::string Bytes::ToRaw(void) const {
  return std::string(data_.begin(), data_.end());
}

Bytes& Bytes::operator^=(const Bytes& rhs) {
  size_t i = 0;
  for (byte_type& byte : data_) {
    byte ^= rhs.data_.at(i % rhs.data_.size());
    ++i;
  }
  return *this;
}

}  // namespace cryptopals::util

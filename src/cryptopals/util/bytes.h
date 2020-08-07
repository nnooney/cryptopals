#ifndef CRYPTOPALS_UTIL_BYTES_H_
#define CRYPTOPALS_UTIL_BYTES_H_

#include <compare>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <vector>

#include "cryptopals/proto/cryptopals_enums.pb.h"

namespace cryptopals::util {

class Bytes {
 public:
  using byte_type = uint8_t;
  using data_type = std::vector<byte_type>;

  // The constructors are only made public to support default constructors for
  // aggregates. Prefer using a Create* function below to initialize a single
  // object.
  // Default constructor.
  Bytes() = default;
  // A constructor to create a Bytes object with `size` bytes.
  explicit Bytes(data_type::size_type size) : data_(size) {}
  // A constructor to create a Bytes object with the contents of the range
  // [first, last).
  template <class InputIt>
  Bytes(InputIt first, InputIt last) : data_(first, last) {}

  // Create a Bytes object from an input string `input` with the encoding
  // `format`. The Bytes object is padded with 0s to the nearest byte.
  // Convenience functions are also provided for each supported format. The
  // format used to create the Bytes object is saved to `format_`.
  static Bytes CreateFromFormat(const std::string_view input,
                                cryptopals::BytesEncodedFormat format);
  static Bytes CreateFromBase64(const std::string_view input);
  static Bytes CreateFromHex(const std::string_view input);
  static Bytes CreateFromRaw(const std::string_view input);

  // Create a Bytes object from any integral type.
  template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  static Bytes CreateFromIntegral(const T input) {
    Bytes bytes;
    bytes.data_.reserve(sizeof(T));
    bytes.format_ = cryptopals::BytesEncodedFormat::RAW;
    for (size_t i = 0; i < sizeof(T); ++i) {
      bytes.data_.insert(bytes.data_.begin(), input >> (i * 8));
    }
    return bytes;
  }

  // Create a Bytes object from a range.
  template <class InputIt>
  static Bytes CreateFromRange(InputIt first, InputIt last) {
    return Bytes(first, last);
  }

  // Appends the `other` Bytes object to this Bytes object.
  inline void Append(const Bytes& other) {
    data_.insert(data_.end(), other.begin(), other.end());
  }

  // Sets the `format` of the Bytes object for printing.
  inline void SetFormat(cryptopals::BytesEncodedFormat format) {
    format_ = format;
  }

  // Returns a std::string with the encoding `format` representing the bytes
  // of this object. Convenience functions are also provided for each supported
  // format.
  std::string ToFormat(cryptopals::BytesEncodedFormat format) const;
  std::string ToBase64(void) const;
  std::string ToHex(void) const;
  std::string ToRaw(void) const;

  // Support for comparing Bytes objects.
  friend std::weak_ordering operator<=>(const Bytes& lhs, const Bytes& rhs) {
    return lhs.data_ <=> rhs.data_;
  }
  friend inline bool operator==(const Bytes& lhs, const Bytes& rhs) {
    return lhs.data_ == rhs.data_;
  }

  // Applies the XOR operation to the bytes object. The `rhs` operand is
  // repeated so that it is the same length as this object.
  Bytes& operator^=(const Bytes& rhs);

  // Returns the XOR of `lhs` and `rhs`. The length of the result is equal to
  // the length of `lhs`; the `rhs` operand may be truncated or repeated. For
  // example, ABCDEF ^ 1234 = B9F9FD.
  friend inline Bytes operator^(Bytes lhs, const Bytes& rhs) {
    lhs ^= rhs;
    return lhs;
  }

  // Support for streamed printing of Bytes objects.
  friend std::ostream& operator<<(std::ostream& os, const Bytes& obj) {
    os << obj.ToFormat(obj.format_);
    return os;
  }

  // The following functions support using a Bytes object like a raw vector of
  // data.
  inline data_type::iterator begin() { return data_.begin(); }
  inline data_type::const_iterator begin() const { return data_.begin(); }
  inline data_type::iterator end() { return data_.end(); }
  inline data_type::const_iterator end() const { return data_.end(); }
  inline data_type::size_type size() const noexcept { return data_.size(); }
  inline data_type::reference at(data_type::size_type pos) {
    return data_.at(pos);
  }
  inline data_type::const_reference at(data_type::size_type pos) const {
    return data_.at(pos);
  }
  inline void push_back(data_type::const_reference value) {
    data_.push_back(value);
  }

 private:
  // A vector to hold the raw data contained in this class.
  data_type data_;
  // A type to hold the encoded format (used in printing).
  cryptopals::BytesEncodedFormat format_ = cryptopals::HEX;
};

}  // namespace cryptopals::util

#endif  // CRYPTOPALS_UTIL_BYTES_H_

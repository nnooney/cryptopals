#include "cryptopals/util/padding.h"

#include <algorithm>
#include <iostream>

namespace cryptopals::util {

void AddPkcs7Padding(Bytes& input, uint8_t block_size) {
  uint8_t padding_size = block_size - (input.size() % block_size);
  input.Append(Bytes::CreateFromRaw(
      std::string(padding_size, static_cast<char>(padding_size))));
}

// Removes PKCS#7 padding from `input`.
absl::Status RemovePkcs7Padding(Bytes& input) {
  Bytes::byte_type last_byte = input.back();

  if (!std::all_of(input.end() - static_cast<size_t>(last_byte), input.end(),
                   [=](Bytes::byte_type i) { return i == last_byte; })) {
    return absl::InvalidArgumentError("input has malformed PKCS#7 padding");
  }

  std::cerr << "Removing " << static_cast<size_t>(last_byte)
            << " bytes from input";

  input.Resize(input.size() - static_cast<size_t>(last_byte));
  return absl::OkStatus();
}

}  // namespace cryptopals::util

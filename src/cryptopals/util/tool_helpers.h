#ifndef CRYPTOPALS_UTIL_TOOL_HELPERS_H_
#define CRYPTOPALS_UTIL_TOOL_HELPERS_H_

#include <string>
#include <string_view>
#include <vector>

#include "absl/status/statusor.h"
#include "cryptopals/util/string_utils.h"
#include "cryptopals_enums.pb.h"

namespace cryptopals::util {

// Provides a typedef for the Enum_Parse function generated for every enum by
// the protobuf compiler.
template <typename EnumType>
using EnumParseFunc = bool (*)(const std::string& name, EnumType* value);

// Parses an enumeration of EnumType from a string by converting `input` to
// uppercase (in accordance with the protobuf style guide, see
// https://developers.google.com/protocol-buffers/docs/style#enums) and using
// the provided `Enum_Parse` function to attempt convert it to a value.
template <typename EnumType>
absl::StatusOr<EnumType> ParseEnumFromString(
    std::string_view input, EnumParseFunc<EnumType> Enum_Parse) {
  if (input.empty()) {
    return absl::InvalidArgumentError("cannot parse enum from empty input");
  }
  std::string temp(input.data(), input.size());
  StrToUpper(&temp);

  EnumType result;
  bool rc = Enum_Parse(temp, &result);
  if (!rc) {
    return absl::InvalidArgumentError("parsing enum failed from input: ");
  }
  return result;
}

// Returns the input parsed from the provided input method.
std::vector<std::string> GetInputsForMethod(std::vector<char*> inputs,
                                            cryptopals::InputMethod method);

}  // namespace cryptopals::util

#endif  // CRYPTOPALS_UTIL_TOOL_HELPERS_H_

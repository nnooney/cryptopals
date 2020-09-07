// Functions used to adapt absl::Status to other contexts. Particularly useful
// with StatusBuilder.With() functions.

#ifndef CRYPTOPALS_UTIL_STATUS_ADAPTORS_H_
#define CRYPTOPALS_UTIL_STATUS_ADAPTORS_H_

#include "absl/status/status.h"

namespace cryptopals::util {

template <typename T>
auto Return(T value) {
  return [&](absl::Status status) { return value; };
}

}  // namespace cryptopals::util

#endif  // CRYPTOPALS_UTIL_STATUS_ADAPTORS_H_

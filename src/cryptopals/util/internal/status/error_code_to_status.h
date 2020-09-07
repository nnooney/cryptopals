// This is a modified version of the xls file
// xls/common/status/error_code_to_status.h obtained from
// https://github.com/google/xls. Modifications are made to make this class
// compatible with Abseil and to simplify namespaces.

// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CRYPTOPALS_UTIL_INTERNAL_STATUS_ERROR_CODE_TO_STATUS_H_
#define CRYPTOPALS_UTIL_INTERNAL_STATUS_ERROR_CODE_TO_STATUS_H_

#include <system_error>  // NOLINT(build/c++11)

#include "absl/status/status.h"
#include "cryptopals/util/internal/status/status_builder.h"

namespace absl {

absl::StatusCode ErrorCodeToStatusCode(const std::error_code& ec);

absl::StatusBuilder ErrorCodeToStatus(const std::error_code& ec);

// Converts an `errno` value into an absl::Status.
absl::StatusBuilder ErrnoToStatus(int errno_value);

}  // namespace absl

#endif  // CRYPTOPALS_UTIL_INTERNAL_STATUS_ERROR_CODE_TO_STATUS_H_

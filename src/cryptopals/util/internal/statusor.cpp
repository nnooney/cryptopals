// This is a modified version of the iree file
// iree/base/internal/statusor.cpp obtained from
// https://github.com/google/iree. Modifications are made to make
// this class compatible with absl::Status and to simplify namespaces.

// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cryptopals/util/internal/statusor.h"

#include "absl/status/status.h"
#include "cryptopals/util/logging.h"

namespace absl {

namespace internal_statusor {

void Helper::HandleInvalidStatusCtorArg(Status* status) {
  const char* kMessage =
      "An OK status is not a valid constructor argument to StatusOr<T>";
  LOG(ERROR) << kMessage;
  *status = InternalError(kMessage);
  abort();
}

void Helper::Crash(const Status& status) {
  LOG(FATAL) << "Attempting to fetch value instead of handling error "
             << status;
  abort();
}

}  // namespace internal_statusor

}  // namespace absl

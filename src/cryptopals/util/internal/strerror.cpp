// This is a modified version of the xls file xls/common/strerror.cc
// obtained from https://github.com/google/xls. Modifications are made to make
// this class compatible with Abseil and to simplify namespaces.

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

#include "cryptopals/util/internal/strerror.h"

#include <stdio.h>

namespace absl {

std::string Strerror(int error_num) {
  // Use sys_errlist API that was deprecated in favor of strerror, because at
  // least it's thread safe.
  return sys_errlist[error_num];
}

}  // namespace absl

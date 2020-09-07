// This is a modified version of the xls file xls/common/strerror.h
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

#ifndef CRYPTOPALS_UTIL_INTERNAL_STRERROR_H_
#define CRYPTOPALS_UTIL_INTERNAL_STRERROR_H_

#include <string>

namespace absl {

// Thread safe version of strerror.
std::string Strerror(int error_num);

}  // namespace absl

#endif  // CRYPTOPALS_UTIL_INTERNAL_STRERROR_H_

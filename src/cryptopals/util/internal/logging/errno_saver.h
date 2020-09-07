// This is a modified version of the xls file xls/common/logging/errno_saver.h
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

#ifndef CRYPTOPALS_UTIL_INTERNAL_LOGGING_ERRNO_SAVER_H_
#define CRYPTOPALS_UTIL_INTERNAL_LOGGING_ERRNO_SAVER_H_

#include <cerrno>

namespace absl {
namespace logging_internal {

// `ErrnoSaver` captures the value of `errno` upon construction and restores it
// upon deletion.  It is used in low-level code and must be super fast.  Do not
// add instrumentation, even in debug modes.
class ErrnoSaver {
 public:
  ErrnoSaver() : saved_errno_(errno) {}
  ~ErrnoSaver() { errno = saved_errno_; }
  int operator()() const { return saved_errno_; }

 private:
  const int saved_errno_;
};

}  // namespace logging_internal
}  // namespace absl

#endif  // CRYPTOPALS_UTIL_INTERNAL_LOGGING_ERRNO_SAVER_H_

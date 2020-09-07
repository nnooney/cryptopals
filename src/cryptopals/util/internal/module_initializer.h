// This is a modified version of the xls file xls/common/module_initializer.h
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

// This file provides helpers for initialization code for global objects/state.
// This library should be used for all initialization for global/static objects.
// Usage example:
//
//    static const char* hostname = nullptr;
//    REGISTER_MODULE_INITIALIZER(my_hostname, {
//      // Code to initialize "hostname".
//    });
//
// Note that, due to preprocessor weirdness, there may be issues with the use
// of commas in your initialization code.  If you run into them, try using
// parens around the commas, or use a helper function as follows:
//
//    static const char* hostname = nullptr;
//    static void InitHostname() {
//      // Code to initialize "hostname".
//    }
//    REGISTER_MODULE_INITIALIZER(my_hostname, InitHostname());
//
// This also helps the compiler to accurately attribute compilation errors to
// pieces of your initialization code.

#ifndef CRYPTOPALS_UTIL_INTERNAL_MODULE_INITIALIZER_H_
#define CRYPTOPALS_UTIL_INTERNAL_MODULE_INITIALIZER_H_

namespace absl {
namespace module_initializer_internal {

class ModuleInitializer {
 public:
  typedef void (*void_function)();
  ModuleInitializer(const char*, void_function f) { f(); }
};

}  // namespace module_initializer_internal
}  // namespace absl

#define REGISTER_MODULE_INITIALIZER(name, body)                           \
  namespace {                                                             \
  static void module_init_module_##name() { body; }                       \
  ::absl::module_initializer_internal::ModuleInitializer                  \
      module_initializer_module_##name(#name, module_init_module_##name); \
  }

#endif  // CRYPTOPALS_UTIL_INTERNAL_MODULE_INITIALIZER_H_

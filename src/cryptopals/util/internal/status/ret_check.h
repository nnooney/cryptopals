// This is a modified version of the xls file xls/common/status/ret_check.h
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

// Macros for non-fatal assertions.  The `RET_CHECK` family of macros return
// an absl::Status with code `absl::StatusCode::kInternal` from the current
// method.
//
//   RET_CHECK(ptr != nullptr);
//   RET_CHECK_GT(value, 0) << "Optional additional message";
//   RET_CHECK_FAIL() << "Always fails";
//   RET_CHECK_OK(status) << "If status is not OK, return an internal
//   error";
//
// The RET_CHECK* macros can only be used in functions that return
// absl::Status or StatusOr.  The generated `absl::Status` will contain the
// string "RET_CHECK failure".
//
// On failure these routines will log a stack trace to `ERROR`.  The
// `RET_CHECK` macros end with a `StatusBuilder` in their tail position and
// can be customized like calls to `RETURN_IF_ERROR`.
//
// Be careful with the usage of RET_CHECK_* for checking user sensitive data
// since it logs the underlying input values on failure. RET_CHECK is a
// safer way for this situation because it just logs the input condition as a
// string literal on failure.

#ifndef CRYPTOPALS_UTIL_INTERNAL_STATUS_RET_CHECK_H_
#define CRYPTOPALS_UTIL_INTERNAL_STATUS_RET_CHECK_H_

#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>

#include "absl/flags/declare.h"
#include "absl/status/status.h"
#include "cryptopals/util/internal/source_location.h"
#include "cryptopals/util/internal/status/status_builder.h"
#include "cryptopals/util/internal/status/status_macros.h"
#include "cryptopals/util/internal/status/statusor.h"

namespace absl {
namespace internal_status_macros_ret_check {

// Returns a StatusBuilder that corresponds to a `RET_CHECK` failure.
absl::StatusBuilder RetCheckFailSlowPath(absl::SourceLocation location);
absl::StatusBuilder RetCheckFailSlowPath(absl::SourceLocation location,
                                         const char* condition);
absl::StatusBuilder RetCheckFailSlowPath(absl::SourceLocation location,
                                         const char* condition,
                                         const absl::Status& s);

// Takes ownership of `condition`.  This API is a little quirky because it is
// designed to make use of the `::Check_*Impl` methods that implement `CHECK_*`
// and `DCHECK_*`.
absl::StatusBuilder RetCheckFailSlowPath(absl::SourceLocation location,
                                         std::string* condition);

inline absl::StatusBuilder RetCheckImpl(const absl::Status& status,
                                        const char* condition,
                                        absl::SourceLocation location) {
  if (ABSL_PREDICT_TRUE(status.ok()))
    return absl::StatusBuilder(absl::OkStatus(), location);
  return RetCheckFailSlowPath(location, condition, status);
}

inline const absl::Status& AsStatus(const absl::Status& status) {
  return status;
}

template <typename T>
inline const absl::Status& AsStatus(const absl::StatusOr<T>& status_or) {
  return status_or.status();
}

// A helper class for formatting `expr (V1 vs. V2)` in a `RET_CHECK_XX`
// statement.  See `MakeCheckOpString` for sample usage.
class CheckOpMessageBuilder {
 public:
  // Inserts `exprtext` and ` (` to the stream.
  explicit CheckOpMessageBuilder(const char* exprtext);
  // Deletes `stream_`.
  ~CheckOpMessageBuilder();
  // For inserting the first variable.
  std::ostream* ForVar1() { return stream_; }
  // For inserting the second variable (adds an intermediate ` vs. `).
  std::ostream* ForVar2();
  // Get the result (inserts the closing `)`).
  std::string* NewString();

 private:
  std::ostringstream* stream_;
};

// This formats a value for a failing `RET_CHECK_XX` statement.  Ordinarily,
// it uses the definition for `operator<<`, with a few special cases below.
template <typename T>
inline void MakeCheckOpValueString(std::ostream* os, const T& v) {
  (*os) << v;
}

// Overrides for char types provide readable values for unprintable characters.
void MakeCheckOpValueString(std::ostream* os, char v);
void MakeCheckOpValueString(std::ostream* os, signed char v);
void MakeCheckOpValueString(std::ostream* os, unsigned char v);

// We need an explicit specialization for `std::nullptr_t`.
void MakeCheckOpValueString(std::ostream* os, std::nullptr_t v);
void MakeCheckOpValueString(std::ostream* os, const char* v);
void MakeCheckOpValueString(std::ostream* os, const signed char* v);
void MakeCheckOpValueString(std::ostream* os, const unsigned char* v);
void MakeCheckOpValueString(std::ostream* os, char* v);
void MakeCheckOpValueString(std::ostream* os, signed char* v);
void MakeCheckOpValueString(std::ostream* os, unsigned char* v);

// Build the error message string.  Specify no inlining for code size.
template <typename T1, typename T2>
std::string* MakeCheckOpString(const T1& v1, const T2& v2,
                               const char* exprtext) ABSL_ATTRIBUTE_NOINLINE;

template <typename T1, typename T2>
std::string* MakeCheckOpString(const T1& v1, const T2& v2,
                               const char* exprtext) {
  CheckOpMessageBuilder comb(exprtext);
  ::absl::internal_status_macros_ret_check::MakeCheckOpValueString(
      comb.ForVar1(), v1);
  ::absl::internal_status_macros_ret_check::MakeCheckOpValueString(
      comb.ForVar2(), v2);
  return comb.NewString();
}

// Helper functions for `COMMON_MACROS_INTERNAL_RET_CHECK_OP`
// macro.  The `(int, int)` specialization works around the issue that the
// compiler will not instantiate the template version of the function on values
// of unnamed enum type - see comment below.
#define COMMON_MACROS_INTERNAL_RET_CHECK_OP(name, op)                      \
  template <typename T1, typename T2>                                      \
  inline std::string* name##Impl(const T1& v1, const T2& v2,               \
                                 const char* exprtext) {                   \
    if (ABSL_PREDICT_TRUE(v1 op v2))                                       \
      return nullptr;                                                      \
    else                                                                   \
      return ::absl::internal_status_macros_ret_check::MakeCheckOpString(  \
          v1, v2, exprtext);                                               \
  }                                                                        \
  inline std::string* name##Impl(int v1, int v2, const char* exprtext) {   \
    return ::absl::internal_status_macros_ret_check::name##Impl<int, int>( \
        v1, v2, exprtext);                                                 \
  }

COMMON_MACROS_INTERNAL_RET_CHECK_OP(Check_EQ, ==)
COMMON_MACROS_INTERNAL_RET_CHECK_OP(Check_NE, !=)
COMMON_MACROS_INTERNAL_RET_CHECK_OP(Check_LE, <=)
COMMON_MACROS_INTERNAL_RET_CHECK_OP(Check_LT, <)
COMMON_MACROS_INTERNAL_RET_CHECK_OP(Check_GE, >=)
COMMON_MACROS_INTERNAL_RET_CHECK_OP(Check_GT, >)
#undef COMMON_MACROS_INTERNAL_RET_CHECK_OP

// `RET_CHECK_EQ` and friends want to pass their arguments by reference,
// however this winds up exposing lots of cases where people have defined and
// initialized static const data members but never declared them (i.e. in a .cc
// file), meaning they are not referenceable.  This function avoids that problem
// for integers (the most common cases) by overloading for every primitive
// integer type, even the ones we discourage, and returning them by value.
template <typename T>
inline const T& GetReferenceableValue(const T& t) {
  return t;
}
inline char GetReferenceableValue(char t) { return t; }
inline unsigned char GetReferenceableValue(unsigned char t) { return t; }
inline signed char GetReferenceableValue(signed char t) { return t; }
inline short GetReferenceableValue(short t) {  // NOLINT: runtime/int
  return t;
}
inline unsigned short GetReferenceableValue(  // NOLINT: runtime/int
    unsigned short t) {                       // NOLINT: runtime/int
  return t;
}
inline int GetReferenceableValue(int t) { return t; }
inline unsigned int GetReferenceableValue(unsigned int t) { return t; }
inline long GetReferenceableValue(long t)  // NOLINT: runtime/int
{
  return t;
}
inline unsigned long GetReferenceableValue(  // NOLINT: runtime/int
    unsigned long t) {                       // NOLINT: runtime/int
  return t;
}
inline long long GetReferenceableValue(long long t) {  // NOLINT: runtime/int
  return t;
}
inline unsigned long long GetReferenceableValue(  // NOLINT: runtime/int
    unsigned long long t) {                       // NOLINT: runtime/int
  return t;
}

}  // namespace internal_status_macros_ret_check
}  // namespace absl

#define RET_CHECK(cond)                                                  \
  while (ABSL_PREDICT_FALSE(!(cond)))                                    \
  return ::absl::internal_status_macros_ret_check::RetCheckFailSlowPath( \
      ABSL_LOC, #cond)

#define RET_CHECK_FAIL()                                                 \
  return ::absl::internal_status_macros_ret_check::RetCheckFailSlowPath( \
      ABSL_LOC)

// Takes an expression returning absl::Status and asserts that the status is
// `ok()`.  If not, it returns an internal error.
//
// This is similar to `RETURN_IF_ERROR` in that it propagates errors.  The
// difference is that it follows the behavior of `RET_CHECK`, returning an
// internal error (wrapping the original error text), including the filename and
// line number, and logging a stack trace.
//
// This is appropriate to use to write an assertion that a function that returns
// `absl::Status` cannot fail, particularly when the error code itself should
// not be surfaced.
#define RET_CHECK_OK(status)                                               \
  RETURN_IF_ERROR(::absl::internal_status_macros_ret_check::RetCheckImpl(  \
      ::absl::internal_status_macros_ret_check::AsStatus(status), #status, \
      ABSL_LOC))

#if defined(STATIC_ANALYSIS) || defined(PORTABLE_STATUS)
#define COMMON_MACROS_INTERNAL_RET_CHECK_OP(name, op, lhs, rhs) \
  RET_CHECK((lhs)op(rhs))
#else
#define COMMON_MACROS_INTERNAL_RET_CHECK_OP(name, op, lhs, rhs)                \
  while (                                                                      \
      std::string* _result =                                                   \
          ::absl::internal_status_macros_ret_check::Check_##name##Impl(        \
              ::absl::internal_status_macros_ret_check::GetReferenceableValue( \
                  lhs),                                                        \
              ::absl::internal_status_macros_ret_check::GetReferenceableValue( \
                  rhs),                                                        \
              #lhs " " #op " " #rhs))                                          \
  return ::absl::internal_status_macros_ret_check::RetCheckFailSlowPath(       \
      ABSL_LOC, _result)
#endif

#define RET_CHECK_EQ(lhs, rhs) \
  COMMON_MACROS_INTERNAL_RET_CHECK_OP(EQ, ==, lhs, rhs)
#define RET_CHECK_NE(lhs, rhs) \
  COMMON_MACROS_INTERNAL_RET_CHECK_OP(NE, !=, lhs, rhs)
#define RET_CHECK_LE(lhs, rhs) \
  COMMON_MACROS_INTERNAL_RET_CHECK_OP(LE, <=, lhs, rhs)
#define RET_CHECK_LT(lhs, rhs) \
  COMMON_MACROS_INTERNAL_RET_CHECK_OP(LT, <, lhs, rhs)
#define RET_CHECK_GE(lhs, rhs) \
  COMMON_MACROS_INTERNAL_RET_CHECK_OP(GE, >=, lhs, rhs)
#define RET_CHECK_GT(lhs, rhs) \
  COMMON_MACROS_INTERNAL_RET_CHECK_OP(GT, >, lhs, rhs)

#endif  // CRYPTOPALS_UTIL_INTERNAL_STATUS_RET_CHECK_H_

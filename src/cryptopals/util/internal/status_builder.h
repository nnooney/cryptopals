// This is a modified version of the iree file
// iree/base/internal/status_builder.h obtained from
// https://github.com/google/iree. Modifications are made to make
// this class compatible with absl::Status and to simplify namespaces.
// The function Annotate was imported from iree/base/internal/status.h as well.

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

#ifndef ABSL_BASE_INTERNAL_STATUS_BUILDER_H_
#define ABSL_BASE_INTERNAL_STATUS_BUILDER_H_

#include "absl/status/status.h"
#include "absl/strings/internal/ostringstream.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "cryptopals/util/internal/source_location.h"

namespace absl {

// Creates a status based on an original_status, but enriched with additional
// information. The builder implicitly converts to Status and StatusOr<T>
// allowing for it to be returned directly.
class ABSL_MUST_USE_RESULT StatusBuilder {
 public:
  // Creates a `StatusBuilder` based on an original status.
  explicit StatusBuilder(const Status& original_status,
                         SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
  explicit StatusBuilder(Status&& original_status,
                         SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);

  // Creates a `StatusBuilder` from a status code.
  // A typical user will not specify `location`, allowing it to default to the
  // current location.
  explicit StatusBuilder(StatusCode code,
                         SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);

  StatusBuilder(const StatusBuilder& sb);
  StatusBuilder& operator=(const StatusBuilder& sb);
  StatusBuilder(StatusBuilder&&) = default;
  StatusBuilder& operator=(StatusBuilder&&) = default;

  // Appends to the extra message that will be added to the original status.
  template <typename T>
  StatusBuilder& operator<<(const T& value) &;
  template <typename T>
  StatusBuilder&& operator<<(const T& value) &&;

  // No-op functions that may be added later.
  StatusBuilder& LogError() & { return *this; }
  StatusBuilder&& LogError() && { return std::move(LogError()); }
  StatusBuilder& LogWarning() & { return *this; }
  StatusBuilder&& LogWarning() && { return std::move(LogWarning()); }
  StatusBuilder& LogInfo() & { return *this; }
  StatusBuilder&& LogInfo() && { return std::move(LogInfo()); }

  // Returns true if the Status created by this builder will be ok().
  bool ok() const;

  // Returns the error code for the Status created by this builder.
  StatusCode code() const;

  // Returns the source location used to create this builder.
  SourceLocation source_location() const;

  // Implicit conversion to Status.
  operator Status() const&;
  operator Status() &&;

 private:
  Status CreateStatus() &&;

  static Status JoinMessageToStatus(Status s, absl::string_view msg);

  // The status that the result will be based on.
  Status status_;

  // The location to record if this status is logged.
  SourceLocation loc_;

  // Lazy construction of the expensive stream.
  struct Rep {
    explicit Rep() = default;
    Rep(const Rep& r);

    // Gathers additional messages added with `<<` for use in the final status.
    std::string stream_message;
    absl::strings_internal::OStringStream stream{&stream_message};
  };

  std::unique_ptr<Rep> rep_;
};

inline StatusBuilder::StatusBuilder(const StatusBuilder& sb)
    : status_(sb.status_), loc_(sb.loc_) {
  if (sb.rep_ != nullptr) {
    rep_ = absl::make_unique<Rep>(*sb.rep_);
  }
}

inline StatusBuilder& StatusBuilder::operator=(const StatusBuilder& sb) {
  status_ = sb.status_;
  loc_ = sb.loc_;
  if (sb.rep_ != nullptr) {
    rep_ = absl::make_unique<Rep>(*sb.rep_);
  } else {
    rep_ = nullptr;
  }
  return *this;
}

template <typename T>
StatusBuilder& StatusBuilder::operator<<(const T& value) & {
  if (status_.ok()) return *this;
  if (rep_ == nullptr) rep_ = absl::make_unique<Rep>();
  rep_->stream << value;
  return *this;
}
template <typename T>
StatusBuilder&& StatusBuilder::operator<<(const T& value) && {
  return std::move(operator<<(value));
}

// Implicitly converts `builder` to `Status` and write it to `os`.
std::ostream& operator<<(std::ostream& os, const StatusBuilder& builder);
std::ostream& operator<<(std::ostream& os, StatusBuilder&& builder);

// Each of the functions below creates StatusBuilder with a canonical error.
// The error code of the StatusBuilder matches the name of the function.
StatusBuilder AbortedErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder AlreadyExistsErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder CancelledErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder DataLossErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder DeadlineExceededErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder FailedPreconditionErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder InternalErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder InvalidArgumentErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder NotFoundErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder OutOfRangeErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder PermissionDeniedErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder UnauthenticatedErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder ResourceExhaustedErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder UnavailableErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder UnimplementedErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);
StatusBuilder UnknownErrorBuilder(
    SourceLocation location ABSL_LOC_CURRENT_DEFAULT_ARG);

// Returns a Status that is identical to `s` except that the message()
// has been augmented by adding `msg` to the end of the original message.
Status Annotate(const Status& s, absl::string_view msg);

}  // namespace absl

#endif  // ABSL_BASE_INTERNAL_STATUS_BUILDER_H_
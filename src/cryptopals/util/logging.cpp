// Initialize logging facilities. This file attempts to make logging as similar
// as possible to Google's logging. See
// https://hpc.nih.gov/development/glog.html for details about Google's logging.

#include "cryptopals/util/logging.h"

#include <cstring>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/time/time.h"
#include "loguru.hpp"

ABSL_FLAG(bool, logtostderr, false,
          "log messages go to stderr instead of logfiles");
ABSL_FLAG(bool, alsologtostderr, false,
          "log messages go to stderr in addition to logfiles");

namespace cryptopals::util {

void InitLogging(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  bool logtostderr = absl::GetFlag(FLAGS_logtostderr);
  bool alsologtostderr = absl::GetFlag(FLAGS_alsologtostderr);

  if (logtostderr || alsologtostderr) {
    loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
  } else {
    loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
  }

  loguru::init(argc, argv);

  if (!logtostderr) {
    const char* slash = strrchr(argv[0], '/');
    const char* short_program_name = slash ? slash + 1 : argv[0];

    absl::Time now = absl::Now();
    absl::TimeZone utc = absl::UTCTimeZone();

    std::string logfile_name =
        absl::StrJoin({short_program_name, "log", "%s",
                       absl::FormatTime("%Y%m%d-%H%M%S", now, utc).c_str()},
                      ".");
    std::string logfile_path = absl::StrJoin({"/tmp", logfile_name.c_str()}, "/");
    std::string_view logfile_template(logfile_path);

    // Setup logging to /tmp/<program_name>.log.<severity_level>.<date>.<time>
    loguru::add_file(absl::StrFormat(logfile_template, "INFO").c_str(),
                     loguru::Truncate, loguru::Verbosity_INFO);
    loguru::add_file(absl::StrFormat(logfile_template, "WARNING").c_str(),
                     loguru::Truncate, loguru::Verbosity_WARNING);
    loguru::add_file(absl::StrFormat(logfile_template, "ERROR").c_str(),
                     loguru::Truncate, loguru::Verbosity_ERROR);
    loguru::add_file(absl::StrFormat(logfile_template, "FATAL").c_str(),
                     loguru::Truncate, loguru::Verbosity_FATAL);
  }
}

}  // namespace cryptopals::util

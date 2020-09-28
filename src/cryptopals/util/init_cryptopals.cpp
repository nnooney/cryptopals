#include "cryptopals/util/init_cryptopals.h"

#include <filesystem>
#include <fstream>
#include <string_view>

#include "absl/base/module_initializer.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/logging/log_flags.h"
#include "absl/strings/str_join.h"
#include "absl/time/time.h"
#include "cryptopals/util/logging.h"

namespace cryptopals::util {
namespace {

class FileLogSink : public absl::LogSink {
 public:
  explicit FileLogSink(std::string_view path) {
    output.open(path.data(), std::ios_base::app);
  }

  void Send(const absl::LogEntry& entry) override {
    output << absl::StrCat(entry.ToString(), "\n");
    output.flush();
  }

  bool ok() const { return output.good(); }

 private:
  std::ofstream output;
};

static FileLogSink* sink = nullptr;

void InitLogging(int argc, char** argv) {
  if (!absl::GetFlag(FLAGS_logtostderr)) {
    const char* slash = strrchr(argv[0], '/');
    const char* short_program_name = slash ? slash + 1 : argv[0];

    absl::Time now = absl::Now();
    absl::TimeZone utc = absl::UTCTimeZone();
    std::string logfile_name =
        absl::StrJoin({short_program_name, "log",
                       absl::FormatTime("%Y%m%d-%H%M%S", now, utc).c_str()},
                      ".");
    std::string logfile_path =
        absl::StrJoin({"/tmp", logfile_name.c_str()}, "/");

    sink = new FileLogSink(logfile_path);
    CHECK(sink->ok()) << "Failed to create log sink";
    absl::AddLogSink(sink);
  }

  // Log useful program information.
  LOG(INFO) << "Command Line: "
            << absl::StrJoin(std::vector<char*>{argv, argv + argc}, " ");
  LOG(INFO) << "Current Directory: " << std::filesystem::current_path();
}

}  // namespace

void InitCryptopals(std::string_view usage, int argc, char** argv) {
  absl::SetProgramUsageMessage(usage);
  absl::ParseCommandLine(argc, argv);
  InitLogging(argc, argv);
}

}  // namespace cryptopals::util

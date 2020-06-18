#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/status/status.h"
#include "cryptopals/proto/cryptopals_enums.pb.h"
#include "cryptopals/util/bytes.h"
#include "cryptopals/util/logging.h"
#include "cryptopals/util/string_utils.h"

ABSL_FLAG(std::string, from, "", "format of input(s)");
ABSL_FLAG(std::string, to, "", "format of output(s)");

int main(int argc, char **argv) {
  cryptopals::util::InitLogging(argc, argv);
  absl::SetProgramUsageMessage(absl::StrCat(
      "Converts input between various encoding formats.\nExample usage: ",
      argv[0], " --from hex --to base64 049A5CDF 1982EC 30FD7745"));
  std::vector<char *> positional_args = absl::ParseCommandLine(argc, argv);

  std::string from_format_flag = absl::GetFlag(FLAGS_from);
  cryptopals::util::StrToUpper(&from_format_flag);
  std::string to_format_flag = absl::GetFlag(FLAGS_to);
  cryptopals::util::StrToUpper(&to_format_flag);

  cryptopals::BytesEncodedFormat from_format;
  if (from_format_flag.empty()) {
    LOG(ERROR) << "Missing required option: --from";
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }
  bool rc = BytesEncodedFormat_Parse(from_format_flag, &from_format);
  if (!rc) {
    LOG(ERROR) << "Unable to parse --from flag: " << from_format_flag;
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }

  cryptopals::BytesEncodedFormat to_format;
  if (to_format_flag.empty()) {
    LOG(ERROR) << "Missing required option: --to";
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }
  rc = BytesEncodedFormat_Parse(to_format_flag, &to_format);
  if (!rc) {
    LOG(ERROR) << "Unable to parse --to flag: " << to_format_flag;
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }

  LOG(INFO) << "Converting from " << BytesEncodedFormat_Name(from_format)
            << " to " << BytesEncodedFormat_Name(to_format);

  for (int i = 1; i < positional_args.size(); ++i) {
    cryptopals::util::Bytes bytes = cryptopals::util::Bytes::CreateFromFormat(
        positional_args.at(i), from_format);

    std::cout << bytes.ToFormat(to_format) << std::endl;
  }

  return 0;
}

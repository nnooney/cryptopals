#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/status/status.h"
#include "cryptopals/proto/cryptopals_enums.pb.h"
#include "cryptopals/util/bytes.h"
#include "cryptopals/util/string_utils.h"
#include "loguru.hpp"

ABSL_FLAG(std::string, from, "", "Format of input");
ABSL_FLAG(std::string, to, "", "Format of output");

int main(int argc, char **argv) {
  loguru::init(argc, argv);
  absl::SetProgramUsageMessage(absl::StrCat(
      "Converts input between various encoding formats.\nExample usage: ",
      argv[0], " --from hex --to base64 049A5CDF 1982EC 30FD7745"));
  std::vector<char *> positional_args = absl::ParseCommandLine(argc, argv);

  std::string from_format_flag = absl::GetFlag(FLAGS_from);
  cryptopals::util::strToUpper(&from_format_flag);
  std::string to_format_flag = absl::GetFlag(FLAGS_to);
  cryptopals::util::strToUpper(&to_format_flag);

  cryptopals::ConvertFormat from_format;
  if (from_format_flag.empty()) {
    LOG(ERROR) << "Missing required option: --from";
    return 1;
  }
  bool rc = ConvertFormat_Parse(from_format_flag, &from_format);
  if (!rc) {
    LOG(ERROR) << "Unable to parse --from flag: " << from_format_flag;
    return 1;
  }

  cryptopals::ConvertFormat to_format;
  if (to_format_flag.empty()) {
    LOG(ERROR) << "Missing required option: --to";
    return 1;
  }
  rc = ConvertFormat_Parse(to_format_flag, &to_format);
  if (!rc) {
    LOG(ERROR) << "Unable to parse --to flag: " << to_format_flag;
    return 1;
  }

  LOG(INFO) << "Converting from " << ConvertFormat_Name(from_format) << " to "
            << ConvertFormat_Name(to_format);

  for (int i = 1; i < positional_args.size(); ++i) {
    std::vector<uint8_t> bytes;
    switch (from_format) {
      case cryptopals::ConvertFormat::BASE64:
        bytes = cryptopals::util::bytes::from_base64(positional_args.at(i));
        break;
      case cryptopals::ConvertFormat::HEX:
        bytes = cryptopals::util::bytes::from_hex(positional_args.at(i));
        break;
      default:
        LOG(ERROR) << "Unsupported --from format: "
                   << ConvertFormat_Name(from_format);
        return 1;
    }

    switch (to_format) {
      case cryptopals::ConvertFormat::BASE64:
        std::cout << cryptopals::util::bytes::to_base64(bytes) << std::endl;
        break;
      case cryptopals::ConvertFormat::HEX:
        std::cout << cryptopals::util::bytes::to_hex(bytes) << std::endl;
        break;
      default:
        LOG(ERROR) << "Unsupported --to format: "
                   << ConvertFormat_Name(to_format);
        return 1;
    }
  }

  return 0;
}

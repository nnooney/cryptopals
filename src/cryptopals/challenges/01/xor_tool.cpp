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

ABSL_FLAG(std::string, format, "", "format of the operands and output");

int main(int argc, char** argv) {
  cryptopals::util::InitLogging(argc, argv);
  absl::SetProgramUsageMessage(absl::StrCat(
      "Implements the XOR operation on sequences of bytes.\nExample usage: ",
      argv[0], "--format hex ABCDEF 1234"));
  std::vector<char*> positional_args = absl::ParseCommandLine(argc, argv);

  std::string format_flag = absl::GetFlag(FLAGS_format);
  cryptopals::util::StrToUpper(&format_flag);

  cryptopals::BytesEncodedFormat format;
  if (format_flag.empty()) {
    LOG(ERROR) << "Missing required option: --format";
    return 1;
  }
  bool rc = BytesEncodedFormat_Parse(format_flag, &format);
  if (!rc) {
    LOG(ERROR) << "Unable to parse --format flag: " << format_flag;
    return 1;
  }

  if (positional_args.size() < 2) {
    LOG(ERROR) << "Expected at least one positional argument";
    return 1;
  }
  cryptopals::util::Bytes result =
      cryptopals::util::Bytes::CreateFromFormat(positional_args.at(1), format);

  for (int i = 2; i < positional_args.size(); ++i) {
    cryptopals::util::Bytes bytes = cryptopals::util::Bytes::CreateFromFormat(
        positional_args.at(i), format);
    result ^= bytes;
  }

  std::cout << result.ToFormat(format) << std::endl;
  return 0;
}

#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/status/status.h"
#include "cryptopals/proto/cryptopals_enums.pb.h"
#include "cryptopals/util/bytes.h"
#include "cryptopals/util/init_cryptopals.h"
#include "cryptopals/util/internal/status_macros.h"
#include "cryptopals/util/logging.h"
#include "cryptopals/util/tool_helpers.h"

ABSL_FLAG(std::string, format, "", "format of the operands and output");

int main(int argc, char** argv) {
  cryptopals::util::InitCryptopals(
      absl::StrCat("Implements the XOR operation on sequences of "
                   "bytes.\nExample usage: ",
                   argv[0], "--format hex ABCDEF 1234"),
      argc, argv);
  std::vector<char*> positional_args = absl::ParseCommandLine(argc, argv);

  ASSIGN_OR_RETURN(
      cryptopals::BytesEncodedFormat format,
      cryptopals::util::ParseEnumFromString<cryptopals::BytesEncodedFormat>(
          absl::GetFlag(FLAGS_format), cryptopals::BytesEncodedFormat_Parse),
      static_cast<int>(_.code()));

  if (positional_args.size() < 2) {
    LOG(ERROR) << "Expected at least one positional argument";
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
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

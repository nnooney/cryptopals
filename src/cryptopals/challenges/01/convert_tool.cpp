#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/status/status.h"
#include "cryptopals/proto/cryptopals_enums.pb.h"
#include "cryptopals/util/bytes.h"
#include "cryptopals/util/init_cryptopals.h"
#include "cryptopals/util/internal/status/status_macros.h"
#include "cryptopals/util/logging.h"
#include "cryptopals/util/status_adaptors.h"
#include "cryptopals/util/tool_helpers.h"

ABSL_FLAG(std::string, from, "", "format of input(s)");
ABSL_FLAG(std::string, to, "", "format of output(s)");

int main(int argc, char** argv) {
  cryptopals::util::InitCryptopals(
      absl::StrCat(
          "Converts input between various encoding formats.\nExample usage: ",
          argv[0], " --from hex --to base64 049A5CDF 1982EC 30FD7745"),
      argc, argv);
  std::vector<char*> positional_args = absl::ParseCommandLine(argc, argv);

  ASSIGN_OR_RETURN(
      cryptopals::BytesEncodedFormat from_format,
      cryptopals::util::ParseEnumFromString<cryptopals::BytesEncodedFormat>(
          absl::GetFlag(FLAGS_from), cryptopals::BytesEncodedFormat_Parse),
      _.LogError().With(cryptopals::util::Return(EXIT_FAILURE)));

  ASSIGN_OR_RETURN(
      cryptopals::BytesEncodedFormat to_format,
      cryptopals::util::ParseEnumFromString<cryptopals::BytesEncodedFormat>(
          absl::GetFlag(FLAGS_to), cryptopals::BytesEncodedFormat_Parse),
      _.LogError().With(cryptopals::util::Return(EXIT_FAILURE)));

  LOG(INFO) << "Converting from " << BytesEncodedFormat_Name(from_format)
            << " to " << BytesEncodedFormat_Name(to_format);

  for (int i = 1; i < positional_args.size(); ++i) {
    cryptopals::util::Bytes bytes = cryptopals::util::Bytes::CreateFromFormat(
        positional_args.at(i), from_format);

    std::cout << bytes.ToFormat(to_format) << std::endl;
  }

  return 0;
}

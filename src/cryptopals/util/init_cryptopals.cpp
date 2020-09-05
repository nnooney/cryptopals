#include "cryptopals/util/init_cryptopals.h"

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "cryptopals/util/logging.h"

namespace cryptopals::util {

void InitCryptopals(std::string_view usage, int argc, char** argv) {
  absl::SetProgramUsageMessage(usage);
  InitLogging(argc, argv);
}

}  // namespace cryptopals::util

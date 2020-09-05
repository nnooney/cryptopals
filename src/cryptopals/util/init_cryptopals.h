#ifndef CRYPTOPALS_UTIL_INIT_CRYPTOPALS_H_
#define CRYPTOPALS_UTIL_INIT_CRYPTOPALS_H_

#include <string_view>

namespace cryptopals::util {

// Initializes common binary utilities for Cryptopals.
void InitCryptopals(std::string_view usage, int argc, char** argv);

}  // namespace cryptopals::util

#endif  // CRYPTOPALS_UTIL_INIT_CRYPTOPALS_H_

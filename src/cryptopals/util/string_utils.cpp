#include "cryptopals/util/string_utils.h"

#include <algorithm>
#include <cctype>

namespace cryptopals::util {

void StrToUpper(std::string* str) {
  std::transform(str->begin(), str->end(), str->begin(),
                 [](unsigned char c) { return std::toupper(c); });
}

}  // namespace cryptopals::util

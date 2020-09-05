#include "cryptopals/analysis/aes_block_analyzer.h"

#include <vector>

#include "cryptopals/util/aes.h"
#include "cryptopals/util/bytes_util.h"

namespace cryptopals::analysis {

using cryptopals::util::AesState;
using cryptopals::util::Bytes;
using cryptopals::util::SplitBytes;

double AesBlockAnalyzer::AnalyzeBytes(const Bytes& input) {
  double matching_blocks_count = 0;
  std::vector<Bytes> ciphertext_blocks =
      SplitBytes(input, AesState::SIZE_BYTES);

  for (auto b1 = ciphertext_blocks.begin(); b1 != ciphertext_blocks.end();
       ++b1) {
    for (auto b2 = b1 + 1; b2 != ciphertext_blocks.end(); ++b2) {
      if (*b1 == *b2) {
        matching_blocks_count++;
      }
    }
  }

  // Normalize by dividing by the total number of pairs of blocks.
  return matching_blocks_count /
         (ciphertext_blocks.size() * (ciphertext_blocks.size() - 1) / 2);
}

}  // namespace cryptopals::analysis

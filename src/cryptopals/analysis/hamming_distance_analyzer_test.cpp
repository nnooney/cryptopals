#include "cryptopals/analysis/hamming_distance_analyzer.h"

#include "cryptopals/util/bytes.h"
#include "gtest/gtest.h"

namespace cryptopals::analysis {

using cryptopals::util::Bytes;

TEST(HammingDistanceAnalyzerTest, CompareBytesTest) {
  Bytes test_lhs_1 = Bytes::CreateFromRaw("this is a test");
  Bytes test_rhs_1 = Bytes::CreateFromRaw("wokka wokka!!!");

  HammingDistanceAnalyzer hamming_distance_analyzer;
  EXPECT_EQ(hamming_distance_analyzer.CompareBytes(test_lhs_1, test_rhs_1), 37);
}

}  // namespace cryptopals::analysis

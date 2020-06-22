#include "cryptopals/encoding/ascii.h"

#include "gtest/gtest.h"

namespace cryptopals::encoding {

using cryptopals::util::Bytes;

TEST(AsciiEncodingTest, GenerateHistogramTest) {
  AsciiEncoding ascii_encoding;

  Bytes test_in_1 = Bytes::CreateFromRaw("This is a test!");
  absl::flat_hash_map<uint8_t, size_t> expected_histogram = {
      {'T', 1}, {'h', 1}, {'i', 2}, {'s', 3}, {' ', 3},
      {'a', 1}, {'t', 2}, {'e', 1}, {'!', 1}};

  absl::flat_hash_map<uint8_t, size_t> histogram =
      ascii_encoding.GenerateHistogram(test_in_1);

  EXPECT_EQ(histogram, expected_histogram);
}

}  // namespace cryptopals::encoding

#include "cryptopals/lib/bytes.h"

#include "gtest/gtest.h"

namespace cryptopals::lib::bytes {

TEST(BytesTest, FromHexSuccess) {
  std::vector<uint8_t> expected_bytes = {0x01, 0x23, 0x45, 0x67,
                                         0x89, 0xab, 0xcd, 0xef};
  EXPECT_EQ(from_hex("0123456789abcdef"), expected_bytes);
}

TEST(BytesTest, FromHexOddLengthSuccess) {
  std::vector<uint8_t> expected_bytes = {0x01, 0x20};
  EXPECT_EQ(from_hex("012"), expected_bytes);
}

TEST(BytesTest, ToHexSuccess) {
  std::vector<uint8_t> input_bytes = {0x01, 0x23, 0x45, 0x67,
                                      0x89, 0xab, 0xcd, 0xef};
  EXPECT_EQ(to_hex(input_bytes), "0123456789abcdef");
}

TEST(BytesTest, FromHexAndBackSuccess) {
  std::string encoded = "0123456789abcdef";
  EXPECT_EQ(to_hex(from_hex(encoded)), encoded);
}

TEST(BytesTest, FromBase64Success) {
  std::vector<uint8_t> expected_bytes = {
      0x00, 0x10, 0x83, 0x10, 0x51, 0x87, 0x20, 0x92, 0x8b, 0x30, 0xd3, 0x8f,
      0x41, 0x14, 0x93, 0x51, 0x55, 0x97, 0x61, 0x96, 0x9b, 0x71, 0xd7, 0x9f,
      0x82, 0x18, 0xa3, 0x92, 0x59, 0xa7, 0xa2, 0x9a, 0xab, 0xb2, 0xdb, 0xaf,
      0xc3, 0x1c, 0xb3, 0xd3, 0x5d, 0xb7, 0xe3, 0x9e, 0xbb, 0xf3, 0xdf, 0xbf};
  EXPECT_EQ(
      from_base64(
          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"),
      expected_bytes);
}

TEST(BytesTest, FromBase64OddLengthSuccess) {
  std::vector<uint8_t> expected_bytes = {0x00, 0x10, 0x80};
  EXPECT_EQ(from_base64("ABC"), expected_bytes);
}

TEST(BytesTest, FromBase64PaddedSuccess) {
  std::vector<uint8_t> expected_bytes = {0x00, 0x10};
  EXPECT_EQ(from_base64("ABA="), expected_bytes);

  std::vector<uint8_t> expected_bytes2 = {0x00};
  EXPECT_EQ(from_base64("AA=="), expected_bytes2);
}

TEST(BytesTest, ToBase64Success) {
  std::vector<uint8_t> input_bytes = {
      0x00, 0x10, 0x83, 0x10, 0x51, 0x87, 0x20, 0x92, 0x8b, 0x30, 0xd3, 0x8f,
      0x41, 0x14, 0x93, 0x51, 0x55, 0x97, 0x61, 0x96, 0x9b, 0x71, 0xd7, 0x9f,
      0x82, 0x18, 0xa3, 0x92, 0x59, 0xa7, 0xa2, 0x9a, 0xab, 0xb2, 0xdb, 0xaf,
      0xc3, 0x1c, 0xb3, 0xd3, 0x5d, 0xb7, 0xe3, 0x9e, 0xbb, 0xf3, 0xdf, 0xbf};
  EXPECT_EQ(to_base64(input_bytes),
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
}

TEST(BytesTest, ToBase64OddLengthSuccess) {
  std::vector<uint8_t> input_bytes = {0x00};
  EXPECT_EQ(to_base64(input_bytes), "AA==");

  std::vector<uint8_t> input_bytes2 = {0x00, 0x10};
  EXPECT_EQ(to_base64(input_bytes2), "ABA=");
}

TEST(BytesTest, FromBase64AndBackSuccess) {
  std::string encoded =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  EXPECT_EQ(to_base64(from_base64(encoded)), encoded);
}

}  // namespace cryptopals::lib::bytes

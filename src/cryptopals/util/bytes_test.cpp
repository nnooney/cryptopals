#include "cryptopals/util/bytes.h"

#include "gtest/gtest.h"

namespace cryptopals::util {

TEST(BytesTest, HexConversionTest) {
  std::string test_inout_1 = "0123456789abcdef";
  EXPECT_EQ(Bytes::CreateFromHex(test_inout_1).ToHex(), test_inout_1);

  std::string test_in_2 = "012";
  std::string test_out_2 = "0120";
  EXPECT_EQ(Bytes::CreateFromHex(test_in_2).ToHex(), test_out_2);
}

TEST(BytesTest, Base64ConversionTest) {
  std::string test_inout_1 =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  EXPECT_EQ(Bytes::CreateFromBase64(test_inout_1).ToBase64(), test_inout_1);

  std::string test_inout_2 = "AA==";
  EXPECT_EQ(Bytes::CreateFromBase64(test_inout_2).ToBase64(), test_inout_2);

  std::string test_inout_3 = "ABA=";
  EXPECT_EQ(Bytes::CreateFromBase64(test_inout_3).ToBase64(), test_inout_3);

  std::string test_in_4 = "ABC";
  std::string test_out_4 = "ABCA";
  EXPECT_EQ(Bytes::CreateFromBase64(test_in_4).ToBase64(), test_out_4);
}

TEST(BytesTest, XorTest) {
  Bytes lhs = Bytes::CreateFromHex("1c0111001f010100061a024b53535009181c");
  Bytes rhs = Bytes::CreateFromHex("686974207468652062756c6c277320657965");

  Bytes expected_result =
      Bytes::CreateFromHex("746865206b696420646f6e277420706c6179");
  EXPECT_EQ(lhs ^ rhs, expected_result);
}

}  // namespace cryptopals::util

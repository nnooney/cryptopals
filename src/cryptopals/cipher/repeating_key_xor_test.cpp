#include "cryptopals/cipher/repeating_key_xor.h"

#include "gtest/gtest.h"

namespace cryptopals::cipher {

using cryptopals::util::Bytes;

TEST(RepeatingKeyXorTest, EncryptDecryptTest) {
  std::string test_inout_1 =
      "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a "
      "cymbal";
  Bytes test_inout_1_bytes = Bytes::CreateFromRaw(test_inout_1);
  test_inout_1_bytes.SetFormat(cryptopals::BytesEncodedFormat::HEX);

  Bytes test_out_1 = Bytes::CreateFromHex(
      "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765"
      "272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27"
      "282f");
  Bytes test_key_1 = Bytes::CreateFromRaw("ICE");

  RepeatingKeyXor repeating_key_xor;
  EXPECT_EQ(repeating_key_xor.Encrypt(test_inout_1_bytes, test_key_1),
            test_out_1);

  EXPECT_EQ(
      repeating_key_xor
          .Decrypt(repeating_key_xor.Encrypt(test_inout_1_bytes, test_key_1),
                   test_key_1)
          .ToRaw(),
      test_inout_1);
}

}  // namespace cryptopals::cipher

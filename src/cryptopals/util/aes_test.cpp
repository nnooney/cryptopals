#include "cryptopals/util/aes.h"

#include <algorithm>

#include "cryptopals/util/bytes.h"
#include "cryptopals/util/internal/status/matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace cryptopals::util {

TEST(AesTest, MultiplyHelpers) {
  uint8_t input_1 = 0x57;
  EXPECT_EQ(xtime(input_1), 0xae);
  EXPECT_EQ(apply_xtime(input_1, 0), input_1);
  EXPECT_EQ(apply_xtime(input_1, 1), 0xae);
  EXPECT_EQ(apply_xtime(input_1, 2), 0x47);
  EXPECT_EQ(apply_xtime(input_1, 3), 0x8e);
  EXPECT_EQ(apply_xtime(input_1, 4), 0x07);
  EXPECT_EQ(mul(input_1, 0x13), 0xfe);
}

TEST(AesTest, KeyScheduleHelpers) {
  Bytes input_1 = Bytes::CreateFromHex("09cf4f3c");
  Bytes rot_1 = Bytes::CreateFromHex("cf4f3c09");
  Bytes sub_1 = Bytes::CreateFromHex("8a84eb01");
  Bytes rcon_1 = Bytes::CreateFromHex("01000000");
  Bytes out_1 = Bytes::CreateFromHex("8b84eb01");

  EXPECT_EQ(RotWord(input_1), rot_1);
  EXPECT_EQ(SubWord(RotWord(input_1)), sub_1);
  EXPECT_EQ(SubWord(RotWord(input_1)) ^ rcon_1, out_1);
}

TEST(AesTest, GenerateKeySchedule) {
  Bytes input_key = Bytes::CreateFromHex("2b7e151628aed2a6abf7158809cf4f3c");
  Bytes expected_schedule = Bytes::CreateFromHex(
      "2b7e151628aed2a6abf7158809cf4f3ca0fafe1788542cb123a339392a6c7605f2c295f2"
      "7a96b9435935807a7359f67f3d80477d4716fe3e1e237e446d7a883bef44a541a8525b7f"
      "b671253bdb0bad00d4d1c6f87c839d87caf2b8bc11f915bc6d88a37a110b3efddbf98641"
      "ca0093fd4e54f70e5f5fc9f384a64fb24ea6dc4fead27321b58dbad2312bf5607f8d292f"
      "ac7766f319fadc2128d12941575c006ed014f9a8c9ee2589e13f0cc8b6630ca6");
  ASSERT_OK_AND_ASSIGN(Bytes generated_schedule,
                       GenerateKeySchedule(input_key));
  EXPECT_EQ(generated_schedule, expected_schedule);
}

TEST(AesTest, EncryptBlockHelpers) {
  Bytes input_bytes_1 =
      Bytes::CreateFromHex("193de3bea0f4e22b9ac68d2ae9f84808");
  Bytes sub_bytes_1 = Bytes::CreateFromHex("d42711aee0bf98f1b8b45de51e415230");
  Bytes shift_bytes_1 =
      Bytes::CreateFromHex("d4bf5d30e0b452aeb84111f11e2798e5");
  Bytes mix_bytes_1 = Bytes::CreateFromHex("046681e5e0cb199a48f8d37a2806264c");
  Bytes round_key_1 = Bytes::CreateFromHex("a0fafe1788542cb123a339392a6c7605");
  Bytes output_bytes_1 =
      Bytes::CreateFromHex("a49c7ff2689f352b6b5bea43026a5049");

  AesState state;
  std::copy_n(input_bytes_1.begin(), AesState::SIZE_BYTES, state.bytes.begin());

  SubstituteBytes(state);
  EXPECT_EQ(Bytes::CreateFromRange(state.bytes.begin(), state.bytes.end()),
            sub_bytes_1);

  ShiftRows(state);
  EXPECT_EQ(Bytes::CreateFromRange(state.bytes.begin(), state.bytes.end()),
            shift_bytes_1);

  MixColumns(state);
  EXPECT_EQ(Bytes::CreateFromRange(state.bytes.begin(), state.bytes.end()),
            mix_bytes_1);

  AddRoundKey(state, aes_block_span{round_key_1.begin(), round_key_1.size()});
  EXPECT_EQ(Bytes::CreateFromRange(state.bytes.begin(), state.bytes.end()),
            output_bytes_1);
}

TEST(AesTest, DecryptBlockHelpers) {
  Bytes input_bytes_1 =
      Bytes::CreateFromHex("a49c7ff2689f352b6b5bea43026a5049");
  Bytes round_key_1 = Bytes::CreateFromHex("a0fafe1788542cb123a339392a6c7605");
  Bytes key_bytes_1 = Bytes::CreateFromHex("046681e5e0cb199a48f8d37a2806264c");
  Bytes mix_bytes_1 = Bytes::CreateFromHex("d4bf5d30e0b452aeb84111f11e2798e5");
  Bytes shift_bytes_1 =
      Bytes::CreateFromHex("d42711aee0bf98f1b8b45de51e415230");
  Bytes output_bytes_1 =
      Bytes::CreateFromHex("193de3bea0f4e22b9ac68d2ae9f84808");

  AesState state;
  std::copy_n(input_bytes_1.begin(), AesState::SIZE_BYTES, state.bytes.begin());

  AddRoundKey(state, aes_block_span{round_key_1.begin(), round_key_1.size()});
  EXPECT_EQ(Bytes::CreateFromRange(state.bytes.begin(), state.bytes.end()),
            key_bytes_1);

  InvMixColumns(state);
  EXPECT_EQ(Bytes::CreateFromRange(state.bytes.begin(), state.bytes.end()),
            mix_bytes_1);

  InvShiftRows(state);
  EXPECT_EQ(Bytes::CreateFromRange(state.bytes.begin(), state.bytes.end()),
            shift_bytes_1);

  InvSubstituteBytes(state);
  EXPECT_EQ(Bytes::CreateFromRange(state.bytes.begin(), state.bytes.end()),
            output_bytes_1);
}

class Aes128Test : public testing::TestWithParam<
                       std::pair<std::string_view, std::string_view>> {
 public:
};

TEST_P(Aes128Test, EncryptBlock) {
  Bytes key = Bytes::CreateFromHex("00000000000000000000000000000000");
  auto [plaintext, ciphertext] = GetParam();

  Bytes plain_bytes = Bytes::CreateFromHex(plaintext);
  ASSERT_OK_AND_ASSIGN(
      Bytes encryption_result,
      EncryptBlock(aes_block_span{plain_bytes.begin(), plain_bytes.size()},
                   key));
  EXPECT_EQ(encryption_result, Bytes::CreateFromHex(ciphertext));
}

TEST_P(Aes128Test, DecryptBlock) {
  Bytes key = Bytes::CreateFromHex("00000000000000000000000000000000");
  auto [plaintext, ciphertext] = GetParam();

  Bytes cipher_bytes = Bytes::CreateFromHex(ciphertext);
  ASSERT_OK_AND_ASSIGN(
      Bytes decryption_result,
      DecryptBlock(aes_block_span{cipher_bytes.begin(), cipher_bytes.size()},
                   key));
  EXPECT_EQ(decryption_result, Bytes::CreateFromHex(plaintext));
}

// This array stores a series of plaintext and ciphertext blocks encrypted with
// the 128-bit key of all 0s.
std::array<std::pair<std::string_view, std::string_view>, 128> aes128_tests = {{
    {"80000000000000000000000000000000", "3ad78e726c1ec02b7ebfe92b23d9ec34"},
    {"c0000000000000000000000000000000", "aae5939c8efdf2f04e60b9fe7117b2c2"},
    {"e0000000000000000000000000000000", "f031d4d74f5dcbf39daaf8ca3af6e527"},
    {"f0000000000000000000000000000000", "96d9fd5cc4f07441727df0f33e401a36"},
    {"f8000000000000000000000000000000", "30ccdb044646d7e1f3ccea3dca08b8c0"},
    {"fc000000000000000000000000000000", "16ae4ce5042a67ee8e177b7c587ecc82"},
    {"fe000000000000000000000000000000", "b6da0bb11a23855d9c5cb1b4c6412e0a"},
    {"ff000000000000000000000000000000", "db4f1aa530967d6732ce4715eb0ee24b"},
    {"ff800000000000000000000000000000", "a81738252621dd180a34f3455b4baa2f"},
    {"ffc00000000000000000000000000000", "77e2b508db7fd89234caf7939ee5621a"},
    {"ffe00000000000000000000000000000", "b8499c251f8442ee13f0933b688fcd19"},
    {"fff00000000000000000000000000000", "965135f8a81f25c9d630b17502f68e53"},
    {"fff80000000000000000000000000000", "8b87145a01ad1c6cede995ea3670454f"},
    {"fffc0000000000000000000000000000", "8eae3b10a0c8ca6d1d3b0fa61e56b0b2"},
    {"fffe0000000000000000000000000000", "64b4d629810fda6bafdf08f3b0d8d2c5"},
    {"ffff0000000000000000000000000000", "d7e5dbd3324595f8fdc7d7c571da6c2a"},
    {"ffff8000000000000000000000000000", "f3f72375264e167fca9de2c1527d9606"},
    {"ffffc000000000000000000000000000", "8ee79dd4f401ff9b7ea945d86666c13b"},
    {"ffffe000000000000000000000000000", "dd35cea2799940b40db3f819cb94c08b"},
    {"fffff000000000000000000000000000", "6941cb6b3e08c2b7afa581ebdd607b87"},
    {"fffff800000000000000000000000000", "2c20f439f6bb097b29b8bd6d99aad799"},
    {"fffffc00000000000000000000000000", "625d01f058e565f77ae86378bd2c49b3"},
    {"fffffe00000000000000000000000000", "c0b5fd98190ef45fbb4301438d095950"},
    {"ffffff00000000000000000000000000", "13001ff5d99806efd25da34f56be854b"},
    {"ffffff80000000000000000000000000", "3b594c60f5c8277a5113677f94208d82"},
    {"ffffffc0000000000000000000000000", "e9c0fc1818e4aa46bd2e39d638f89e05"},
    {"ffffffe0000000000000000000000000", "f8023ee9c3fdc45a019b4e985c7e1a54"},
    {"fffffff0000000000000000000000000", "35f40182ab4662f3023baec1ee796b57"},
    {"fffffff8000000000000000000000000", "3aebbad7303649b4194a6945c6cc3694"},
    {"fffffffc000000000000000000000000", "a2124bea53ec2834279bed7f7eb0f938"},
    {"fffffffe000000000000000000000000", "b9fb4399fa4facc7309e14ec98360b0a"},
    {"ffffffff000000000000000000000000", "c26277437420c5d634f715aea81a9132"},
    {"ffffffff800000000000000000000000", "171a0e1b2dd424f0e089af2c4c10f32f"},
    {"ffffffffc00000000000000000000000", "7cadbe402d1b208fe735edce00aee7ce"},
    {"ffffffffe00000000000000000000000", "43b02ff929a1485af6f5c6d6558baa0f"},
    {"fffffffff00000000000000000000000", "092faacc9bf43508bf8fa8613ca75dea"},
    {"fffffffff80000000000000000000000", "cb2bf8280f3f9742c7ed513fe802629c"},
    {"fffffffffc0000000000000000000000", "215a41ee442fa992a6e323986ded3f68"},
    {"fffffffffe0000000000000000000000", "f21e99cf4f0f77cea836e11a2fe75fb1"},
    {"ffffffffff0000000000000000000000", "95e3a0ca9079e646331df8b4e70d2cd6"},
    {"ffffffffff8000000000000000000000", "4afe7f120ce7613f74fc12a01a828073"},
    {"ffffffffffc000000000000000000000", "827f000e75e2c8b9d479beed913fe678"},
    {"ffffffffffe000000000000000000000", "35830c8e7aaefe2d30310ef381cbf691"},
    {"fffffffffff000000000000000000000", "191aa0f2c8570144f38657ea4085ebe5"},
    {"fffffffffff800000000000000000000", "85062c2c909f15d9269b6c18ce99c4f0"},
    {"fffffffffffc00000000000000000000", "678034dc9e41b5a560ed239eeab1bc78"},
    {"fffffffffffe00000000000000000000", "c2f93a4ce5ab6d5d56f1b93cf19911c1"},
    {"ffffffffffff00000000000000000000", "1c3112bcb0c1dcc749d799743691bf82"},
    {"ffffffffffff80000000000000000000", "00c55bd75c7f9c881989d3ec1911c0d4"},
    {"ffffffffffffc0000000000000000000", "ea2e6b5ef182b7dff3629abd6a12045f"},
    {"ffffffffffffe0000000000000000000", "22322327e01780b17397f24087f8cc6f"},
    {"fffffffffffff0000000000000000000", "c9cacb5cd11692c373b2411768149ee7"},
    {"fffffffffffff8000000000000000000", "a18e3dbbca577860dab6b80da3139256"},
    {"fffffffffffffc000000000000000000", "79b61c37bf328ecca8d743265a3d425c"},
    {"fffffffffffffe000000000000000000", "d2d99c6bcc1f06fda8e27e8ae3f1ccc7"},
    {"ffffffffffffff000000000000000000", "1bfd4b91c701fd6b61b7f997829d663b"},
    {"ffffffffffffff800000000000000000", "11005d52f25f16bdc9545a876a63490a"},
    {"ffffffffffffffc00000000000000000", "3a4d354f02bb5a5e47d39666867f246a"},
    {"ffffffffffffffe00000000000000000", "d451b8d6e1e1a0ebb155fbbf6e7b7dc3"},
    {"fffffffffffffff00000000000000000", "6898d4f42fa7ba6a10ac05e87b9f2080"},
    {"fffffffffffffff80000000000000000", "b611295e739ca7d9b50f8e4c0e754a3f"},
    {"fffffffffffffffc0000000000000000", "7d33fc7d8abe3ca1936759f8f5deaf20"},
    {"fffffffffffffffe0000000000000000", "3b5e0f566dc96c298f0c12637539b25c"},
    {"ffffffffffffffff0000000000000000", "f807c3e7985fe0f5a50e2cdb25c5109e"},
    {"ffffffffffffffff8000000000000000", "41f992a856fb278b389a62f5d274d7e9"},
    {"ffffffffffffffffc000000000000000", "10d3ed7a6fe15ab4d91acbc7d0767ab1"},
    {"ffffffffffffffffe000000000000000", "21feecd45b2e675973ac33bf0c5424fc"},
    {"fffffffffffffffff000000000000000", "1480cb3955ba62d09eea668f7c708817"},
    {"fffffffffffffffff800000000000000", "66404033d6b72b609354d5496e7eb511"},
    {"fffffffffffffffffc00000000000000", "1c317a220a7d700da2b1e075b00266e1"},
    {"fffffffffffffffffe00000000000000", "ab3b89542233f1271bf8fd0c0f403545"},
    {"ffffffffffffffffff00000000000000", "d93eae966fac46dca927d6b114fa3f9e"},
    {"ffffffffffffffffff80000000000000", "1bdec521316503d9d5ee65df3ea94ddf"},
    {"ffffffffffffffffffc0000000000000", "eef456431dea8b4acf83bdae3717f75f"},
    {"ffffffffffffffffffe0000000000000", "06f2519a2fafaa596bfef5cfa15c21b9"},
    {"fffffffffffffffffff0000000000000", "251a7eac7e2fe809e4aa8d0d7012531a"},
    {"fffffffffffffffffff8000000000000", "3bffc16e4c49b268a20f8d96a60b4058"},
    {"fffffffffffffffffffc000000000000", "e886f9281999c5bb3b3e8862e2f7c988"},
    {"fffffffffffffffffffe000000000000", "563bf90d61beef39f48dd625fcef1361"},
    {"ffffffffffffffffffff000000000000", "4d37c850644563c69fd0acd9a049325b"},
    {"ffffffffffffffffffff800000000000", "b87c921b91829ef3b13ca541ee1130a6"},
    {"ffffffffffffffffffffc00000000000", "2e65eb6b6ea383e109accce8326b0393"},
    {"ffffffffffffffffffffe00000000000", "9ca547f7439edc3e255c0f4d49aa8990"},
    {"fffffffffffffffffffff00000000000", "a5e652614c9300f37816b1f9fd0c87f9"},
    {"fffffffffffffffffffff80000000000", "14954f0b4697776f44494fe458d814ed"},
    {"fffffffffffffffffffffc0000000000", "7c8d9ab6c2761723fe42f8bb506cbcf7"},
    {"fffffffffffffffffffffe0000000000", "db7e1932679fdd99742aab04aa0d5a80"},
    {"ffffffffffffffffffffff0000000000", "4c6a1c83e568cd10f27c2d73ded19c28"},
    {"ffffffffffffffffffffff8000000000", "90ecbe6177e674c98de412413f7ac915"},
    {"ffffffffffffffffffffffc000000000", "90684a2ac55fe1ec2b8ebd5622520b73"},
    {"ffffffffffffffffffffffe000000000", "7472f9a7988607ca79707795991035e6"},
    {"fffffffffffffffffffffff000000000", "56aff089878bf3352f8df172a3ae47d8"},
    {"fffffffffffffffffffffff800000000", "65c0526cbe40161b8019a2a3171abd23"},
    {"fffffffffffffffffffffffc00000000", "377be0be33b4e3e310b4aabda173f84f"},
    {"fffffffffffffffffffffffe00000000", "9402e9aa6f69de6504da8d20c4fcaa2f"},
    {"ffffffffffffffffffffffff00000000", "123c1f4af313ad8c2ce648b2e71fb6e1"},
    {"ffffffffffffffffffffffff80000000", "1ffc626d30203dcdb0019fb80f726cf4"},
    {"ffffffffffffffffffffffffc0000000", "76da1fbe3a50728c50fd2e621b5ad885"},
    {"ffffffffffffffffffffffffe0000000", "082eb8be35f442fb52668e16a591d1d6"},
    {"fffffffffffffffffffffffff0000000", "e656f9ecf5fe27ec3e4a73d00c282fb3"},
    {"fffffffffffffffffffffffff8000000", "2ca8209d63274cd9a29bb74bcd77683a"},
    {"fffffffffffffffffffffffffc000000", "79bf5dce14bb7dd73a8e3611de7ce026"},
    {"fffffffffffffffffffffffffe000000", "3c849939a5d29399f344c4a0eca8a576"},
    {"ffffffffffffffffffffffffff000000", "ed3c0a94d59bece98835da7aa4f07ca2"},
    {"ffffffffffffffffffffffffff800000", "63919ed4ce10196438b6ad09d99cd795"},
    {"ffffffffffffffffffffffffffc00000", "7678f3a833f19fea95f3c6029e2bc610"},
    {"ffffffffffffffffffffffffffe00000", "3aa426831067d36b92be7c5f81c13c56"},
    {"fffffffffffffffffffffffffff00000", "9272e2d2cdd11050998c845077a30ea0"},
    {"fffffffffffffffffffffffffff80000", "088c4b53f5ec0ff814c19adae7f6246c"},
    {"fffffffffffffffffffffffffffc0000", "4010a5e401fdf0a0354ddbcc0d012b17"},
    {"fffffffffffffffffffffffffffe0000", "a87a385736c0a6189bd6589bd8445a93"},
    {"ffffffffffffffffffffffffffff0000", "545f2b83d9616dccf60fa9830e9cd287"},
    {"ffffffffffffffffffffffffffff8000", "4b706f7f92406352394037a6d4f4688d"},
    {"ffffffffffffffffffffffffffffc000", "b7972b3941c44b90afa7b264bfba7387"},
    {"ffffffffffffffffffffffffffffe000", "6f45732cf10881546f0fd23896d2bb60"},
    {"fffffffffffffffffffffffffffff000", "2e3579ca15af27f64b3c955a5bfc30ba"},
    {"fffffffffffffffffffffffffffff800", "34a2c5a91ae2aec99b7d1b5fa6780447"},
    {"fffffffffffffffffffffffffffffc00", "a4d6616bd04f87335b0e53351227a9ee"},
    {"fffffffffffffffffffffffffffffe00", "7f692b03945867d16179a8cefc83ea3f"},
    {"ffffffffffffffffffffffffffffff00", "3bd141ee84a0e6414a26e7a4f281f8a2"},
    {"ffffffffffffffffffffffffffffff80", "d1788f572d98b2b16ec5d5f3922b99bc"},
    {"ffffffffffffffffffffffffffffffc0", "0833ff6f61d98a57b288e8c3586b85a6"},
    {"ffffffffffffffffffffffffffffffe0", "8568261797de176bf0b43becc6285afb"},
    {"fffffffffffffffffffffffffffffff0", "f9b0fda0c4a898f5b9e6f661c4ce4d07"},
    {"fffffffffffffffffffffffffffffff8", "8ade895913685c67c5269f8aae42983e"},
    {"fffffffffffffffffffffffffffffffc", "39bde67d5c8ed8a8b1c37eb8fa9f5ac0"},
    {"fffffffffffffffffffffffffffffffe", "5c005e72c1418c44f569f2ea33ba54f3"},
    {"ffffffffffffffffffffffffffffffff", "3f5b8cc9ea855a0afa7347d23e8d664e"},
}};

INSTANTIATE_TEST_SUITE_P(Aes128Parameterized, Aes128Test,
                         testing::ValuesIn(aes128_tests));

}  // namespace cryptopals::util

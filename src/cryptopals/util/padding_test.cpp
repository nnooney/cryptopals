#include "cryptopals/util/padding.h"

#include "googletest/status_matchers.h"
#include "gtest/gtest.h"

namespace cryptopals::util {
namespace {

TEST(PaddingTest, AddPkcs7Padding) {
  Bytes input = Bytes::CreateFromRaw("YELLOW SUBMARINE");

  Bytes expected_result =
      Bytes::CreateFromRaw("YELLOW SUBMARINE\x04\x04\x04\x04");
  AddPkcs7Padding(input, 20);
  EXPECT_EQ(input.size(), expected_result.size());
  EXPECT_EQ(input, expected_result);
}

TEST(PaddingTest, RemovePkcs7Padding) {
  Bytes input = Bytes::CreateFromRaw("YELLOW SUBMARINE\x04\x04\x04\x04");

  Bytes expected_result = Bytes::CreateFromRaw("YELLOW SUBMARINE");
  ASSERT_OK(RemovePkcs7Padding(input));
  EXPECT_EQ(input.size(), expected_result.size());
  EXPECT_EQ(input, expected_result);
}

TEST(PaddingTest, AddAndRemovePkcs7Padding) {
  Bytes input = Bytes::CreateFromRaw("YELLOW SUBMARINE");

  Bytes expected_result = Bytes::CreateFromRaw("YELLOW SUBMARINE");
  AddPkcs7Padding(input, 20);
  ASSERT_OK(RemovePkcs7Padding(input));
  EXPECT_EQ(input.size(), expected_result.size());
  EXPECT_EQ(input, expected_result);
}

}  // namespace
}  // namespace cryptopals::util

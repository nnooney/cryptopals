#include <algorithm>
#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/status/status.h"
#include "cryptopals/analysis/data/oanc_english.h"
#include "cryptopals/analysis/frequency_analyzer.h"
#include "cryptopals/cipher/decryption_result.h"
#include "cryptopals/cipher/single_byte_xor.h"
#include "cryptopals/encoding/ascii.h"
#include "cryptopals/proto/cryptopals_enums.pb.h"
#include "cryptopals/util/bytes.h"
#include "cryptopals/util/init_cryptopals.h"
#include "cryptopals/util/internal/status_macros.h"
#include "cryptopals/util/logging.h"
#include "cryptopals/util/tool_helpers.h"

ABSL_FLAG(std::string, action, "",
          "the action to perform (encrypt, decrypt, crack)");
ABSL_FLAG(std::string, format, "", "format of the operands and output");
ABSL_FLAG(std::string, key, "", "the key used to encrypt/decrypt a message");
ABSL_FLAG(std::string, input, "stdin",
          "the input method (stdin, ciphertext_file, multi_ciphertext_file)");

namespace {

using cryptopals::util::Bytes;

absl::Status Encrypt(std::string_view encoded_text,
                     cryptopals::BytesEncodedFormat format) {
  std::string key_flag = absl::GetFlag(FLAGS_key);
  if (key_flag.empty()) {
    return absl::InvalidArgumentError("Action ENCRYPT requires --key flag");
  }
  Bytes key_bytes = Bytes::CreateFromFormat(key_flag, format);
  if (key_bytes.size() != 1) {
    return absl::InvalidArgumentError(absl::StrCat(
        "Expected key length to be 1, but key length is ", key_bytes.size()));
  }
  uint8_t key = key_bytes.at(0);

  Bytes plaintext = Bytes::CreateFromRaw(encoded_text);

  cryptopals::cipher::SingleByteXor single_byte_xor;
  Bytes ciphertext = single_byte_xor.Encrypt(plaintext, key);
  std::cout << ciphertext.ToFormat(format) << std::endl;

  return absl::OkStatus();
}

absl::Status Decrypt(std::string_view encoded_text,
                     cryptopals::BytesEncodedFormat format) {
  std::string key_flag = absl::GetFlag(FLAGS_key);
  if (key_flag.empty()) {
    return absl::InvalidArgumentError("Action DECRYPT requires --key flag");
  }
  Bytes key_bytes = Bytes::CreateFromFormat(key_flag, format);
  if (key_bytes.size() != 1) {
    return absl::InvalidArgumentError(absl::StrCat(
        "Expected key length to be 1, but key length is ", key_bytes.size()));
  }
  uint8_t key = key_bytes.at(0);

  Bytes ciphertext = Bytes::CreateFromFormat(encoded_text, format);

  cryptopals::cipher::SingleByteXor single_byte_xor;
  Bytes plaintext = single_byte_xor.Decrypt(ciphertext, key);
  std::cout << plaintext.ToRaw() << std::endl;

  return absl::OkStatus();
}

absl::Status Crack(std::string_view encoded_text,
                   cryptopals::BytesEncodedFormat format) {
  const Bytes ciphertext = Bytes::CreateFromFormat(encoded_text, format);
  cryptopals::cipher::SingleByteXor single_byte_xor;
  cryptopals::cipher::SingleByteXor::DecryptionResultType decryption_result =
      single_byte_xor.Crack(ciphertext);
  if (decryption_result.score < 200) {
    std::cout << std::hex << std::showbase << decryption_result << std::endl;
  }

  return absl::OkStatus();
}

}  // namespace

int main(int argc, char** argv) {
  cryptopals::util::InitCryptopals(
      "Explores the single-byte xor cipher. Possible operations are defined "
      "with the --action flag.",
      argc, argv);
  std::vector<char*> positional_args = absl::ParseCommandLine(argc, argv);

  ASSIGN_OR_RETURN(
      cryptopals::BytesEncodedFormat format,
      cryptopals::util::ParseEnumFromString<cryptopals::BytesEncodedFormat>(
          absl::GetFlag(FLAGS_format), cryptopals::BytesEncodedFormat_Parse),
      static_cast<int>(_.code()));

  ASSIGN_OR_RETURN(
      cryptopals::CipherAction action,
      cryptopals::util::ParseEnumFromString<cryptopals::CipherAction>(
          absl::GetFlag(FLAGS_action), cryptopals::CipherAction_Parse),
      static_cast<int>(_.code()));

  ASSIGN_OR_RETURN(
      cryptopals::InputMethod input,
      cryptopals::util::ParseEnumFromString<cryptopals::InputMethod>(
          absl::GetFlag(FLAGS_input), cryptopals::InputMethod_Parse),
      static_cast<int>(_.code()));

  if (positional_args.size() < 2) {
    LOG(ERROR) << "Expected at least one positional argument";
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }

  std::vector<std::string> inputs = cryptopals::util::GetInputsForMethod(
      std::vector<char*>(positional_args.begin() + 1, positional_args.end()),
      input);

  switch (action) {
    case cryptopals::CipherAction::ENCRYPT: {
      for (std::string input : inputs) {
        absl::Status status = Encrypt(input, format);
        if (!status.ok()) {
          LOG(ERROR) << status;
          return static_cast<int>(status.code());
        }
      }
      break;
    }
    case cryptopals::CipherAction::DECRYPT: {
      for (std::string input : inputs) {
        absl::Status status = Decrypt(input, format);
        if (!status.ok()) {
          LOG(ERROR) << status;
          return static_cast<int>(status.code());
        }
      }
      break;
    }
    case cryptopals::CipherAction::CRACK: {
      for (std::string input : inputs) {
        absl::Status status = Crack(input, format);
        if (!status.ok()) {
          LOG(ERROR) << status;
          return static_cast<int>(status.code());
        }
      }
      break;
    }
    default:
      LOG(ERROR) << "Unsupported --action flag: " << CipherAction_Name(action);
      return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }

  return 0;
}

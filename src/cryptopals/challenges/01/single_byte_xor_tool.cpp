#include <algorithm>
#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/status/status.h"
#include "cryptopals/analysis/english_ascii_frequency_analyzer.h"
#include "cryptopals/cipher/decryption_result.h"
#include "cryptopals/cipher/single_byte_xor.h"
#include "cryptopals/proto/cryptopals_enums.pb.h"
#include "cryptopals/util/bytes.h"
#include "cryptopals/util/logging.h"
#include "cryptopals/util/string_utils.h"

ABSL_FLAG(std::string, action, "",
          "the action to perform (encrypt, decrypt, crack)");
ABSL_FLAG(std::string, format, "", "format of the operands and output");
ABSL_FLAG(std::string, key, "", "the key used to encrypt/decrypt a message");

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
  std::vector<cryptopals::cipher::SingleByteXor::DecryptionResultType>
      decryption_results;
  cryptopals::cipher::SingleByteXor single_byte_xor;
  cryptopals::analysis::EnglishAsciiFrequencyAnalyzer english_ascii_frequency_analyzer;

  for (uint8_t possible_key = 0; possible_key < UINT8_MAX; ++possible_key) {
    Bytes decrypted_text = single_byte_xor.Decrypt(ciphertext, possible_key);
    double score = english_ascii_frequency_analyzer.AnalyzeBytes(decrypted_text);
    decryption_results.push_back({.score = score,
                                  .decrypted_text = decrypted_text,
                                  .key = possible_key});
  }
  std::sort(decryption_results.begin(), decryption_results.end(),
            std::greater<>());

  std::cout << ciphertext << std::endl;
  for (int i = 0; i < decryption_results.size(); ++i) {
    if (decryption_results[i].score < 15) {
      break;
    }
    std::cout << std::hex << decryption_results[i] << std::endl;
  }

  return absl::OkStatus();
}

}  // namespace

int main(int argc, char** argv) {
  cryptopals::util::InitLogging(argc, argv);
  absl::SetProgramUsageMessage(
      absl::StrCat("Explores the single-byte xor cipher. Possible operations "
                   "are defined with the --action flag."));
  std::vector<char*> positional_args = absl::ParseCommandLine(argc, argv);

  std::string format_flag = absl::GetFlag(FLAGS_format);
  cryptopals::util::StrToUpper(&format_flag);
  std::string action_flag = absl::GetFlag(FLAGS_action);
  cryptopals::util::StrToUpper(&action_flag);

  cryptopals::BytesEncodedFormat format;
  if (format_flag.empty()) {
    LOG(ERROR) << "Missing required option: --format";
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }
  bool rc = BytesEncodedFormat_Parse(format_flag, &format);
  if (!rc) {
    LOG(ERROR) << "Unable to parse --format flag: " << format_flag;
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }

  cryptopals::CipherAction action;
  if (action_flag.empty()) {
    LOG(ERROR) << "Missing required option: --action";
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }
  rc = CipherAction_Parse(action_flag, &action);
  if (!rc) {
    LOG(ERROR) << "Unable to parse --action flag: " << action_flag;
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }

  if (positional_args.size() < 2) {
    LOG(ERROR) << "Expected at least one positional argument";
    return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }

  switch (action) {
    case cryptopals::CipherAction::ENCRYPT: {
      absl::Status status = Encrypt(positional_args.at(1), format);
      if (!status.ok()) {
        LOG(ERROR) << status;
        return static_cast<int>(status.code());
      }
      break;
    }
    case cryptopals::CipherAction::DECRYPT: {
      absl::Status status = Decrypt(positional_args.at(1), format);
      if (!status.ok()) {
        LOG(ERROR) << status;
        return static_cast<int>(status.code());
      }
      break;
    }
    case cryptopals::CipherAction::CRACK: {
      absl::Status status = Crack(positional_args.at(1), format);
      if (!status.ok()) {
        LOG(ERROR) << status;
        return static_cast<int>(status.code());
      }
      break;
    }
    default:
      LOG(ERROR) << "Unsupported --action flag: " << CipherAction_Name(action);
      return static_cast<int>(absl::StatusCode::kInvalidArgument);
  }

  return 0;
}

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/status/status.h"
#include "cryptopals/cipher/aes_ecb.h"
#include "cryptopals/proto/cryptopals_enums.pb.h"
#include "cryptopals/util/bytes.h"
#include "cryptopals/util/logging.h"
#include "cryptopals/util/string_utils.h"

ABSL_FLAG(std::string, action, "",
          "the action to perform (encrypt, decrypt, crack)");
ABSL_FLAG(std::string, format, "", "format of the operands and output");
ABSL_FLAG(bool, file, "",
          "treat the input as a filepath rather than direct input");
ABSL_FLAG(std::string, key, "", "the key used to encrypt/decrypt a message");

namespace {

using cryptopals::util::Bytes;

absl::Status Encrypt(std::string_view encoded_text,
                     cryptopals::BytesEncodedFormat format) {
  std::string key_flag = absl::GetFlag(FLAGS_key);
  if (key_flag.empty()) {
    return absl::InvalidArgumentError("Action ENCRYPT requires --key flag");
  }
  Bytes key = Bytes::CreateFromFormat(key_flag, format);
  Bytes plaintext = Bytes::CreateFromRaw(encoded_text);

  cryptopals::cipher::AesEcb aes_ecb;
  Bytes ciphertext = aes_ecb.Encrypt(plaintext, key);
  std::cout << ciphertext.ToFormat(format) << std::endl;

  return absl::OkStatus();
}

absl::Status Decrypt(std::string_view encoded_text,
                     cryptopals::BytesEncodedFormat format) {
  std::string key_flag = absl::GetFlag(FLAGS_key);
  if (key_flag.empty()) {
    return absl::InvalidArgumentError("Action DECRYPT requires --key flag");
  }
  Bytes key = Bytes::CreateFromFormat(key_flag, format);
  Bytes ciphertext = Bytes::CreateFromFormat(encoded_text, format);

  cryptopals::cipher::AesEcb aes_ecb;
  Bytes plaintext = aes_ecb.Decrypt(ciphertext, key);
  std::cout << plaintext.ToRaw() << std::endl;

  return absl::OkStatus();
}

absl::Status Crack(std::string_view encoded_text,
                   cryptopals::BytesEncodedFormat format) {
  return absl::UnimplementedError("I don't know how to crack this yet");
}

}  // namespace

int main(int argc, char** argv) {
  cryptopals::util::InitLogging(argc, argv);
  absl::SetProgramUsageMessage(
      absl::StrCat("Explores the AES ECB cipher. Possible operations "
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

  std::string input_text;
  if (absl::GetFlag(FLAGS_file)) {
    std::ifstream file_stream(positional_args.at(1));
    std::ostringstream file_contents;
    file_contents << file_stream.rdbuf();
    input_text = file_contents.str();
    input_text.erase(std::remove(input_text.begin(), input_text.end(), '\n'),
                     input_text.end());
  } else {
    input_text = positional_args.at(1);
  }

  switch (action) {
    case cryptopals::CipherAction::ENCRYPT: {
      absl::Status status = Encrypt(input_text, format);
      if (!status.ok()) {
        LOG(ERROR) << status;
        return static_cast<int>(status.code());
      }
      break;
    }
    case cryptopals::CipherAction::DECRYPT: {
      absl::Status status = Decrypt(input_text, format);
      if (!status.ok()) {
        LOG(ERROR) << status;
        return static_cast<int>(status.code());
      }
      break;
    }
    case cryptopals::CipherAction::CRACK: {
      absl::Status status = Crack(input_text, format);
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

#include "cryptopals/util/tool_helpers.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "cryptopals/util/logging.h"

namespace cryptopals::util {

std::vector<std::string> GetInputsForMethod(std::vector<char*> inputs,
                                            cryptopals::InputMethod method) {
  std::vector<std::string> results;
  switch (method) {
    case cryptopals::InputMethod::STDIN: {
      for (char* input : inputs) {
        results.emplace_back(input);
      }
      break;
    }
    case cryptopals::InputMethod::CIPHERTEXT_FILE: {
      for (char* input : inputs) {
        std::ifstream file_stream(input);
        std::ostringstream file_contents;
        file_contents << file_stream.rdbuf();

        std::string temp = file_contents.str();
        temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
        results.push_back(temp);
      }
      break;
    }
    case cryptopals::InputMethod::MULTI_CIPHERTEXT_FILE: {
      std::string line;
      for (char* input : inputs) {
        std::ifstream file_stream(input);
        while (std::getline(file_stream, line)) {
          results.push_back(line);
        }
      }
      break;
    }
    default: {
      LOG(ERROR) << "Unsupported input method: " << InputMethod_Name(method);
      break;
    }
  }
  return results;
}

}  // namespace cryptopals::util

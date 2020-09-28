#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "absl/container/flat_hash_map.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/strings/str_split.h"
#include "cryptopals/encoding/ascii.h"
#include "cryptopals/proto/cryptopals_enums.pb.h"
#include "cryptopals/util/algorithm.h"
#include "cryptopals/util/bytes.h"
#include "cryptopals/util/init_cryptopals.h"

using cryptopals::encoding::AsciiEncoding;
using cryptopals::util::Bytes;

ABSL_FLAG(std::string, filemap, "",
          "A text file containing the absolute paths of files to process, one "
          "per line");

int main(int argc, char** argv) {
  cryptopals::util::InitCryptopals(
      "Reads a series of input files and generates a frequency of each code "
      "point in the file. The input files can be specified as positional "
      "arguments on the command line or in a file map (see --filemap)",
      argc, argv);
  std::vector<char*> positional_args = absl::ParseCommandLine(argc, argv);

  std::string filemap_flag = absl::GetFlag(FLAGS_filemap);

  std::vector<std::string> files_list;
  if (!filemap_flag.empty()) {
    std::ifstream filemap_stream(filemap_flag);
    std::ostringstream filemap_contents;
    filemap_contents << filemap_stream.rdbuf();
    files_list = absl::StrSplit(filemap_contents.str(), "\n");
  } else {
    files_list.assign(positional_args.begin() + 1, positional_args.end());
  }

  AsciiEncoding ascii_encoding;
  AsciiEncoding::HistogramType histogram;

  size_t total_input_length = 0;
  for (const auto& file : files_list) {
    std::ifstream input_stream(file);

    std::ostringstream contents;
    contents << input_stream.rdbuf();

    AsciiEncoding::HistogramType file_histogram =
        ascii_encoding.GenerateHistogram(Bytes::CreateFromRaw(contents.str()));
    histogram = cryptopals::util::MapCombine(histogram, file_histogram,
                                             std::plus<size_t>());

    total_input_length += contents.str().size();
  }

  for (auto [k, v] : histogram) {
    std::cout << std::hex << std::setw(2) << static_cast<int>(k) << std::dec
              << ": " << std::fixed
              << (static_cast<double>(v) / total_input_length) << std::endl;
  }
  return 0;
}

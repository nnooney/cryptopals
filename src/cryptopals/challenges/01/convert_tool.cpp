#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

ABSL_FLAG(std::string, input, "", "Input data to convert.");

int main(int argc, char **argv) {
    absl::ParseCommandLine(argc, argv);

    return 0;
}

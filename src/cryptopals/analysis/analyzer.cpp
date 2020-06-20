#include "cryptopals/analysis/analyzer.h"

#include "cryptopals/util/logging.h"

namespace cryptopals::analysis {

double AnalyzerInterface::AnalyzeBytes(const cryptopals::util::Bytes& input) {
  LOG(ERROR) << "Invoked base implementation, use derived method instead";
  return 0.0;
}

double AnalyzerInterface::CompareBytes(const cryptopals::util::Bytes& lhs,
                                       const cryptopals::util::Bytes& rhs) {
  LOG(ERROR) << "Invoked base implementation, use derived method instead";
  return 0.0;
}

}  // namespace cryptopals::analysis

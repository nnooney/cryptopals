#ifndef CRYPTOPALS_ENCODING_H_
#define CRYPTOPALS_ENCODING_H_

#include <string>

#include "absl/container/flat_hash_map.h"
#include "cryptopals/util/bytes.h"

namespace cryptopals::encoding {

// The EncodingInterface class describes the public interface required for an
// encoding.
template <class CodePointType>
class EncodingInterface {
 public:
  // HistogramType is a reference to an absl::flat_hash_map that stores the
  // number of occurrences of several code points.
  using HistogramType = absl::flat_hash_map<CodePointType, size_t>;

  virtual ~EncodingInterface() {}

  // Generates a histogram of the occurrences of each code point in `input`.
  virtual HistogramType GenerateHistogram(
      const cryptopals::util::Bytes& input) const = 0;
};

}  // namespace cryptopals::encoding

#endif  // CRYPTOPALS_ENCODING_H_

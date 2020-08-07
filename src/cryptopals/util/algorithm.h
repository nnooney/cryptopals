#ifndef CRYPTOPALS_UTIL_ALGORITHM_H_
#define CRYPTOPALS_UTIL_ALGORITHM_H_

#include <cstdint>

namespace cryptopals::util {

// Calculates the smallest multiple of `n` greater than or equal to `value`.
inline constexpr size_t CeilingMultiple(size_t value, size_t n) {
  return (value + n - 1) / n * n;
}

// Calculates the largest multiple of `n` less than or equal to `value`.
inline constexpr size_t FloorMultiple(size_t value, size_t n) {
  return value / n * n;
}

// MapCombine combines two map-like containers by merging the contents of `rhs`
// into `lhs`. Three cases can occur when combining:
//
//  1. Key exists in `lhs` only. The value of `lhs` is maintained as is.
//  2. Key exists in `rhs` only. The result of `f(lhs[], rhs)` is assigned.
//     Here, `lhs` is default constructed.
//  3. Key exists in `lhs` and `rhs`. The result of `f(lhs, rhs)` is assigned.
//
// Be aware of the lack of symmetry; the function `f` is always applied to
// elements of `rhs` but may not be applied to elements of `lhs`.
template <typename C1, typename C2, typename Function>
C1 MapCombine(C1 lhs, const C2& rhs, Function&& f) {
  for (auto rhs_it = rhs.begin(); rhs_it != rhs.end(); ++rhs_it) {
    auto& key = rhs_it->first;
    lhs.insert_or_assign(key, f(lhs[key], rhs_it->second));
  }
  return lhs;
}

}  // namespace cryptopals::util

#endif  // CRYPTOPALS_UTIL_ALGORITHM_H_

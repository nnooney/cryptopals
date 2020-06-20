// Relative frequencies of letters in the English language. Data obtained from:
// https://en.wikipedia.org/wiki/Letter_frequency

#ifndef CRYPTOPALS_FREQUENCY_ENGLISH_H_
#define CRYPTOPALS_FREQUENCY_ENGLISH_H_

namespace cryptopals::analysis::english {

inline constexpr double A = 8.497;
inline constexpr double B = 1.492;
inline constexpr double C = 2.202;
inline constexpr double D = 4.253;
inline constexpr double E = 11.162;
inline constexpr double F = 2.228;
inline constexpr double G = 2.015;
inline constexpr double H = 6.094;
inline constexpr double I = 7.546;
inline constexpr double J = 0.153;
inline constexpr double K = 1.292;
inline constexpr double L = 4.025;
inline constexpr double M = 2.406;
inline constexpr double N = 6.749;
inline constexpr double O = 7.507;
inline constexpr double P = 1.929;
inline constexpr double Q = 0.095;
inline constexpr double R = 7.587;
inline constexpr double S = 6.327;
inline constexpr double T = 9.356;
inline constexpr double U = 2.758;
inline constexpr double V = 0.978;
inline constexpr double W = 2.560;
inline constexpr double X = 0.150;
inline constexpr double Y = 1.994;
inline constexpr double Z = 0.077;

inline constexpr double alphabet[26] = {
  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};

}  // namespace cryptopals::analysis::english

#endif  // CRYPTOPALS_FREQUENCY_ENGLISH_H_

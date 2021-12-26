#include <cmath>

namespace w {

namespace detail {

constexpr double PI = 3.141592653589793238463;

inline double cossum2(double a0, double a1, int n, int N) { return a0 - a1 * std::cos((2 * PI * n) / N); }
inline double cossum3(double a0, double a1, double a2, int n, int N) {
    return a0 - a1 * std::cos((2 * PI * n) / N) + a2 * std::cos((4 * PI * n) / N);
}
inline double cossum4(double a0, double a1, double a2, double a3, int n, int N) {
    return a0 - a1 * std::cos((2 * PI * n) / N) + a2 * std::cos((4 * PI * n) / N) - a3 * std::cos((6 * PI * n) / N);
}

inline double kaiser(double beta, int n, int N) {
    const double x = (2 * n) / double(N) - 1;
    const double arg = beta * std::sqrt(1 - x * x);

    return std::cyl_bessel_i(0, beta * x) / std::cyl_bessel_i(0, beta);
}

}  // namespace detail

inline double rectangular(int n, int N) { return 1; }

inline double barlett(int n, int N) { return 1 - std::abs((n - N / 2) / double(N / 2)); }

inline double hamming(int n, int N) {
    constexpr double a0 = 0.53836;
    return detail::cossum2(a0, 1 - a0, n, N);
}

inline double hann(int n, int N) {
    constexpr double a0 = 0.5;
    return detail::cossum2(a0, a0, n, N);
}

inline double blackman(int n, int N) {
    constexpr double a0 = 0.4265907136715391229579;
    constexpr double a1 = 0.4965606190885640584695;
    constexpr double a2 = 0.07684866723989681857266;
    return detail::cossum3(a0, a1, a2, n, N);
}

inline double blackmanNuttall(int n, int N) {
    constexpr double a0 = 0.3635819;
    constexpr double a1 = 0.4891775;
    constexpr double a2 = 0.1365995;
    constexpr double a3 = 0.0106411;
    return detail::cossum4(a0, a1, a2, a3, n, N);
}

inline double blackmanHarris(int n, int N) {
    constexpr double a0 = 0.35875;
    constexpr double a1 = 0.48829;
    constexpr double a2 = 0.14128;
    constexpr double a3 = 0.01168;
    return detail::cossum4(a0, a1, a2, a3, n, N);
}

inline double kaiser4(int n, int N) { return detail::kaiser(4, n, N); }

inline double kaiser8(int n, int N) { return detail::kaiser(8, n, N); }

inline double kaiser16(int n, int N) { return detail::kaiser(16, n, N); }

}  // namespace w
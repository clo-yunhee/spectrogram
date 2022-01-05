#ifndef WINDOWFUNCS_H
#define WINDOWFUNCS_H

#include <QStringList>

#include "../utils.h"

namespace w {
double rectangular(int n, int N);
double barlett(int n, int N);
double hamming(int n, int N);
double hann(int n, int N);
double blackman(int n, int N);
double blackmanNuttall(int n, int N);
double blackmanHarris(int n, int N);
double kaiser4(int n, int N);
double kaiser8(int n, int N);
double kaiser16(int n, int N);
}  // namespace w

class WindowFunctions final {
   public:
    enum Type {
        Rectangular = 0,
        Barlett,
        Hamming,
        Hann,
        Blackman,
        BlackmanNuttall,
        BlackmanHarris,
        Kaiser4,
        Kaiser8,
        Kaiser16,
        TypeLast,
    };

    using FunctionType = double (*)(int n, int N);

    static inline FunctionType get(Type type) { return g_funcs[type]; }

    static inline double eval(Type type, int n, int N) { return (g_funcs[type])(n, N); }

    static const QStringList& names() { return g_names; }

   private:
    WindowFunctions() = delete;

    static const FunctionType g_funcs[TypeLast];
    static const QStringList g_names;
};

#include "windowfunctions_defs.h"

#endif  // WINDOWFUNCS_H
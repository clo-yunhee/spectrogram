#include "windowfunctions.h"

// clang-format off

const WindowFunctions::FunctionType WindowFunctions::g_funcs[WindowFunctions::TypeLast] = {
    w::rectangular,     
    w::barlett,       
    w::hamming,
    w::hann, 
    w::blackman,
    w::blackmanNuttall,
    w::blackmanHarris,
    w::kaiser4, 
    w::kaiser8, 
    w::kaiser16,
};

const QStringList WindowFunctions::g_names = {
    "Rectangular",
    "Barlett",
    "Hamming",
    "Hann",
    "Blackman",
    "Blackman-Nuttall",
    "Blackman-Harris",
    qUnicode("Kaiser (\U0001d70B\U0001d6fc = 4, \U0001d6fc = 1.27)"),
    qUnicode("Kaiser (\U0001d70b\U0001d6fc = 8, \U0001d6fc = 2.55)"),
    qUnicode("Kaiser (\U0001d70b\U0001d6fc = 16, \U0001d6fc = 5.09)"),
};
#ifndef UTILS_H
#define UTILS_H

#include <QString>

#ifdef _WIN32
#define qUnicode(lit) QString::fromWCharArray(L##lit)
#else
#define qUnicode(lit) QString::fromUtf8(lit)
#endif

#endif UTILS_H
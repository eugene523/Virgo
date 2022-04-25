#ifndef VIRGO_UTILS_H
#define VIRGO_UTILS_H

#include <string>

bool NearlyEqual(long double a, long double b, long double delta);

int RandomInt(int min, int max);

std::string numSep(uint64_t num);

#endif //VIRGO_UTILS_H

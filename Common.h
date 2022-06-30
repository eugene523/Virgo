#ifndef VIRGO_COMMON_H
#define VIRGO_COMMON_H

#include <limits>

using v_int  = long long int;
using v_real = long double;
using uint   = unsigned int;

#define v_int_min (std::numeric_limits<long long int>.min())
#define v_int_max (std::numeric_limits<long long int>.max())
#define v_real_min (std::numeric_limits<long double>.min())
#define v_real_max (std::numeric_limits<long double>.max())

#endif //VIRGO_COMMON_H

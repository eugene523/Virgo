#include <cstdlib>
#include <cmath>
#include "Common.h"

int RandomInt(int min, int max)
{
    uint a   = max - min + 1;
    int rnd = rand() % a + min;
    return rnd;
}

std::string numSep(uint64_t num) {
    if (num < 1000)
        return std::to_string(num);

    char symbols[] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    char buffer[32] {};
    int  numOfProceedDigits = 0;
    int  pos = 0;
    while (num > 0) {
        buffer[pos] = symbols[num % 10];
        numOfProceedDigits++;
        pos++;
        if (numOfProceedDigits % 3 == 0) {
            buffer[pos] = '\'';
            pos++;
        }
        num /= 10;
    }

    pos--;
    if (buffer[pos] == '\'') {
        buffer[pos] = 0;
        pos--;
    }

    std::stringstream s;
    for ( ; pos >= 0 ; pos--) {
        s << buffer[pos];
    }
    return s.str();
}
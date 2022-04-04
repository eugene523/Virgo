#include <cstdlib>
#include <cmath>

int RandomInt(int min, int max)
{
    int a   = max - min + 1;
    int rnd = rand() % a + min;
    return rnd;
}

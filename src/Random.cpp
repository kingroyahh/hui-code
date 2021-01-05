#include <math.h>
#include "Random.h"
#include "Type.h"
#include "Log.h"

Random::Random()
{
    randomSeed = 0;
}

Random::~Random()
{
}

int Random::SetRandomSeed(int seedValue)
{
    randomSeed = seedValue;
    return randomSeed;
}

int Random::GetSeed()
{
    return randomSeed;
}

int Random::doRand()
{
    int hi = 0;
    int lo = 0;
    int x = 0;
    if (randomSeed == 0)
    {
        randomSeed = 123459876;
    }
    
    hi = floor(randomSeed / 127773);
    lo = randomSeed % 127773;
    
    x = 16807 * lo  - 2836 * hi;
    if (x < 0)
    {
        x += RANDOM_MAX;
    }
    
    randomSeed = x;
    
    return (randomSeed % RANDOM_MAX);
}

int Random::RandForWidth(int width)
{
    //TODO: server start set one time
//    randomIndex = randomIndex + 123456;
//    srand(randomIndex);
    int r = doRand() % width + 1;
    debug("show random : %d\n", r);
    return r;
}

int Random::RandForWidthFromZero(int width)
{
    //TODO: server start set one time
    //    randomIndex = randomIndex + 123456;
    //    srand(randomIndex);
    int r = doRand() % width;
    debug("show random : %d\n", r);
    return r;
}

void Random::RandIntArray(int randList[],int arrySize)
{
    for (unsigned x = 0; x < (unsigned)arrySize; ++x)
    {
        int r = doRand() % arrySize;
        int a = randList[r];
        int b = randList[arrySize - 1];
        randList[arrySize - 1] = a;
        randList[r] = b;
    }
}

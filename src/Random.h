#ifndef _GRAV_RANDOM_H_
#define _GRAV_RANDOM_H_

class Random
{
public:
    
    int SetRandomSeed(int seedValue);
    int GetSeed();
    
    int RandForWidth(int width);
    int RandForWidthFromZero(int width);
    void RandIntArray(int randList[],int arrySize);
    
    Random();
    virtual ~Random();
    
private:
    int doRand();
    int randomSeed;
    
};
#endif

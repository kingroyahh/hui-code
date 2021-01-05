#ifndef _GRAV_ACTOR_DATAS_H_
#define _GRAV_ACTOR_DATAS_H_
#include "Type.h"
#include "Enum.h"

#define ACTOR_XP_CONSUME 4
#define ACTOR_XP_RESTORE 2

class ActorDatas
{
public:
    ActorPtr actor;
    int hpOrigin;
    double hp;
    int xp;
    double hpMax;
    int xpMax;
    bool reAliving = false;
    void Init(ActorPtr actorParam, double hpInit, int xpInit, double hpMax,int xpMax);
    void ChangeHp(double hpChange);
    void ChangeXp(int xpChange);
    void ReAlive();
    bool CanDoXpSkill();
    bool IsAlive();
    
    double GetHpPercent();
    
    int GetDeltaHp();
    double doubleHitRate;
    ActorDatas();
    virtual ~ActorDatas();

private:
};

#endif

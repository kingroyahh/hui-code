#include "ActorDatas.h"
#include "Actor.h"
#include "Enum.h"
#include "BattleField.h"
#include "EventData.h"
#include "Log.h"

void ActorDatas::Init(ActorPtr actorParam, double hpInit, int xpInit, double hpMaxValue, int xpMaxValue)
{
    hpOrigin = hpInit;
    hp = hpOrigin;
    xp = xpInit;
    xpMax = xpMaxValue;
    hpMax = hpMaxValue;
    actor = actorParam;
}

ActorDatas::ActorDatas()
{
}

ActorDatas::~ActorDatas()
{

}

void ActorDatas::ChangeHp(double hpChange)
{
    TRACE("ActorDatas::ChangeHp actorID[%d] hpChange[%d]", actor->actorId, hpChange);
    hp += hpChange;
    if (hp >= hpMax)
    {
        hp = hpMax;
    }
    if (hp <= 0)
    {
        debug("dead actor : %d\n", actor->actorId);
        hp = 0;
    }
    
    if(hp>0){
        reAliving = false;
    }
}

void ActorDatas::ChangeXp(int xpChange)
{
    TRACE("ActorDatas::ChangeXp actor[%d] xpChange[%d]", actor->actorId, xpChange);
    xp += xpChange;
    if (xp > xpMax)
    {
        xp = xpMax;
    }
    if (xp < 0)
    {
        xp = 0;
    }
}

void ActorDatas::ReAlive()
{
    reAliving = true;
}
bool ActorDatas::CanDoXpSkill()
{
    if (actor->battleField->InfiniteSkill)
    {
        return true;
    }
    if (xp >= xpMax)
    {
        return true;
    }
    return false;
}

bool ActorDatas::IsAlive()
{
    if (hp <= 0)
    {
        if(reAliving)
        {
            return true;
        }
        return false;
    }
    
    return true;
}

double ActorDatas::GetHpPercent()
{
    double per =  hp * (double)1.0 / hpMax;
    TRACE("hp percent : %f, actorId : %d", per, actor->actorId);
    return per;
}

int ActorDatas::GetDeltaHp()
{
    return (hp - hpOrigin);
}

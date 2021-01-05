//
//  ConditionCheck.cpp
//  grav_test
//
//  Created by kingroy on 16/10/18.
//  Copyright © 2016年 Babeltime. All rights reserved.
//

#include "ConditionCheck.h"
#include "Type.h"
#include "Enum.h"
#include "Actor.h"
#include "ActorModifier.h"

bool CheckMoreHp(ActorPtr actor, int param)
{
    if(actor != 0)
    {
        double precent = actor->actorDatas.GetHpPercent();
        if (precent * 100 >= param)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    
}
bool CheckLessHp(ActorPtr actor, int param)
{
    if(actor != 0)
    {
        double precent = actor->actorDatas.GetHpPercent();
        if (precent * 100 <= param)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
bool CheckMoreXp(ActorPtr actor, int param)
{
    if(actor != 0)
    {
        if (actor->actorDatas.xp >= param)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

}
bool CheckLessXp(ActorPtr actor, int param)
{
    if(actor != 0)
    {
        if (actor->actorDatas.xp <= param)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
bool CheckHaveSpecialBuff(ActorPtr actor, int param)
{
    if(actor != 0)
    {
        return actor->actorMod->GetModCntByGroupID(param) > 0;
    }
    else
    {
        return false;
    }
}
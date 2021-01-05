//
//  BaseCalc.h
//  grav_test
//
//  Created by Andrew Li on 7/26/16.
//  Copyright © 2016 Babeltime. All rights reserved.
//

#ifndef _GRAV_BASE_CALC_H_
#define _GRAV_BASE_CALC_H_

#include "Type.h"
#include "Enum.h"
#include "BattleModeMeta.h"
class BaseCalc
{
protected:
    BaseCalc(){}
    virtual ~BaseCalc() {}
public:

    virtual void LoadAll();
    /*virtual double CalculateLifeSteal(double, ActorPtr) = 0;
    virtual double CalculateResilience(double, ActorPtr) = 0;
    virtual double CalculateHeal(ActorPtr, ActorPtr, double, double,bool) = 0;
    virtual double CalculateHitChance(DamageType, ActorPtr, ActorPtr) = 0;
    virtual double CalculateStrikeDamage(double, ActorPtr, ActorPtr) = 0;
    virtual double CalculateStrikeChance(DamageType, ActorPtr, ActorPtr) = 0;
    virtual double CalculateBlockDamage(double damage) = 0;
    virtual double CalculateBlockChance(DamageType, ActorPtr, ActorPtr) = 0;
    virtual double CalculateDamage(DamageType, ActorPtr, ActorPtr,double, double, double,double,EBattleType,bool) = 0;
    virtual double CalculateShield(ActorAttrType, ActorPtr, double, double) = 0;
    virtual double CalculateDamageXpRecovery(double damage, ActorPtr) = 0;
    virtual double CalculateHitXpRecovery(double damage, ActorPtr) = 0;
    virtual double CalculateHealXpRecovery(double damage, ActorPtr) = 0;*/
};

#endif /* _GRAV_BASE_CALC_H_ */

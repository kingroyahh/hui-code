//
//  NativeCalc.h
//  grav
//
//  Created by Andrew Li on 7/26/16.
//  Copyright © 2016 Babeltime. All rights reserved.
//

#ifndef _GRAV_NATIVE_CALC_H_
#define _GRAV_NATIVE_CALC_H_

#include "BaseCalc.h"
#include "Singleton.h"

class NativeCalc : public Singleton<NativeCalc>
{
private:
    typedef struct
    {
        double atkValue;
        double atkPer;
        double attrValue;
        double skillAddition;
        double levelAddition;
        double finalAddition;
        double campAddition;
        double pvpDamage;
        bool isComboAffect;
        double doubleHitRate;
        double doubleHitRateForceZero;
    } CasterAttrs;
    
    typedef struct
    {
        double defValue;
        double defPer;
        double attrValue;
        double skillReduction;
        double finalReduction;
        double campReduction;
    } TargetAttrs;
    double GetAttrByTypeCalc(ActorAttrType, ActorPtr);
    double CalculateClosePhysical(ActorPtr, ActorPtr, double, double, double, double, double, double, EBattleType, bool,double);
    //double CalculateRemotePhysical(ActorPtr, ActorPtr, double, double, double, double, double, int, EBattleType);
    //double CalculateIceMagical(ActorPtr, ActorPtr, double, double, double, double, double, int, EBattleType);
    double CalculateFireMagical(ActorPtr, ActorPtr, double, double, double, double, double, double, EBattleType, bool,double);
    //double CalculateThunderMagical(ActorPtr, ActorPtr, double, double, double, double, double, int, EBattleType);

    double CalculateDamageBridge(CasterAttrs, TargetAttrs, double, double, double, double,double);

    double CalculateRealDamage(CasterAttrs, TargetAttrs, double, double, double, double,double);
    double CalculateAtLeastDamage(CasterAttrs, TargetAttrs, double, double, double, double,double);

public:
    double blockRateArg;
    double strikeRateArg;
    double healStrikeRateArg;
    double damageAdditionArg;
    double damageReductionArg;
    double damageFix1;
    double damageFix2;
    virtual void LoadAll();
public:
    double CalculateLifeSteal(double, ActorPtr);
    double CalculateResilience(double, ActorPtr);
    double CalculateHeal(ActorPtr, ActorPtr, double, double,bool);
    
    double CalculateHitChance(DamageType, ActorPtr, ActorPtr);
    
    double CalculateStrikeDamage(double, ActorPtr, ActorPtr);
    double CalculateStrikeChance(DamageType, ActorPtr, ActorPtr);
    
    double CalculateBlockDamage(double damage);
    double CalculateBlockChance(DamageType, ActorPtr, ActorPtr);
    
    double CalculateDamage(DamageType, ActorPtr, ActorPtr, double, double, double, double, EBattleType,bool,double);
    
    double CalculateShield(ActorAttrType,ActorPtr,ActorPtr, double, double);

    double CalculateDamageXpRecovery(double damage, ActorPtr actor);
    double CalculateHitXpRecovery(double hitToRecory, ActorPtr actor);
    double CalculateHealXpRecovery(double hitToRecory, ActorPtr actor);
};


#endif /* _GRAV_NATIVE_CALC_H_ */

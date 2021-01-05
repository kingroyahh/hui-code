//
//  Calculator.cpp
//  grav
//
//  Created by Andrew Li on 7/26/16.
//  Copyright © 2016 Babeltime. All rights reserved.
//

#include "Calculator.h"
#include "LuaCalc.h"
#include "NativeCalc.h"

#ifdef GRAV_LUA_CALC
bool use_lua = true;
#else
bool use_lua = false;
#endif

double CalculateDamageXpRecovery(double damage, ActorPtr target)
{
    return NativeCalc::instance()->CalculateDamageXpRecovery(damage, target);
}
double CalculateHitXpRecovery(double hitToRecory, ActorPtr target)
{
    return NativeCalc::instance()->CalculateHitXpRecovery(hitToRecory, target);
}

double CalculateHealXpRecovery(double hitToRecory, ActorPtr target)
{
    return NativeCalc::instance()->CalculateHealXpRecovery(hitToRecory, target);
}
double CalculateLifeSteal(double damage, ActorPtr caster)
{
    return use_lua ? LuaCalc::instance()->CalculateLifeSteal(damage, caster) :
        NativeCalc::instance()->CalculateLifeSteal(damage, caster);
}
double CalculateResilience(double damage, ActorPtr target)
{
    return NativeCalc::instance()->CalculateResilience(damage, target);
}

double CalculateHeal(ActorPtr caster, ActorPtr target, double skillFactor, double skillDamage,bool ignoreAttr)
{
    return NativeCalc::instance()->CalculateHeal(caster, target, skillFactor, skillDamage,ignoreAttr);
}

double CalculateHitChance(DamageType dType, ActorPtr caster, ActorPtr target)
{
    return use_lua ? LuaCalc::instance()->CalculateHitChance(dType, caster, target) :
        NativeCalc::instance()->CalculateHitChance(dType, caster, target);
}

double CalculateStrikeDamage(double damage, ActorPtr caster, ActorPtr target)
{
    return use_lua ? LuaCalc::instance()->CalculateStrikeDamage(damage, caster, target) :
        NativeCalc::instance()->CalculateStrikeDamage(damage, caster, target);
}

double CalculateStrikeChance(DamageType dType, ActorPtr caster, ActorPtr target)
{
    return use_lua ? LuaCalc::instance()->CalculateStrikeChance(dType, caster, target) :
        NativeCalc::instance()->CalculateStrikeChance(dType, caster, target);
}

double CalculateBlockDamage(double damage)
{
    return use_lua ? LuaCalc::instance()->CalculateBlockDamage(damage) :
        NativeCalc::instance()->CalculateBlockDamage(damage);
}

double CalculateBlockChance(DamageType dType, ActorPtr caster, ActorPtr target)
{
    return use_lua ? LuaCalc::instance()->CalculateBlockChance(dType, caster, target) :
        NativeCalc::instance()->CalculateBlockChance(dType, caster, target);
}

double CalculateDamage(DamageType dType, ActorPtr caster, ActorPtr target,
    double skillFactor, double skillDamage, double skillFinalDamage, double doubleHitRate, EBattleType combatType, bool ignoreDefence, double debuffDmgRate)
{
    return NativeCalc::instance()->CalculateDamage(dType, caster, target, skillFactor, skillDamage, skillFinalDamage, doubleHitRate, combatType, ignoreDefence, debuffDmgRate);
}

double CalculateShield(ActorAttrType attrType, ActorPtr caster , ActorPtr actor, double skillFactor, double skillValue)
{
    return use_lua ? LuaCalc::instance()->CalculateShield(attrType, actor, skillFactor, skillValue) :
        NativeCalc::instance()->CalculateShield(attrType,caster,actor, skillFactor, skillValue);
}




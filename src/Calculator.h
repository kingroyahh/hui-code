//
//  Calculator.h
//  grav
//
//  Created by Andrew Li on 7/26/16.
//  Copyright © 2016 Babeltime. All rights reserved.
//

#ifndef _GRAV_CALCULATOR_H_
#define _GRAV_CALCULATOR_H_

#include "Type.h"
#include "Enum.h"
#include "BattleModeMeta.h"

double CalculateDamageXpRecovery(double, ActorPtr);
double CalculateHitXpRecovery(double, ActorPtr);
double CalculateHealXpRecovery(double, ActorPtr);
double CalculateLifeSteal(double, ActorPtr);
double CalculateResilience(double, ActorPtr);
double CalculateHeal(ActorPtr, ActorPtr, double, double,bool);
double CalculateHitChance(DamageType, ActorPtr, ActorPtr);
double CalculateStrikeDamage(double, ActorPtr, ActorPtr);
double CalculateStrikeChance(DamageType, ActorPtr, ActorPtr);
double CalculateBlockDamage(double damage);
double CalculateBlockChance(DamageType, ActorPtr, ActorPtr);
double CalculateDamage(DamageType, ActorPtr, ActorPtr,double, double, double,double,EBattleType,bool,double);
double CalculateShield(ActorAttrType, ActorPtr ,ActorPtr, double, double);
#endif /* _GRAV_CALCULATOR_H_ */

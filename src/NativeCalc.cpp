//
//  NativeCalc.cpp
//  grav
//
//  Created by Andrew Li on 7/26/16.
//  Copyright © 2016 Babeltime. All rights reserved.
//

#include "NativeCalc.h"
#include "Log.h"
#include "Enum.h"
#include "Type.h"
#include "Actor.h"
#include "ValueMeta.h"
#include <algorithm>


void NativeCalc::LoadAll()
{
    debug("BaseCalc::LoadAll");
    const ValueMeta *meta1 = ValueMetaSet::instance()->Get(DAMAGE_ADDITION_ARG);
    if (meta1 == NULL_PTR)
    {
        error("BaseCalc value meta cant read! : %d", DAMAGE_ADDITION_ARG);
        return;
    }
    damageAdditionArg = meta1->floatValue;

    const ValueMeta *meta2 = ValueMetaSet::instance()->Get(DAMAGE_REDUCTION_ARG);
    if (meta2 == NULL_PTR)
    {
        error("BaseCalc value meta cant read! : %d", DAMAGE_REDUCTION_ARG);
        return;
    }
    damageReductionArg = meta2->floatValue;

    const ValueMeta *meta3 = ValueMetaSet::instance()->Get(STRIKE_DAMAGE_ARG);
    if (meta3 == NULL_PTR)
    {
        error("BaseCalc value meta cant read! : %d", STRIKE_DAMAGE_ARG);
        return;
    }
    strikeRateArg = meta3->floatValue;

    const ValueMeta *meta4 = ValueMetaSet::instance()->Get(BLOCK_RATE_ARG);
    if (meta4 == NULL_PTR)
    {
        error("BaseCalc value meta cant read! : %d", BLOCK_RATE_ARG);
        return;
    }
    blockRateArg = meta4->floatValue;

    const ValueMeta *meta5 = ValueMetaSet::instance()->Get(DAMAGE_ARG_1);
    if (meta5 == NULL_PTR)
    {
        error("BaseCalc value meta cant read! : %d", DAMAGE_ARG_1);
        return;
    }
    damageFix1 = meta5->floatValue;

    const ValueMeta *meta6 = ValueMetaSet::instance()->Get(DAMAGE_ARG_2);
    if (meta6 == NULL_PTR)
    {
        error("BaseCalc value meta cant read! : %d", DAMAGE_ARG_2);
        return;
    }
    damageFix2 = meta6->floatValue;

    const ValueMeta *meta7 = ValueMetaSet::instance()->Get(HEAL_STRIKE_DAMAGE_ARG);
    if (meta3 == NULL_PTR)
    {
        error("BaseCalc value meta cant read! : %d", HEAL_STRIKE_DAMAGE_ARG);
        return;
    }
    healStrikeRateArg = meta7->floatValue;
}

double NativeCalc::CalculateClosePhysical(ActorPtr caster, ActorPtr target,
                                         double skillFactor, double skillDamage,
                                         double skillFinalDamage, double campAddition, double campReduction, double doubleHitRate, EBattleType combatType, bool ignoreDefence, double debuffDmgRate)
{
    CasterAttrs casterAttr;
    memset(&casterAttr, 0, sizeof(casterAttr));
    casterAttr.atkValue = GetAttrByTypeCalc(ClosePhysicalAttack, caster);
    casterAttr.atkPer = GetAttrByTypeCalc(AttackPercentAddition, caster);
    casterAttr.attrValue = GetAttrByTypeCalc(AttrCloseDamageAddition, caster);
    casterAttr.skillAddition = GetAttrByTypeCalc(SkillCloseDamageAddition, caster);
    casterAttr.levelAddition = caster->GetAttrByType(LevelDamagePercentAddition);
    casterAttr.finalAddition = caster->GetAttrByType(FinalDamageAddition);
    casterAttr.campAddition = campAddition;
    casterAttr.isComboAffect = caster->actorAttrs.IsComboAffect();
    casterAttr.doubleHitRateForceZero = caster->actorAttrs.GetAttrByType(DoubleHitRateForceZero);
    casterAttr.doubleHitRate = caster->actorAttrs.GetAttrByType(DoubleHitRate);
    if (combatType == BM_Pvp)
    {
        casterAttr.pvpDamage = caster->GetAttrByType(PvpDmgAddition) - target->GetAttrByType(PvpDmgReduction);
    }
    else
    {
        casterAttr.pvpDamage = 0;
    }
    
    debug("caster atk:%f, per:%f, attr:%f, skill:%f, level:%f, final:%f, camp:%f, pvp:%f\n", casterAttr.atkValue, casterAttr.atkPer,
          casterAttr.attrValue, casterAttr.skillAddition, casterAttr.levelAddition, casterAttr.finalAddition, casterAttr.campAddition, casterAttr.pvpDamage);

    
    TargetAttrs targetAttr;
    memset(&targetAttr, 0, sizeof(targetAttr));
    targetAttr.defValue = GetAttrByTypeCalc(ClosePhysicalDefense, target);
    targetAttr.defPer = GetAttrByTypeCalc(DefensePercentAddition, target);
    targetAttr.attrValue = GetAttrByTypeCalc(AttrCloseDamageReduction, target);
    targetAttr.skillReduction = GetAttrByTypeCalc(SkillCloseDamageReduction, target);
    targetAttr.finalReduction = target->GetAttrByType(FinalDamageReduction);
    targetAttr.campReduction = campReduction;
    debug("target def:%f, per:%f, attr:%f, skill:%f, final:%f, camp:%f\n", targetAttr.defValue,
          targetAttr.defPer, targetAttr.attrValue, targetAttr.skillReduction, targetAttr.finalReduction, targetAttr.campReduction);
    return CalculateDamageBridge(casterAttr, targetAttr, skillFactor, skillDamage, skillFinalDamage, doubleHitRate, debuffDmgRate);
}
//
//double NativeCalc::CalculateRemotePhysical(ActorPtr caster, ActorPtr target,
//                                          double skillFactor, double skillDamage,
//                                          double skillFinalDamage, double campAddition, double campReduction, int doubleHitIndex, EBattleType combatType)
//{
//    CasterAttrs casterAttr;
//    memset(&casterAttr, 0, sizeof(casterAttr));
//    casterAttr.atkValue = GetAttrByTypeCalc(RemotePhysicalAttack, caster);
//    casterAttr.atkPer = GetAttrByTypeCalc(AttackPercentAddition, caster);
//    casterAttr.attrValue = GetAttrByTypeCalc(AttrRemoteDamageAddition, caster);
//    casterAttr.skillAddition = GetAttrByTypeCalc(SkillRemoteDamageAddition, caster);
//    casterAttr.levelAddition = caster->actorAttrs.GetAttrByType(LevelDamagePercentAddition);
//    casterAttr.finalAddition = caster->actorAttrs.GetAttrByType(FinalDamageAddition);
//    casterAttr.campAddition = campAddition;
//    if (combatType == BM_Pvp)
//    {
//        casterAttr.pvpDamage = caster->GetAttrByType(PvpDmgAddition) - target->GetAttrByType(PvpDmgReduction);
//    }
//    else
//    {
//        casterAttr.pvpDamage = 0;
//    }
//    
//    TargetAttrs targetAttr;
//    memset(&targetAttr, 0, sizeof(targetAttr));
//    targetAttr.defValue = GetAttrByTypeCalc(RemotePhysicalDefense, target);
//    targetAttr.defPer = GetAttrByTypeCalc(DefensePercentAddition, target);
//    targetAttr.attrValue = GetAttrByTypeCalc(AttrRemoteDamageReduction, target);
//    targetAttr.skillReduction = GetAttrByTypeCalc(SkillRemoteDamageReduction, target);
//    targetAttr.finalReduction = target->GetAttrByType(FinalDamageReduction);
//    targetAttr.campReduction = campReduction;
//    
//    return CalculateDamageBridge(casterAttr, targetAttr, skillFactor, skillDamage, skillFinalDamage, doubleHitIndex);
//}
//
//double NativeCalc::CalculateIceMagical(ActorPtr caster, ActorPtr target,
//                                      double skillFactor, double skillDamage,
//                                      double skillFinalDamage, double campAddition, double campReduction, int doubleHitIndex, EBattleType combatType)
//{
//    CasterAttrs casterAttr;
//    memset(&casterAttr, 0, sizeof(casterAttr));
//    casterAttr.atkValue = GetAttrByTypeCalc(IceMagicalAttack, caster);
//    casterAttr.atkPer = GetAttrByTypeCalc(AttackPercentAddition, caster);
//    casterAttr.attrValue = GetAttrByTypeCalc(AttrIceDamageAddition, caster);
//    casterAttr.skillAddition = GetAttrByTypeCalc(SkillIceDamageAddition, caster);
//    casterAttr.levelAddition = caster->actorAttrs.GetAttrByType(LevelDamagePercentAddition);
//    casterAttr.finalAddition = caster->actorAttrs.GetAttrByType(FinalDamageAddition);
//    casterAttr.campAddition = campAddition;
//    if (combatType == BM_Pvp)
//    {
//        casterAttr.pvpDamage = caster->GetAttrByType(PvpDmgAddition) - target->GetAttrByType(PvpDmgReduction);
//    }
//    else
//    {
//        casterAttr.pvpDamage = 0;
//    }
//    
//    TargetAttrs targetAttr;
//    memset(&targetAttr, 0, sizeof(targetAttr));
//    targetAttr.defValue = GetAttrByTypeCalc(IceMagicalDefense, target);
//    targetAttr.defPer = GetAttrByTypeCalc(DefensePercentAddition, target);
//    targetAttr.attrValue = GetAttrByTypeCalc(AttrIceDamageReduction, target);
//    targetAttr.skillReduction = GetAttrByTypeCalc(SkillIceDamageReduction, target);
//    targetAttr.finalReduction = target->GetAttrByType(FinalDamageReduction);
//    targetAttr.campReduction = campReduction;
//    
//    return CalculateDamageBridge(casterAttr, targetAttr, skillFactor, skillDamage, skillFinalDamage, doubleHitIndex);
//}

double NativeCalc::CalculateFireMagical(ActorPtr caster, ActorPtr target,
                                       double skillFactor, double skillDamage,
                                       double skillFinalDamage, double campAddition, double campReduction, double doubleHitRate, EBattleType combatType, bool ignoreDefence, double debuffDmgRate)
{
    CasterAttrs casterAttr;
    memset(&casterAttr, 0, sizeof(casterAttr));
    casterAttr.atkValue = GetAttrByTypeCalc(FireMagicalAttack, caster);
    casterAttr.atkPer = GetAttrByTypeCalc(AttackPercentAddition, caster);
    casterAttr.attrValue = GetAttrByTypeCalc(AttrFireDamageAddition, caster);
    casterAttr.skillAddition = GetAttrByTypeCalc(SkillFireDamageAddition, caster);
    casterAttr.levelAddition = caster->actorAttrs.GetAttrByType(LevelDamagePercentAddition);
    casterAttr.finalAddition = caster->actorAttrs.GetAttrByType(FinalDamageAddition);
    casterAttr.campAddition = campAddition;
    casterAttr.isComboAffect = caster->actorAttrs.IsComboAffect();
    casterAttr.doubleHitRate = caster->actorAttrs.GetAttrByType(DoubleHitRate);
    casterAttr.doubleHitRateForceZero = caster->actorAttrs.GetAttrByType(DoubleHitRateForceZero);
    if (combatType == BM_Pvp)
    {
        casterAttr.pvpDamage = caster->GetAttrByType(PvpDmgAddition) - target->GetAttrByType(PvpDmgReduction);
    }
    else
    {
        casterAttr.pvpDamage = 0;
    }
    
    TargetAttrs targetAttr;
    memset(&targetAttr, 0, sizeof(targetAttr));
    targetAttr.defValue = GetAttrByTypeCalc(FireMagicalDefense, target);
    targetAttr.defPer = GetAttrByTypeCalc(DefensePercentAddition, target);
    targetAttr.attrValue = GetAttrByTypeCalc(AttrFireDamageReduction, target);
    targetAttr.skillReduction = GetAttrByTypeCalc(SkillFireDamageReduction, target);
    targetAttr.finalReduction = target->GetAttrByType(FinalDamageReduction);
    targetAttr.campReduction = campReduction;
    
    return CalculateDamageBridge(casterAttr, targetAttr, skillFactor, skillDamage, skillFinalDamage, doubleHitRate, debuffDmgRate);
}
//
//double NativeCalc::CalculateThunderMagical(ActorPtr caster, ActorPtr target,
//                                          double skillFactor, double skillDamage,
//                                          double skillFinalDamage, double campAddition, double campReduction, int doubleHitIndex, EBattleType combatType)
//{
//    CasterAttrs casterAttr;
//    memset(&casterAttr, 0, sizeof(casterAttr));
//    casterAttr.atkValue = GetAttrByTypeCalc(ThunderMagicalAttack, caster);
//    casterAttr.atkPer = GetAttrByTypeCalc(AttackPercentAddition, caster);
//    casterAttr.attrValue = GetAttrByTypeCalc(AttrThunderDamageAddition, caster);
//    casterAttr.skillAddition = GetAttrByTypeCalc(SkillThunderDamageAddition, caster);
//    casterAttr.levelAddition = caster->actorAttrs.GetAttrByType(LevelDamagePercentAddition);
//    casterAttr.finalAddition = caster->actorAttrs.GetAttrByType(FinalDamageAddition);
//    casterAttr.campAddition = campAddition;
//    if (combatType == BM_Pvp)
//    {
//        casterAttr.pvpDamage = caster->GetAttrByType(PvpDmgAddition) - target->GetAttrByType(PvpDmgReduction);
//    }
//    else
//    {
//        casterAttr.pvpDamage = 0;
//    }
//    
//    TargetAttrs targetAttr;
//    memset(&targetAttr, 0, sizeof(targetAttr));
//    targetAttr.defValue = GetAttrByTypeCalc(ThunderMagicalDefense, target);
//    targetAttr.defPer = GetAttrByTypeCalc(DefensePercentAddition, target);
//    targetAttr.attrValue = GetAttrByTypeCalc(AttrThunderDamageReduction, target);
//    targetAttr.skillReduction = GetAttrByTypeCalc(SkillThunderDamageReduction, target);
//    targetAttr.finalReduction = target->GetAttrByType(FinalDamageReduction);
//    targetAttr.campReduction = campReduction;
//    
//    return CalculateDamageBridge(casterAttr, targetAttr, skillFactor, skillDamage, skillFinalDamage, doubleHitIndex);
//}

double NativeCalc::CalculateDamageBridge(CasterAttrs caster, TargetAttrs target,
    double skillFactor, double skillDamage, double skillFinalDamage, double doubleHitRate, double debuffDmgRate)
{
    if (caster.atkValue*caster.atkPer*0.9 > target.defValue*target.defPer)
    {
        return CalculateRealDamage(caster, target, skillFactor, skillDamage, skillFinalDamage, doubleHitRate, debuffDmgRate);
    }
    else
    {
        return CalculateAtLeastDamage(caster, target, skillFactor, skillDamage, skillFinalDamage, doubleHitRate, debuffDmgRate);
    }
}

double NativeCalc::CalculateRealDamage(CasterAttrs caster, TargetAttrs target,
    double skillFactor, double skillDamage, double skillFinalDamage, double doubleHitRate, double debuffDmgRate)
{
    double damageFix = damageFix1;
    double damage = 0;
    double attrDamageNum = max(damageAdditionArg, 1 + caster.attrValue - target.attrValue);
    damage = ((caster.atkValue * caster.atkPer - target.defValue * target.defPer) * damageFix * skillFactor + skillDamage)
//  *(attrDamageNum + caster.campAddition - target.campReduction + caster.pvpDamage) *
     *(attrDamageNum *(1 + caster.campAddition - target.campReduction)*(1 + caster.pvpDamage) ) *
    caster.skillAddition*target.skillReduction*(1 + caster.levelAddition) + skillFinalDamage + caster.finalAddition - target.finalReduction;
    //debug("attrDamageNum:%f, real damage:%f\n", attrDamageNum, damage);
    if(caster.isComboAffect)
    {
        if(caster.doubleHitRateForceZero != 1)
        {
            damage *= (1 + caster.doubleHitRate);
        }
    }
    else
    {
        damage *= (1 + doubleHitRate);
    }
    damage *= (1 + debuffDmgRate);
    if(damage <= 0)
    {
        damage = 1;
    }
    return damage;
}

double NativeCalc::CalculateAtLeastDamage(CasterAttrs caster, TargetAttrs target,
    double skillFactor, double skillDamage, double skillFinalDamage, double doubleHitRate, double debuffDmgRate)
{
    double damageFix = damageFix2;
    double damage = 0;
    double attrDamageNum = max(damageAdditionArg, 1 + caster.attrValue - target.attrValue);
    damage = (caster.atkValue*caster.atkPer*damageFix*skillFactor + skillDamage)*
    (attrDamageNum + caster.campAddition - target.campReduction + caster.pvpDamage)*caster.skillAddition*target.skillReduction
    *(1 + caster.levelAddition) + skillFinalDamage + caster.finalAddition - target.finalReduction;
    if(caster.isComboAffect)
    {
        if(caster.doubleHitRateForceZero != 1)
        {
            damage *= (1 + caster.doubleHitRate);
        }
    }
    else
    {
        damage *= (1 + doubleHitRate);
    }
    
    damage *= (1 + debuffDmgRate);
    //debug("damageFix2: %f, attrDamageNum:%f, atLeast damage:%f\n", damageFix, attrDamageNum, damage);
    if(damage <= 0)
    {
        damage = 1;
    }
    return damage;
}

double NativeCalc::GetAttrByTypeCalc(ActorAttrType dType, ActorPtr actor)
{
    switch (dType)
    {
            //attack
        case Attack:
            return actor->GetAttrByType(Attack);
        case ClosePhysicalAttack:
            return actor->GetAttrByType(Attack) + actor->GetAttrByType(PhysicalAttack) + actor->GetAttrByType(ClosePhysicalAttack);
        case RemotePhysicalAttack:
            return actor->GetAttrByType(Attack) + actor->GetAttrByType(PhysicalAttack) + actor->GetAttrByType(RemotePhysicalAttack);
        case IceMagicalAttack:
            return actor->GetAttrByType(Attack) + actor->GetAttrByType(MagicalAttack) + actor->GetAttrByType(IceMagicalAttack);
        case FireMagicalAttack:
            return actor->GetAttrByType(Attack) + actor->GetAttrByType(MagicalAttack) + actor->GetAttrByType(FireMagicalAttack);
        case ThunderMagicalAttack:
            return actor->GetAttrByType(Attack) + actor->GetAttrByType(MagicalAttack) + actor->GetAttrByType(ThunderMagicalAttack);
        case AttackPercentAddition:
            return 1 + actor->GetAttrByType(AttackPercentAddition);
            
            //defense
        case ClosePhysicalDefense:
            return actor->GetAttrByType(Defense) + actor->GetAttrByType(PhysicalDefense) + actor->GetAttrByType(ClosePhysicalDefense);
        case RemotePhysicalDefense:
            return actor->GetAttrByType(Defense) + actor->GetAttrByType(PhysicalDefense) + actor->GetAttrByType(RemotePhysicalDefense);
        case IceMagicalDefense:
            return actor->GetAttrByType(Defense) + actor->GetAttrByType(MagicalDefense) + actor->GetAttrByType(IceMagicalDefense);
        case FireMagicalDefense:
            return actor->GetAttrByType(Defense) + actor->GetAttrByType(MagicalDefense) + actor->GetAttrByType(FireMagicalDefense);
        case ThunderMagicalDefense:
            return actor->GetAttrByType(Defense) + actor->GetAttrByType(MagicalDefense) + actor->GetAttrByType(ThunderMagicalDefense);
        case DefensePercentAddition:
            return 1 + actor->GetAttrByType(DefensePercentAddition);
            
            //attribute damage addition
        case AttrCloseDamageAddition:
            return actor->GetAttrByType(AttrDamageAddition) + actor->GetAttrByType(AttrPhysicalDamageAddition) + actor->GetAttrByType(AttrCloseDamageAddition);
        case AttrRemoteDamageAddition:
            return actor->GetAttrByType(AttrDamageAddition) + actor->GetAttrByType(AttrPhysicalDamageAddition) + actor->GetAttrByType(AttrRemoteDamageAddition);
        case AttrIceDamageAddition:
            return actor->GetAttrByType(AttrDamageAddition) + actor->GetAttrByType(AttrMagicalDamageAddition) + actor->GetAttrByType(AttrIceDamageAddition);
        case AttrFireDamageAddition:
            return actor->GetAttrByType(AttrDamageAddition) + actor->GetAttrByType(AttrMagicalDamageAddition) + actor->GetAttrByType(AttrFireDamageAddition);
        case AttrThunderDamageAddition:
            return actor->GetAttrByType(AttrDamageAddition) + actor->GetAttrByType(AttrMagicalDamageAddition) + actor->GetAttrByType(AttrThunderDamageAddition);
            
            //attribute damage reduction
        case AttrCloseDamageReduction:
            return actor->GetAttrByType(AttrDamageReduction) + actor->GetAttrByType(AttrPhysicalDamageReduction) + actor->GetAttrByType(AttrCloseDamageReduction);
        case AttrRemoteDamageReduction:
            return actor->GetAttrByType(AttrDamageReduction) + actor->GetAttrByType(AttrPhysicalDamageReduction) + actor->GetAttrByType(AttrRemoteDamageReduction);
        case AttrIceDamageReduction:
            return actor->GetAttrByType(AttrDamageReduction) + actor->GetAttrByType(AttrMagicalDamageReduction) + actor->GetAttrByType(AttrIceDamageReduction);
        case AttrFireDamageReduction:
            return actor->GetAttrByType(AttrDamageReduction) + actor->GetAttrByType(AttrMagicalDamageReduction) + actor->GetAttrByType(AttrFireDamageReduction);
        case AttrThunderDamageReduction:
            return actor->GetAttrByType(AttrDamageReduction) + actor->GetAttrByType(AttrMagicalDamageReduction) + actor->GetAttrByType(AttrThunderDamageReduction);
            
            //skill damage addition
        case SkillCloseDamageAddition:
            return max(damageAdditionArg, 1 + actor->GetAttrByType(SkillDamageAddition) + actor->GetAttrByType(SkillPhysicalDamageAddition) + actor->
                       GetAttrByType(SkillCloseDamageAddition));
        case SkillRemoteDamageAddition:
            return max(damageAdditionArg, 1 + actor->GetAttrByType(SkillDamageAddition) + actor->GetAttrByType(SkillPhysicalDamageAddition) + actor->
                       GetAttrByType(SkillRemoteDamageAddition));
        case SkillIceDamageAddition:
            return max(damageAdditionArg, 1 + actor->GetAttrByType(SkillDamageAddition) + actor->GetAttrByType(SkillMagicalDamageAddition) + actor->
                       GetAttrByType(SkillIceDamageAddition));
        case SkillFireDamageAddition:
            return max(damageAdditionArg, 1 + actor->GetAttrByType(SkillDamageAddition) + actor->GetAttrByType(SkillMagicalDamageAddition) + actor->
                       GetAttrByType(SkillFireDamageAddition));
        case SkillThunderDamageAddition:
            return max(damageAdditionArg, 1 + actor->GetAttrByType(SkillDamageAddition) + actor->GetAttrByType(SkillMagicalDamageAddition) + actor->
                       GetAttrByType(SkillThunderDamageAddition));
            
            //skill damage reduction
        case SkillCloseDamageReduction:
            return max(damageReductionArg, 1 - actor->GetAttrByType(SkillDamageReduction) - actor->GetAttrByType(SkillPhysicalDamageReduction) - actor->
                       GetAttrByType(SkillCloseDamageReduction));
        case SkillRemoteDamageReduction:
            return max(damageReductionArg, 1 - actor->GetAttrByType(SkillDamageReduction) - actor->GetAttrByType(SkillPhysicalDamageReduction) - actor->
                       GetAttrByType(SkillRemoteDamageReduction));
        case SkillIceDamageReduction:
            return max(damageReductionArg, 1 - actor->GetAttrByType(SkillDamageReduction) - actor->GetAttrByType(SkillMagicalDamageReduction) - actor->
                       GetAttrByType(SkillIceDamageReduction));
        case SkillFireDamageReduction:
            return max(damageReductionArg, 1 - actor->GetAttrByType(SkillDamageReduction) - actor->GetAttrByType(SkillMagicalDamageReduction) - actor->
                       GetAttrByType(SkillFireDamageReduction));
        case SkillThunderDamageReduction:
            return max(damageReductionArg, 1 - actor->GetAttrByType(SkillDamageReduction) - actor->GetAttrByType(SkillMagicalDamageReduction) - actor->
                       GetAttrByType(SkillThunderDamageReduction));
            
            //hit
        case ClosePhysicalHit:
            return actor->GetAttrByType(HitChance) + actor->GetAttrByType(PhysicalHit) + actor->GetAttrByType(ClosePhysicalHit);
        case RemotePysicalHit:
            return actor->GetAttrByType(HitChance) + actor->GetAttrByType(PhysicalHit) + actor->GetAttrByType(RemotePysicalHit);
        case IceMagicalHit:
            return actor->GetAttrByType(HitChance) + actor->GetAttrByType(MagicalHit) + actor->GetAttrByType(IceMagicalHit);
        case FireMagicalHit:
            return actor->GetAttrByType(HitChance) + actor->GetAttrByType(MagicalHit) + actor->GetAttrByType(FireMagicalHit);
        case ThunderMagicalHit:
            return actor->GetAttrByType(HitChance) + actor->GetAttrByType(MagicalHit) + actor->GetAttrByType(ThunderMagicalHit);
            
            //dodge;
        case ClosePhysicalDodge:
            return actor->GetAttrByType(DodgeChance) + actor->GetAttrByType(PhysicalDodge) + actor->GetAttrByType(ClosePhysicalDodge);
        case RemotePhysicalDodge:
            return actor->GetAttrByType(DodgeChance) + actor->GetAttrByType(PhysicalDodge) + actor->GetAttrByType(RemotePhysicalDodge);
        case IceMagicalDodge:
            return actor->GetAttrByType(DodgeChance) + actor->GetAttrByType(MagicalDodge) + actor->GetAttrByType(IceMagicalDodge);
        case FireMagicalDodge:
            return actor->GetAttrByType(DodgeChance) + actor->GetAttrByType(MagicalDodge) + actor->GetAttrByType(FireMagicalDodge);
        case ThunderMagicalDodge:
            return actor->GetAttrByType(DodgeChance) + actor->GetAttrByType(MagicalDodge) + actor->GetAttrByType(ThunderMagicalDodge);
            
            //strike
        case ClosePhysicalStrike:
            return actor->GetAttrByType(StrikeChance) + actor->GetAttrByType(PhysicalStrike) + actor->GetAttrByType(ClosePhysicalStrike);
        case RemotePhysicalStrike:
            return actor->GetAttrByType(StrikeChance) + actor->GetAttrByType(PhysicalStrike) + actor->GetAttrByType(RemotePhysicalStrike);
        case IceMagicalStrike:
            return actor->GetAttrByType(StrikeChance) + actor->GetAttrByType(MagicalStrike) + actor->GetAttrByType(IceMagicalStrike);
        case FireMagicalStrike:
            return actor->GetAttrByType(StrikeChance) + actor->GetAttrByType(MagicalStrike) + actor->GetAttrByType(FireMagicalStrike);
        case ThunderMagicalStrike:
            return actor->GetAttrByType(StrikeChance) + actor->GetAttrByType(MagicalStrike) + actor->GetAttrByType(ThunderMagicalStrike);
            
            //anti strike
        case ClosePhysicalAntiStrike:
            return actor->GetAttrByType(AntiStrikeChance) + actor->GetAttrByType(PhysicalAntiStrike) + actor->GetAttrByType(ClosePhysicalAntiStrike);
        case RemotePhysicalAntiStrike:
            return actor->GetAttrByType(AntiStrikeChance) + actor->GetAttrByType(PhysicalAntiStrike) + actor->GetAttrByType(RemotePhysicalAntiStrike);
        case IceMagicalAntiStrike:
            return actor->GetAttrByType(AntiStrikeChance) + actor->GetAttrByType(MagicalAntiStrike) + actor->GetAttrByType(IceMagicalAntiStrike);
        case FireMagicalAntiStrike:
            return actor->GetAttrByType(AntiStrikeChance) + actor->GetAttrByType(MagicalAntiStrike) + actor->GetAttrByType(FireMagicalAntiStrike);
        case ThunderMagicalAntiStrike:
            return actor->GetAttrByType(AntiStrikeChance) + actor->GetAttrByType(MagicalAntiStrike) + actor->GetAttrByType(ThunderMagicalAntiStrike);
            
            //block
        case ClosePhysicalBlock:
            return actor->GetAttrByType(BlockChance) + actor->GetAttrByType(PhysicalBlock) + actor->GetAttrByType(ClosePhysicalBlock);
        case RemotePhysicalBlock:
            return actor->GetAttrByType(BlockChance) + actor->GetAttrByType(PhysicalBlock) + actor->GetAttrByType(RemotePhysicalBlock);
        case IceMagicalBlock:
            return actor->GetAttrByType(BlockChance) + actor->GetAttrByType(MagicalBlock) + actor->GetAttrByType(IceMagicalBlock);
        case FireMagicalBlock:
            return actor->GetAttrByType(BlockChance) + actor->GetAttrByType(MagicalBlock) + actor->GetAttrByType(FireMagicalBlock);
        case ThunderMagicalBlock:
            return actor->GetAttrByType(BlockChance) + actor->GetAttrByType(MagicalBlock) + actor->GetAttrByType(ThunderMagicalBlock);
            
            //anti block
        case ClosePhysicalAntiBlock:
            return actor->GetAttrByType(AntiBlockChance) + actor->GetAttrByType(PhysicalAntiBlock) + actor->GetAttrByType(ClosePhysicalAntiBlock);
        case RemotePhysicalAntiBlock:
            return actor->GetAttrByType(AntiBlockChance) + actor->GetAttrByType(PhysicalAntiBlock) + actor->GetAttrByType(RemotePhysicalAntiBlock);
        case IceMagicalAntiBlock:
            return actor->GetAttrByType(AntiBlockChance) + actor->GetAttrByType(MagicalAntiBlock) + actor->GetAttrByType(IceMagicalAntiBlock);
        case FireMagicalAntiBlock:
            return actor->GetAttrByType(AntiBlockChance) + actor->GetAttrByType(MagicalAntiBlock) + actor->GetAttrByType(FireMagicalAntiBlock);
        case ThunderMagicalAntiBlock:
            return actor->GetAttrByType(AntiBlockChance) + actor->GetAttrByType(MagicalAntiBlock) + actor->GetAttrByType(ThunderMagicalAntiBlock);
            
        default:
            return 0;
    }
}

// Public Methods

double NativeCalc::CalculateLifeSteal(double damage, ActorPtr caster)
{
    debug("steal damage : %f , rate : %f", damage, caster->GetAttrByType(LifeSteal));
    return damage * caster->GetAttrByType(LifeSteal);
}

double NativeCalc::CalculateResilience(double damage, ActorPtr target)
{
    debug("Resilience damage : %f , rate : %f", damage, target->GetAttrByType(Resilience));
    return damage * target->GetAttrByType(Resilience);
}

double NativeCalc::CalculateHeal(ActorPtr caster, ActorPtr target, double skillFactor, double skillDamage, bool ignoreAttr)
{
    debug("NativeCalc::CalculateHeal caster[%d] target[%d] skillFactor[%f] skillDamage[%f] ignoreAttr[%d]", caster->actorId, target->actorId, skillFactor, skillDamage, ignoreAttr);
    double healNum = 0;
    
    EBattleType combatType = (EBattleType)caster->battleType;
    if (ignoreAttr)
    {
        healNum = caster->GetAttrByType(Attack)*GetAttrByTypeCalc(AttackPercentAddition, caster) * skillFactor + skillDamage;
    } 
    else
    {
        double healRatio =1 ;
        if(combatType == EBattleType::BM_Pvp)
        {
            healRatio =(1+caster->GetAttrByType(PvpHealRate))*(1+target->GetAttrByType(PvpHealedRate));
        }
        healNum = ((caster->GetAttrByType(Attack)*GetAttrByTypeCalc(AttackPercentAddition, caster) + caster->GetAttrByType(HealAddition)
            + target->GetAttrByType(OnHealAddition))*skillFactor + skillDamage)*
            (1 + caster->GetAttrByType(HealPercent))*(1 + target->GetAttrByType(OnHealPercent)) * healRatio;
    }
    
    if(healNum <= 0)
    {
        healNum = 1;
    }
    return healNum;
}

double NativeCalc::CalculateHitChance(DamageType dType, ActorPtr caster, ActorPtr target)
{
    switch (dType)
    {
        case ClosePhysical:
            return 1 + GetAttrByTypeCalc(ClosePhysicalHit, caster) - GetAttrByTypeCalc(ClosePhysicalDodge, target);
        case RemotePhysical:
            return 1 + GetAttrByTypeCalc(RemotePysicalHit, caster) - GetAttrByTypeCalc(RemotePhysicalDodge, target);
        case IceMagical:
            return 1 + GetAttrByTypeCalc(IceMagicalHit, caster) - GetAttrByTypeCalc(IceMagicalDodge, target);
        case FireMagical:
            return 1 + GetAttrByTypeCalc(FireMagicalHit, caster) - GetAttrByTypeCalc(FireMagicalDodge, target);
        case ThunderMagical:
            return 1 + GetAttrByTypeCalc(ThunderMagicalHit, caster) - GetAttrByTypeCalc(ThunderMagicalDodge, target);
        default:
            return 1;
    }
}

double NativeCalc::CalculateStrikeDamage(double damage, ActorPtr caster, ActorPtr target)
{
    double realDamage = damage;
    realDamage = damage*(max((double)1, strikeRateArg + caster->GetAttrByType(StrikeDamagePercentAddition)
                             - target->GetAttrByType(StrikeDamagePercentReduction)));
    return realDamage;
}

double NativeCalc::CalculateStrikeChance(DamageType dType, ActorPtr caster, ActorPtr target)
{
    double ret = 0;
    switch (dType)
    {
        case ClosePhysical:
            ret =  GetAttrByTypeCalc(ClosePhysicalStrike, caster) - GetAttrByTypeCalc(ClosePhysicalAntiStrike, target);
            break;
        case RemotePhysical:
            ret = GetAttrByTypeCalc(RemotePhysicalStrike, caster) - GetAttrByTypeCalc(RemotePhysicalAntiStrike, target);
            break;
        case IceMagical:
            ret = GetAttrByTypeCalc(IceMagicalStrike, caster) - GetAttrByTypeCalc(IceMagicalAntiStrike, target);
            break;
        case FireMagical:
            ret = GetAttrByTypeCalc(FireMagicalStrike, caster) - GetAttrByTypeCalc(FireMagicalAntiStrike, target);
            break;
        case ThunderMagical:
            ret = GetAttrByTypeCalc(ThunderMagicalStrike, caster) - GetAttrByTypeCalc(ThunderMagicalAntiStrike, target);
            break;
        default:
            break;
    }
    ret = max((double)0, min((double)1, ret));
    return ret;
}

double NativeCalc::CalculateBlockDamage(double damage)
{
    return damage * blockRateArg;
}

double NativeCalc::CalculateBlockChance(DamageType dType, ActorPtr caster, ActorPtr target)
{
    debug("NativeCalc::CalculateBlockChance DamageType[%d] caster[%d] target[%d]", dType, caster->actorId, target->actorId);
    double ret = 0;
    switch (dType)
    {
        case ClosePhysical:
            ret = GetAttrByTypeCalc(ClosePhysicalBlock, target) - GetAttrByTypeCalc(ClosePhysicalAntiBlock, caster);
            break;
        case RemotePhysical:
            ret = GetAttrByTypeCalc(RemotePhysicalBlock, target) - GetAttrByTypeCalc(RemotePhysicalAntiBlock, caster);
            break;
        case IceMagical:
            ret = GetAttrByTypeCalc(IceMagicalBlock, target) - GetAttrByTypeCalc(IceMagicalAntiBlock, caster);
            break;
        case FireMagical:
            ret = GetAttrByTypeCalc(FireMagicalBlock, target) - GetAttrByTypeCalc(FireMagicalAntiBlock, caster);
            break;
        case ThunderMagical:
            ret = GetAttrByTypeCalc(ThunderMagicalBlock, target) - GetAttrByTypeCalc(ThunderMagicalAntiBlock, caster);
            break;
        default:
            break;
    }
    ret = max((double)0, min((double)1, ret));
    return ret;
}

double NativeCalc::CalculateDamage(DamageType dType, ActorPtr caster, ActorPtr target,
    double skillFactor, double skillDamage, double skillFinalDamage, double doubleHitRate, EBattleType combatType, bool ignoreDefence, double debuffDmgRate)
{
    debug("calculateDamage : damageType: %d, caster :%d, target: %d, skillFactor: %f skillDamage: %f skillFinalDamage: %f \n",
          dType, caster->actorId, target->actorId, skillFactor, skillDamage, skillFinalDamage);
    double campAddition = 0;
    switch (target->GetNationality())
    {
        case EN_Nation1:
            campAddition = caster->GetAttrByType(NationDamageAddition1);
            break;
        case EN_Nation2:
            campAddition = caster->GetAttrByType(NationDamageAddition2);
            break;
        case EN_Nation3:
            campAddition = caster->GetAttrByType(NationDamageAddition3);
            break;
        default:
            break;
    }
    
    double campReduction = 0;
    switch (caster->GetNationality())
    {
        case EN_Nation1:
            campReduction = target->GetAttrByType(NationDamageReduction1);
            break;
        case EN_Nation2:
            campReduction = target->GetAttrByType(NationDamageReduction2);
            break;
        case EN_Nation3:
            campReduction = target->GetAttrByType(NationDamageReduction3);
            break;
        default:
            break;
    }
    
    switch (dType)
    {
        case ClosePhysical:
            return CalculateClosePhysical(caster, target, skillFactor, skillDamage, skillFinalDamage, campAddition, campReduction, doubleHitRate, combatType, ignoreDefence, debuffDmgRate);
        /*case RemotePhysical:
            return CalculateRemotePhysical(caster, target, skillFactor, skillDamage, skillFinalDamage, campAddition, campReduction, doubleHitIndex, combatType);*/
        /*case IceMagical:
            return CalculateIceMagical(caster, target, skillFactor, skillDamage, skillFinalDamage, campAddition, campReduction, doubleHitIndex, combatType);*/
        case FireMagical:
            return CalculateFireMagical(caster, target, skillFactor, skillDamage, skillFinalDamage, campAddition, campReduction, doubleHitRate, combatType, ignoreDefence, debuffDmgRate);
        /*case ThunderMagical:
            return CalculateThunderMagical(caster, target, skillFactor, skillDamage, skillFinalDamage, campAddition, campReduction, doubleHitIndex, combatType);*/
        default:
            return 0;
    }

}

double NativeCalc::CalculateShield(ActorAttrType attrType, ActorPtr caster, ActorPtr actor, double skillFactor, double skillValue)
{
    debug("NativeCalc::CalculateShield : ActorAttrType: [%d], caster :[%d] [%f] [%f]", attrType,actor->actorId,skillFactor,skillValue);
    EBattleType combatType = (EBattleType)actor->battleType;
    double healRatio =1 ;
    if(combatType == EBattleType::BM_Pvp)
    {
        healRatio =(1+caster->GetAttrByType(PvpHealRate))*(1+actor->GetAttrByType(PvpHealedRate));
    }
    double shieldValue = 0;
    switch (attrType) {
        case Attack:
            shieldValue = actor->GetAttrByType(Attack)*GetAttrByTypeCalc(AttackPercentAddition, actor)*skillFactor * healRatio + skillValue;
            break;
        case Defense:
            shieldValue = actor->GetAttrByType(Defense)*GetAttrByTypeCalc(DefensePercentAddition, actor)*skillFactor * healRatio + skillValue;
            break;
        case PhysicalDefense:
            shieldValue = actor->GetAttrByType(PhysicalDefense)*GetAttrByTypeCalc(DefensePercentAddition, actor)*skillFactor * healRatio + skillValue;
            break;
        case MagicalDefense:
            shieldValue = actor->GetAttrByType(MagicalDefense)*GetAttrByTypeCalc(DefensePercentAddition, actor)*skillFactor * healRatio + skillValue;
            break;
        case HpMax:
        {
            shieldValue = actor->actorDatas.hpMax *skillFactor * healRatio + skillValue;
            break;
        }
        default:
            shieldValue = (1+actor->GetAttrByType(attrType))*skillFactor * healRatio + skillValue;
            break;
    }
    return shieldValue;
}

double NativeCalc::CalculateDamageXpRecovery(double damage, ActorPtr actor)
{
    double hpMax = actor->actorDatas.hpMax;
    double injuredRecoveryRate = actor->GetAttrByType(InjuredXpRecoveryRate);
    double xpMax = actor->actorDatas.xpMax;
    double xpGainRate = actor->GetAttrByType(XpGainRate);
    return damage / hpMax * injuredRecoveryRate * xpMax * xpGainRate;
}

double NativeCalc::CalculateHitXpRecovery(double hitToRecory, ActorPtr actor)
{
    return actor->GetAttrByType(HitXpRecovery) * actor->GetAttrByType(XpGainRate) * hitToRecory;
}

double NativeCalc::CalculateHealXpRecovery(double hitToRecory, ActorPtr actor)
{
    return actor->GetAttrByType(HealXpRecovery) * actor->GetAttrByType(XpGainRate) * hitToRecory;
}

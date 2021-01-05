#include "SubEffectPerformer.h"
#include "EffectUtil.h"
#include "ActorModifier.h"
#include "Actor.h"
#include "BattleField.h"
#include "SubEffectsMeta.h"
#include "EffectMeta.h"
#include "ModifierUtils.h"
#include "BattleArea.h"
#include "Random.h"
#include "Enum.h"
#include "Log.h"
#include "Calculator.h"
#include "AnalysisUtil.h"
#include "EventData.h"
#include "NativeCalc.h"
#include "BattleReplay.pb.h"
#include "TriggerMeta.h"
#include "TriggerManager.h"
// damagesub

void DamageSubPerformer::MakeData(const SubEffectsMeta *meta)
{
    TRACE("DamageSubPerformer::MakeData metaId[%d]", meta->id);
    SubEffectPerformer::MakeData(meta);
    dType = (DamageType)(int)meta->funcParams[0];
    skillFactor = meta->funcParams[1];
    skillGrowFactor = meta->funcParams[2];
    skillAddition = meta->funcParams[3];
    skillGrowAddition = meta->funcParams[4];
    skillFinal = meta->funcParams[5];
    skillGrowFinal = meta->funcParams[6];
    affixId = meta->funcParams[7];
    affixPowerGrow = meta->funcParams[8];
    affixPower = meta->funcParams[9];
    bloodDrinking = (meta->funcParams[10] == 1) ? true : false;
    killHpPer = meta->funcParams[11];
    killHpParam = meta->funcParams[12];

    ignoreBlock = (meta->funcParams[13] == 1) ? true : false;
    ignoreStrike = (meta->funcParams[14] == 1) ? true : false;
    ignoreDefence = (meta->funcParams[15] == 1) ? true : false;
    ignoreResilience = (meta->funcParams[16] == 1) ? true : false;
    hitToXp = meta->funcParams[17];
    deBuffGroupID = meta->funcParams[21];
    deBuffDamageRate = meta->funcParams[22]/10000;
    doRage = (meta->DoRage == 1) ? true : false;
}
void AppendAffixBySkillType(ActorPtr caster, BuffAffixApply affixType, int skillLevel, TempAttrInfo &tempAttr)
{
    caster->actorMod->AppendTempAffix(affixType, skillLevel, tempAttr);
}
void AccquireAffixPowerInfo(ActorPtr caster, BuffAffixApply affixType, int skillLevel, TempAffixPowerInfo &tempInfo)
{
    caster->actorMod->AccquireAffixPowerInfo(affixType, skillLevel, tempInfo);
}


void DamageSubPerformer::Execute(ActorPtr caster, ActorPtr target, int level, double param)
{
    //calculate damage
    TRACE("DamageSubPerformer::Execute Caster[%d] Target[%d] Level[%d] Param", caster->actorId,target->actorId,level,param);
    skilllevel = level;
    auto doEvent = caster->battleField->random.RandForWidth(100);

    if (doEvent < metaRef->DoEvent)
    {
        TriggerParam param1;
        param1.evtType = EEventType::ComingCauseDamage;
        param1.c1.list[0] = caster;
        param1.c2.list[0] = target;
        param1.affixPerformer = this;
        caster->battleField->FireEvent(param1);

        TriggerParam param2;
        param2.evtType = EEventType::ComingSufferDamage;
        param2.c1.list[0] = target;
        param2.c2.list[0] = caster;
        param2.affixPerformer = this;
        caster->battleField->FireEvent(param2);
    }

    
    double factor = skillFactor + skillGrowFactor*(level-1);
    factor = factor * (1 + param);
    if (killHpPer != 0)
    {
        if (target->actorDatas.GetHpPercent() <= killHpPer)
        {
            factor = factor * (1 + killHpParam);
        }
    }
    bool originCasterAlive = caster->IsAlive();
    double addition = skillAddition + skillGrowAddition*(level-1);
    double final = skillFinal + skillGrowFinal*(level-1);
    int debuffCnt = target->actorMod->GetModCntByGroupID(deBuffGroupID);
    float debuffDmgRate = debuffCnt * deBuffDamageRate;

    float affixPowerNew = affixPower + affixPowerGrow * (level - 1);
    TempAttrInfo casterTempAttr;
    TempAffixPowerInfo affixPowerInfo;
    ApplyAffix(caster, affixId, affixPowerNew, casterTempAttr);
    if (skillType == CommonSkillType || skillType == LegendSkillType)
    {
        AppendAffixBySkillType(caster, BuffAffixApply::CauseAtk, level, casterTempAttr);
        AccquireAffixPowerInfo(target, BuffAffixApply::SufferAtk, level, affixPowerInfo);
        for (auto & info : affixPowerInfo)
        {
            ApplyAffix(caster, info.first, info.second, casterTempAttr);
        }
    }
    else if (skillType == XpSkillType || skillType == CombineSkillType)
    {
        AppendAffixBySkillType(caster, BuffAffixApply::CauseSkill, level, casterTempAttr);
        AccquireAffixPowerInfo(target, BuffAffixApply::SufferSkill, level, affixPowerInfo);
        for (auto & info : affixPowerInfo)
        {
            ApplyAffix(caster, info.first, info.second, casterTempAttr);
        }
    }
    double damage = CalculateDamage(dType, caster, target, factor, addition, final, caster->actorDatas.doubleHitRate, caster->battleField->CurBattleMode->BattleType, ignoreDefence, debuffDmgRate);

//    RemoveAffix(caster, casterTempAttr); //move to below
    
    TRACE("damage sub performer casterid : %d, targetid : %d, damage : %f\n", caster->actorId, target->actorId, damage);
    double rate = 0;
    EffectType effectType = EffectType::Default;
    double strikeRate = 0;
    if (!ignoreStrike)
    {
        strikeRate = CalculateStrikeChance(dType, caster, target) * 100;
    }
    double blockRate = 0;
    if (!ignoreBlock)
    {
        blockRate = CalculateBlockChance(dType, caster, target) * 100;
    }

    if ((strikeRate + blockRate) <= 100)
    {
        rate = caster->battleField->random.RandForWidth(100);
    }
    else
    {
        rate = caster->battleField->random.RandForWidth(strikeRate + blockRate);
    }
    
    if (rate <= strikeRate)
    {
        effectType = Strike;
        damage = CalculateStrikeDamage(damage, caster, target);
    }
    else if (strikeRate < rate && rate <= blockRate + strikeRate)
    {
        effectType = Block;
        damage = CalculateBlockDamage(damage);
    }
    double totalDamage = damage;
    

    if (doEvent < metaRef->DoEvent)
    {
        TriggerParam param1;
        param1.evtType = EEventType::CauseDamage;
        param1.c1.list[0] = caster;
        param1.c2.list[0] = target;
        param1.r2 = (int)effectType;
        param1.r4 = totalDamage;
        caster->battleField->FireEvent(param1);

        TriggerParam param2;
        param2.evtType = EEventType::SufferDamage;
        param2.c1.list[0] = target;
        param2.c2.list[0] = caster;
        param2.r2 = param1.r2;
        param2.r4 = param1.r4;
        caster->battleField->FireEvent(param2);
        effectType = (EffectType)param2.r2;
        totalDamage = param2.r4;
    }

    if (totalDamage <= 1)
    {
        totalDamage = 1;
    }
    

    ShieldInfo shieldInfo[MAX_SHIELD_INFO] = { 0 };
    double realDamage = target->actorMod->TriggerShieldDamage(caster, totalDamage, metaRef->id, shieldInfo);
    auto fillParam = FillDamageParam(caster, target, effectType, metaRef->id, -realDamage);
    fillParam.buffId = originBuffId;
    fillParam.pShieldInfo = shieldInfo;
    double resilience = 0;
    double bloodDrinkValue = 0;

    if (!ignoreResilience)
    {
        resilience = (int)CalculateResilience(realDamage, target);
        if (resilience > 0)
        {
            double newHp = caster->actorDatas.hp - resilience;
            if (newHp <= 0)
            {
                if (caster->actorDatas.hp > 1)
                {
                    resilience = 1 - caster->actorDatas.hp;
                }
                else
                {
                    resilience = 0;
                }
            }
            else
            {
                resilience = -resilience;
            }
            if (resilience < 0)
            {
                caster->ChangeHp(resilience);
                fillParam.resilience = resilience;
            }
        }
    }

    

    if (caster->IsAlive())
    {

        if (bloodDrinking)
        {
            bloodDrinkValue = CalculateLifeSteal(realDamage, caster);
            if (bloodDrinkValue >= 1)
            {
                caster->ChangeHp(bloodDrinkValue);
                fillParam.bloodDrink = bloodDrinkValue;
            }
        }
        if (effectType == Block)
        {
            double hpTherapy = caster->GetAttrByType(therapyCauseBlock) * totalDamage;
            if (hpTherapy >= 1)
            {
                caster->ChangeHp(hpTherapy);
                fillParam.blockTherapy = hpTherapy;
            }
        }
        else if (effectType == Strike)
        {
            double hpTherapy = caster->GetAttrByType(therapyCauseStrike) * totalDamage;
            if (hpTherapy >= 1)
            {
                caster->ChangeHp(hpTherapy);
                fillParam.strikeTherapy = hpTherapy;
            }
        }
    }

    //debug("executeSubEffect damage id: %d type: %d damage : %f\n", metaRef->id, effectType, damage);

    //error("target curr hp %f damage %f",target->actorDatas.hp,realDamage);
    double hpOld = target->actorDatas.hp;
    bool result = false;
    if(hpOld <= realDamage){
        TriggerParam param1;
        param1.evtType = EEventType::NearDeath;
        param1.c1.list[0] = target;
        param1.c2.list[0] = caster;
        param1.r2 = (int)effectType;
        param1.r5 = traceData.subEffectId;
        param1.r6 = traceData.effectId;
        param1.r7 = traceData.skillId;
        param1.r8 = traceData.buffId;
        result = caster->battleField->PreFireEvent(param1);
        if(result){
            fillParam.damage = -(hpOld-1);//damage = hp-1
            error("real hit %d",fillParam.damage);
        }
    }
    target->ChangeHp(fillParam.damage);
    
    //if trigger NearDeath  the target will ReAlive
    if (target->IsAlive())
    {
        double hpTherapy = target->GetAttrByType(therapySufferDamage) * totalDamage;
        if (hpTherapy >= 1)
        {
            target->ChangeHp(hpTherapy);
            fillParam.s_damageTherapy = hpTherapy;
        }
        if (effectType == Block)
        {
            double hpTherapy = target->GetAttrByType(therapySufferBlock) * totalDamage;
            if (hpTherapy >= 1)
            {
                target->ChangeHp(hpTherapy);
                fillParam.s_blockTherapy = hpTherapy;
            }
        }
        else if (effectType == Strike)
        {
            double hpTherapy = target->GetAttrByType(therapySufferStrike) * totalDamage;
            if (hpTherapy >= 1)
            {
                target->ChangeHp(hpTherapy);
                fillParam.s_strikeTherapy = hpTherapy;
            }
        }
    }
    
    FillDamageCmd(fillParam);

    if(result){
        TriggerParam param1;
        param1.evtType = EEventType::NearDeath;
        param1.c1.list[0] = target;
        param1.c2.list[0] = caster;
        param1.r2 = (int)effectType;
        param1.r5 = traceData.subEffectId;
        param1.r6 = traceData.effectId;
        param1.r7 = traceData.skillId;
        param1.r8 = traceData.buffId;
        caster->battleField->FireEvent(param1);
    }

    int furyRecovery = (int)CalculateDamageXpRecovery(totalDamage, target);
    if (furyRecovery > 0)
    {
        if (target->ChangeXp(furyRecovery))
        {
            auto param = FillXpChangeParam(caster, target, metaRef->id, furyRecovery);
            param.originBuffID = originBuffId;
            param.recovery = XpRecoveryType::Damage;
            FillXpChangeCmd(param);
        }
    }
    

    if (doRage)
    {
        int hitXpRecovery = (int)CalculateHitXpRecovery(hitToXp, target);
        if (hitXpRecovery > 0)
        {
            if (target->ChangeXp(hitXpRecovery))
            {
                auto param = FillXpChangeParam(caster, target, metaRef->id, hitXpRecovery);
                param.originBuffID = originBuffId;
                param.recovery = XpRecoveryType::Hit;
                FillXpChangeCmd(param);
            }
        }
    }

    CheckDeadEvt(caster,target,effectType);


    BattleAreaPtr castArea = caster->battleField->GetOwnArea(caster->areaIndex);
    castArea->damageAll += totalDamage;

    
    RemoveAffix(caster, casterTempAttr);
    ResetExtraAffix();
}


void HealSubPerformer::MakeData(const SubEffectsMeta *meta)
{
    TRACE("HealSubPerformer::MakeData meta[%d]", meta->id);
    SubEffectPerformer::MakeData(meta);
    skillFactor = meta->funcParams[0];
    skillGrowFactor = meta->funcParams[1];
    skillAddition = meta->funcParams[2];
    skillGrowAddition = meta->funcParams[3];
    strikeRate = meta->funcParams[4];
    healFactor = meta->funcParams[5];
    ignoreAdditionAttr = (meta->funcParams[6] != 0) ? true : false;
    HitToXp = meta->funcParams[7];
}

void HealSubPerformer::Execute(ActorPtr caster, ActorPtr target, int level, double param)
{
    TRACE("HealSubPerformer::Execute caster[%d] target[%d] param[%d]", caster->actorId, target->actorId, param);
    skilllevel = level;
    auto doEvent = caster->battleField->random.RandForWidth(100);

    if (doEvent < metaRef->DoEvent)
    {
        TriggerParam param1;
        param1.evtType = EEventType::ComingCauseHeal;
        param1.c1.list[0] = caster;
        param1.c2.list[0] = target;
        param1.affixPerformer = this;
        caster->battleField->FireEvent(param1);
        TriggerParam param2;
        param2.evtType = EEventType::ComingSufferHeal;
        param2.c1.list[0] = target;
        param2.c2.list[0] = caster;
        param2.affixPerformer = this;
        caster->battleField->FireEvent(param2);
    }

    double factor = skillFactor + skillGrowFactor*(level-1);
    double addition = skillAddition + skillGrowAddition*(level-1);
    double tarHealFactor = healFactor * (1- target->actorDatas.GetHpPercent());
    double healNum = CalculateHeal(caster, target, factor * (1 + tarHealFactor), addition,ignoreAdditionAttr);
    double rate = caster->battleField->random.RandForWidth(100);
    EffectType effectType = EffectType::Default;
    auto newStrikeRate = caster->GetAttrByType(HealStrikeRate) + strikeRate;
    if (rate < newStrikeRate * 100)
    {
        effectType = Strike;
        //strike read in value
        healNum = healNum * NativeCalc::instance()->healStrikeRateArg;
    }
    if (healNum < 0)
    {
        healNum = 0;
    }

    double recovery = healNum;
    if (doEvent < metaRef->DoEvent)
    {
        TriggerParam param1;
        param1.evtType = EEventType::CauseHeal;
        param1.c1.list[0] = caster;
        param1.c2.list[0] = target;
        caster->battleField->FireEvent(param1);
        TriggerParam param2;
        param2.evtType = EEventType::SufferHeal;
        param2.c1.list[0] = target;
        param2.c2.list[0] = caster;
        caster->battleField->FireEvent(param2);
    }
    

    if (recovery < 0)
    {
        double newHp = target->actorDatas.hp + recovery;
        if (newHp <= 0)
        {
            if (target->actorDatas.hp > 1)
            {
                recovery = 1 - target->actorDatas.hp;
            }
            else
            {
                recovery = 0;
            }
        }
    }

    target->ChangeHp(recovery);

    int healXpRecovery = (int)CalculateHealXpRecovery(HitToXp, target);
    if (healXpRecovery > 0)
    {
        if (target->ChangeXp(healXpRecovery))
        {
            auto param = FillXpChangeParam(caster, target, metaRef->id, healXpRecovery);
            param.originBuffID = originBuffId;
            param.recovery = XpRecoveryType::Heal;
            FillXpChangeCmd(param);
        }
    }
    auto remedyParam = FillRecoveryParam(caster, target, effectType, metaRef->id,recovery);
    remedyParam.originBuffID = originBuffId;
    FillRecoveryCmd(remedyParam);
    ResetExtraAffix();
}


void ChangeXpPerformer::MakeData(const SubEffectsMeta *meta)
{
    SubEffectPerformer::MakeData(meta);
}


void ChangeXpPerformer::Execute(ActorPtr caster, ActorPtr target, int level, double param)
{
    skilllevel = level;
    TRACE("ChangeXpPerformer::Execute");
    int xpBase = metaRef->funcParams[0];
    int xpGrow = metaRef->funcParams[1];
    auto realLevel = level;
    if (realLevel >= 1)
    {
        realLevel--;
    }
    int xpChange = xpBase + xpGrow * realLevel;
    if (target->ChangeXp(xpChange))
    {
        auto param = FillXpChangeParam(caster, target, metaRef->id, xpChange);
        param.originBuffID = originBuffId;
        param.recovery = XpRecoveryType::Normal;
        param.isShow = true;
        FillXpChangeCmd(param);
    }
}


void DispelPerformer::MakeData(const SubEffectsMeta *meta)
{
    TRACE("DispelPerformer::MakeData");
    SubEffectPerformer::MakeData(meta);
    buffGroupId = meta->funcParams[0];
    layerCount = meta->funcParams[1];
}

void DispelPerformer::Execute(ActorPtr caster, ActorPtr target, int level, double param)
{
    TRACE("DispelPerformer::Execute");
    skilllevel = level;
    if(buffGroupId == 998)
    {
        target->actorMod->RemoveModByType(caster, bigBuff, metaRef->id);
    }
    else if (buffGroupId == 999)
    {
        target->actorMod->RemoveModByType(caster, bigDebuff, metaRef->id);
    }
    else
    {
        target->actorMod->RemoveModByDispel(caster, buffGroupId, layerCount,metaRef->id);
    }
}
//
////resilience
//void ResiliencePerformer::MakeData(const SubEffectsMeta *meta)
//{
//    debug("ResiliencePerformer::MakeData");
//    SubEffectPerformer::MakeData(meta);
//    damageFactor = meta->funcParams[0];
//}
//
//void ResiliencePerformer::Execute(ActorPtr caster, ActorPtr target, int level, double param)
//{
//    double realDamage = param * damageFactor;
//    debug("resilience execute id: %d,  damage: %d, realDamage: %d\n", metaRef->id, param, realDamage);
//    makeUpdateHpCmd(caster, target, NoneType, realDamage, metaRef->id, 0,0);
//}
//
//heal for hpmax
void HealForHpMaxPerformer::MakeData(const SubEffectsMeta *meta)
{
    SubEffectPerformer::MakeData(meta);
    hpFactorBase = meta->funcParams[0];
    hpFactorGrow = meta->funcParams[1];
}
//
void HealForHpMaxPerformer::Execute(ActorPtr caster, ActorPtr target, int level, double param)
{
    auto realLevel = level;
    if (realLevel >= 1)
    {
        realLevel--;
    }
    EBattleType combatType = (EBattleType)caster->battleType;
    double ratio = hpFactorBase + hpFactorGrow * realLevel;
    double healNum = (ratio *1.0f / 100) * target->actorDatas.hpMax;
    if (hpFactorBase >= 0 && hpFactorGrow >= 0)
    {
        double healRatio =1 ;
        if(combatType == EBattleType::BM_Pvp)
        {
            healRatio =(1+caster->GetAttrByType(PvpHealRate))*(1+target->GetAttrByType(PvpHealedRate));
        }
        healNum = healNum * (1 + caster->GetAttrByType(HealPercent))*(1 + target->GetAttrByType(OnHealPercent)) * healRatio;
    }
    EffectType effectType = EffectType::Default;
    double recovery = healNum;
    if (recovery < 0)
    {
        double newHp = target->actorDatas.hp + recovery;
        if (newHp <= 0)
        {
            if (target->actorDatas.hp > 1)
            {
                recovery = 1 - target->actorDatas.hp;
            }
            else
            {
                recovery = 0;
            }
        }
    }
    target->ChangeHp(recovery);
    auto remedyParam = FillRecoveryParam(caster, target, effectType, metaRef->id, recovery);
    remedyParam.originBuffID = originBuffId;
    FillRecoveryCmd(remedyParam);
}
//
////relife and heal
//void RelifeAndHealPerformer::MakeData(const SubEffectsMeta *meta)
//{
//    SubEffectPerformer::MakeData(meta);
//    hpFactor = meta->funcParams[0];
//}
//
//void RelifeAndHealPerformer::Execute(ActorPtr caster, ActorPtr target, int level, double param)
//{
//    double healNum = hpFactor * target->actorDatas.hpMax;
//    ShowEffectType sType = ShowRelife;
//    info("RelifeAndHealPerformer::Execute id: %d targetId: %d healNum: %f\n", metaRef->id, target->actorId, healNum);
//    makeUpdateHpCmd(caster, target, sType, healNum, metaRef->id, 0,0);
//}
//
////immune and heal
//void ImmuneAndHealPerformer::MakeData(const SubEffectsMeta *meta)
//{
//    SubEffectPerformer::MakeData(meta);
//    hpFactor = meta->funcParams[0];
//}
//
//void ImmuneAndHealPerformer::Execute(ActorPtr caster, ActorPtr target, int level, double param)
//{
//    target->IsImmuneThisTime = true;
//    double healNum = hpFactor * target->actorDatas.hpMax;
//    ShowEffectType sType = NoneType;
//    info("ImmuneAndHealPerformer::Execute id: %d targetId: %d healNum: %f\n", metaRef->id, target->actorId, healNum);
//    makeUpdateHpCmd(caster, target, sType, healNum, metaRef->id, 0,0);
//}

void SubEffectPerformer::MakeData(const SubEffectsMeta* meta)
{
    metaRef = meta;
}

void TempoExtraAffixPerformer::ApplyCasterAffix(Actor *pActor,int affixID, int affixBase, int affixGrow)
{
    if (caster != nullptr && caster != pActor)
    {
        error("TempoExtraAffixPerformer::ApplyCasterAffix");
        return;
    }
    caster = pActor;
    int realLevel = skilllevel;
    if (realLevel >= 1)
    {
        realLevel--;
    }
    int affixPower = affixBase + affixGrow * realLevel;
    ApplyAffix(caster, affixID, affixPower, casterAttr);
}

void TempoExtraAffixPerformer::ApplyTargerAffix(Actor *pActor,int affixID, int affixBase, int affixGrow)
{
    if (target != nullptr && target != pActor)
    {
        error("TempoExtraAffixPerformer::ApplyTargerAffix");
        return;
    }
    target = pActor;
    int realLevel = skilllevel;
    if (realLevel >= 1)
    {
        realLevel--;
    }
    int affixPower = affixBase + affixGrow * realLevel;
    ApplyAffix(target, affixID, affixPower, targetAttr);
}

void TempoExtraAffixPerformer::ResetExtraAffix()
{
    if (caster != nullptr)
    {
        RemoveAffix(caster, casterAttr);
        casterAttr.clear();
        caster = nullptr;
    }
    if (target != nullptr)
    {
        RemoveAffix(target, targetAttr);
        targetAttr.clear();
        target = nullptr;
    }
}

void HealSetPerformer::MakeData(const SubEffectsMeta *meta)
{
    SubEffectPerformer::MakeData(meta);
    hpSetType = meta->funcParams[0];
    hpValue = meta->funcParams[1];
    flyworldType = meta->funcParams[2];
}
//
void HealSetPerformer::Execute(ActorPtr caster, ActorPtr target, int level, double param)
{
    
    double hpChange = 0;
    if(hpSetType == 0){
        hpChange = hpValue - target->actorDatas.hp;
    }else{
        hpChange = target->actorDatas.hpMax * hpValue - target->actorDatas.hp;
    }
    if(hpChange<=0){
        hpChange = 1;
    }
    target->ChangeHp(hpChange);
    EffectType effectType = EffectType::Default;
    auto remedyParam = FillRecoveryParam(caster, target, effectType, metaRef->id, hpChange);
    remedyParam.originBuffID = originBuffId;
    remedyParam.recovery = hpChange;
    remedyParam.isShow = flyworldType == 0;
    FillRecoveryCmd(remedyParam);
}

//
//  SubEffectPerformer.h
//  grav_test
//
//  Created by kingroy on 16/10/21.
//  Copyright © 2016年 Babeltime. All rights reserved.
//

#ifndef _GRAV_SUB_EFFECT_PERFORMER_H_
#define _GRAV_SUB_EFFECT_PERFORMER_H_

#include "Actor.h"
#include "Enum.h"
#include "Type.h"
#include "EffectMeta.h"
#include "SubEffectsMeta.h"

class SubEffectPerformer
{
public:
    TriggerTrace traceData;
    int skilllevel;
    int originBuffId;
    const SubEffectsMeta* metaRef;
    void MakeTrace(TriggerTrace td){
        traceData.subEffectId = td.subEffectId;
        traceData.effectId = td.effectId;
        traceData.skillId = td.skillId;
        traceData.buffId = td.buffId;
    }
    virtual void MakeData(const SubEffectsMeta* meta);
    virtual void Execute(ActorPtr caster, ActorPtr target, int level, double param) = 0;
    SubEffectPerformer() :skilllevel(0), metaRef(nullptr), originBuffId(0){}
};

class TempoExtraAffixPerformer : public SubEffectPerformer
{
public:
    ActorPtr caster;
    ActorPtr target;
    TempAttrInfo casterAttr;
    TempAttrInfo targetAttr;
    TempoExtraAffixPerformer() :caster(nullptr), target(nullptr){}
    void ApplyCasterAffix(Actor* pActor,int affixID, int affixBase, int affixGrow);
    void ApplyTargerAffix(Actor* pActor,int affixID, int affixBase, int affixGrow);
    void ResetExtraAffix();
};

class DamageSubPerformer : public TempoExtraAffixPerformer
{
public:
    DamageType dType;
    double skillFactor;
    double skillGrowFactor;
    double skillAddition;
    double skillGrowAddition;
    double skillFinal;
    double skillGrowFinal;
    int affixId;
    int affixPower;
    int affixPowerGrow;
    bool bloodDrinking;
    double killHpPer;
    double killHpParam;
    bool ignoreBlock;
    bool ignoreStrike;
    bool ignoreDefence;
    bool ignoreResilience;
    double hitToXp;
    bool doRage;
    int deBuffGroupID;
    float deBuffDamageRate;
    SkillType skillType;
    void MakeData(const SubEffectsMeta* meta) override;
    void Execute(ActorPtr caster, ActorPtr target, int level, double param) override;
};

class HealSubPerformer : public TempoExtraAffixPerformer
{
public:
    double HitToXp;
    double skillFactor;
    double skillGrowFactor;
    double skillAddition;
    double skillGrowAddition;
    double strikeRate;
    double healFactor;
    bool ignoreAdditionAttr;
    void MakeData(const SubEffectsMeta* meta) override;
    void Execute(ActorPtr caster, ActorPtr target, int level, double param) override;
};

class ChangeXpPerformer : public SubEffectPerformer
{
public:
    void MakeData(const SubEffectsMeta* meta) override;
    void Execute(ActorPtr caster, ActorPtr target, int level, double param) override;
};

class DispelPerformer : public SubEffectPerformer
{
public:
    int buffGroupId;
    int layerCount;
    void MakeData(const SubEffectsMeta* meta) override;
    void Execute(ActorPtr caster, ActorPtr target, int level, double param) override;
};
//
//class ResiliencePerformer : public SubEffectPerformer
//{
//public:
//    int damageFactor;
//    void MakeData(const SubEffectsMeta* meta) override;
//    void Execute(ActorPtr caster, ActorPtr target, int level, double param) override;
//};
//
class HealForHpMaxPerformer : public SubEffectPerformer
{
public:
    double hpFactorBase;
    double hpFactorGrow;
    void MakeData(const SubEffectsMeta* meta) override;
    void Execute(ActorPtr caster, ActorPtr target, int level, double param) override;
};
//
//class RelifeAndHealPerformer : public SubEffectPerformer
//{
//public:
//    double hpFactor;
//    void MakeData(const SubEffectsMeta* meta) override;
//    void Execute(ActorPtr caster, ActorPtr target, int level, double param) override;
//};
//
//class ImmuneAndHealPerformer : public SubEffectPerformer
//{
//public:
//    double hpFactor;
//    void MakeData(const SubEffectsMeta* meta) override;
//    void Execute(ActorPtr caster, ActorPtr target, int level, double param) override;
//};


class HealSetPerformer : public SubEffectPerformer
{
public:
    int hpSetType;
    double hpValue;
    int flyworldType;
    void MakeData(const SubEffectsMeta* meta) override;
    void Execute(ActorPtr caster, ActorPtr target, int level, double param) override;
};

#endif

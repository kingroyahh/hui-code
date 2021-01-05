#ifndef _GRAV_EFFECT_META_H_
#define _GRAV_EFFECT_META_H_

#ifdef GRAV_UNITY_PLUGIN
#define EFFECT_KEY   "Effects/trunk.db"
#else
#define EFFECT_KEY   "effects.json"
#endif

#include "Enum.h"
#include "Type.h"
#include "MetaSet.h"
#include "Singleton.h"

#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

struct EffectMeta
{
    int id;
    TargetType TargetType;
    TargetFilter TargetFilter;
    int TargetFilter1[MAX_TARGET_FILTER_CNT];
    int TargetFilter2[MAX_TARGET_FILTER_CNT];
    int TargetFilter3[MAX_TARGET_FILTER_CNT];
    int TargetParam;
    float TargetDmgParam[MAX_DMG_TARGET_COUNT];
    int DoEffect;
    int NoDodge;
    // one sub
    SubTargetType SubTarget;
    
    int SubEffectsPara[MAX_DMG_PARAM];
    
    int BuffPara[MAX_DMG_PARAM];
    int BuffDuration[MAX_DMG_PARAM];
    int BuffChance[MAX_DMG_PARAM];
    
    int SpEffects[MAX_DMG_PARAM];
    int SpChance[MAX_DMG_PARAM];
    
    // two sub
    SubTargetType SubTarget2;
    
    int SubEffectsPara2[MAX_DMG_PARAM];
    
    int BuffPara2[MAX_DMG_PARAM];
    int BuffChance2[MAX_DMG_PARAM];
    
    int SpEffects2[MAX_DMG_PARAM];
    int SpChance2[MAX_DMG_PARAM];
    
    int TriggerSeq[MAX_DMG_PARAM];
    int TriggerSeq2[MAX_DMG_PARAM];
};

class EffectMetaSet : public BaseMetaSet<EffectMeta>, public Singleton<EffectMetaSet>
{
protected:
    void AssignPath() override;
    EffectMeta* ParseJObject(Value::Object &jobj) override;
};

#endif

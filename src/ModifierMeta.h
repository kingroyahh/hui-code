#ifndef _GRAV_MODIFIER_META_H_
#define _GRAV_MODIFIER_META_H_

#include "Enum.h"
#include "MetaSet.h"
#include "Type.h"
#include "Singleton.h"

#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

struct ModifierMeta
{
    int id;
    int Affix;
    float AffixPower[MAX_DMG_PARAM];
    int AffixType;
    int DoBuff;
    int Trigger;
    LastEffectType LastEffect;
    float LastParam[MAX_DMG_PARAM];
    
    PeriodEffectType PeriodEffect;
    float PeriodParam[MAX_DMG_PARAM];
    PeriodType PeriodType;
    int PeriodChance;
    int PeriodCondType1;
    float PeriodCondParam1[MAX_DMG_PARAM];
    int PeriodCondType2;
    float PeriodCondParam2[MAX_DMG_PARAM];
    int PeriodCondType3;
    float PeriodCondParam3[MAX_DMG_PARAM];
    int PeriodEffectCaster;

   /* TriggerEffectType triggerEffect;
    float effectsParam[MAX_DMG_PARAM];
    EventType event;
    float eventParam[MAX_DMG_PARAM];
    int triggerChance;
    ModConditionType condition;
    int conParam;
    BattleStateType effectiveTime;
    int triggerTimes;
    */
    BuffType BuffType;
    BuffDuraType DuraType;
    int LastRound;
    int GroupId[MAX_BUFF_GROUP_CNT];
    int MaxLayer;
    int GroupPriority;
    int BeLegend;
    int PeriodSnap;
    int SnapAttribute[MAX_BUFF_CACHE_ATTR_COUNT];
};

class ModifierMetaSet : public BaseMetaSet<ModifierMeta>, public Singleton<ModifierMetaSet>
{
protected:
    void AssignPath() override;
    ModifierMeta* ParseJObject(Value::Object &jobj) override;
};

#endif

#include "ModifierMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void ModifierMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_BUFFS);
#endif
}

ModifierMeta* ModifierMetaSet::ParseJObject(Value::Object &jobj)
{
    ModifierMeta* meta = new ModifierMeta();

    //init
    memset(meta, 0, sizeof(ModifierMeta));
    
    Value::ConstMemberIterator it;
    
    it = jobj.FindMember("id");
    if (it == jobj.MemberEnd())
    {
        error("EffectMeta ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();
    
// affix
    it = jobj.FindMember("Affix");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find Affix\n");
        delete meta;
        return NULL_PTR;
    }
    meta->Affix = it->value.GetInt();

    it = jobj.FindMember("AffixType");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find AffixType\n");
        delete meta;
        return NULL_PTR;
    }
    meta->AffixType = (BuffAffixApply)it->value.GetInt();

    it = jobj.FindMember("AffixPower");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find AffixPower\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->AffixPower[i] = it->value[i].GetFloat();
    }
    
    it = jobj.FindMember("Trigger");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find Trigger\n");
        delete meta;
        return NULL_PTR;
    }
    meta->Trigger = it->value.GetInt();
    
    it = jobj.FindMember("DoBuff");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find DoBuff\n");
        delete meta;
        return NULL_PTR;
    }
    meta->DoBuff = it->value.GetInt();

// last effect
    it = jobj.FindMember("LastEffect");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find LastEffect\n");
        delete meta;
        return NULL_PTR;
    }
    meta->LastEffect = (LastEffectType)it->value.GetInt();
    
    it = jobj.FindMember("LastParam");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find LastParam\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->LastParam[i] = it->value[i].GetFloat();
    }
    
// period effect
    it = jobj.FindMember("PeriodEffect");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodEffect\n");
        delete meta;
        return NULL_PTR;
    }
    meta->PeriodEffect = (PeriodEffectType)it->value.GetInt();
    
    it = jobj.FindMember("PeriodParam");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodParam\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->PeriodParam[i] = it->value[i].GetFloat();
    }
    
    it = jobj.FindMember("PeriodType");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodEffect\n");
        delete meta;
        return NULL_PTR;
    }
    meta->PeriodType = (PeriodType)it->value.GetInt();
    
    it = jobj.FindMember("PeriodChance");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodChance\n");
        delete meta;
        return NULL_PTR;
    }
    meta->PeriodChance = it->value.GetInt();

    it = jobj.FindMember("PeriodEffectCaster");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodEffectCaster\n");
        delete meta;
        return NULL_PTR;
    }
    meta->PeriodEffectCaster = it->value.GetInt();

    it = jobj.FindMember("PeriodCondType1");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodCondType1\n");
        delete meta;
        return NULL_PTR;
    }
    meta->PeriodCondType1 = it->value.GetInt();
    
    it = jobj.FindMember("PeriodCondParam1");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodCondParam1\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->PeriodCondParam1[i] = it->value[i].GetFloat();
    }

    it = jobj.FindMember("PeriodCondType2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodCondType2\n");
        delete meta;
        return NULL_PTR;
    }
    meta->PeriodCondType2 = it->value.GetInt();

    it = jobj.FindMember("PeriodCondParam2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodCondParam2\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->PeriodCondParam2[i] = it->value[i].GetFloat();
    }

    it = jobj.FindMember("PeriodCondType3");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodCondType3\n");
        delete meta;
        return NULL_PTR;
    }
    meta->PeriodCondType3 = it->value.GetInt();

    it = jobj.FindMember("PeriodCondParam3");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodCondParam3\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->PeriodCondParam3[i] = it->value[i].GetFloat();
    }
//
//// trigger effect
//    it = jobj.FindMember("TriggerEffect");
//    if (it == jobj.MemberEnd())
//    {
//        error("ParseJObject failed, could not find TriggerEffect\n");
//        delete meta;
//        return NULL_PTR;
//    }
//    meta->triggerEffect = (TriggerEffectType)it->value.GetInt();
//    
//    it = jobj.FindMember("EffectsParam");
//    if (it == jobj.MemberEnd())
//    {
//        error("ParseJObject failed, could not find EffectsParam\n");
//        delete meta;
//        return NULL_PTR;
//    }
//    for (int i = 0; i < it->value.Size(); ++i)
//    {
//        meta->effectsParam[i] = it->value[i].GetFloat();
//    }
//    
//// event
//    it = jobj.FindMember("Event");
//    if (it == jobj.MemberEnd())
//    {
//        error("ParseJObject failed, could not find Event\n");
//        delete meta;
//        return NULL_PTR;
//    }
//    meta->event = (EventType)it->value.GetInt();
//    
//    it = jobj.FindMember("EventParam");
//    if (it == jobj.MemberEnd())
//    {
//        error("ParseJObject failed, could not find EventParam\n");
//        delete meta;
//        return NULL_PTR;
//    }
//    for (int i = 0; i < it->value.Size(); ++i)
//    {
//        meta->eventParam[i] = it->value[i].GetFloat();
//    }
//    
//    it = jobj.FindMember("TriggerChance");
//    if (it == jobj.MemberEnd())
//    {
//        error("ParseJObject failed, could not find TriggerChance\n");
//        delete meta;
//        return NULL_PTR;
//    }
//    meta->triggerChance = it->value.GetInt();
//    
//    it = jobj.FindMember("Condition");
//    if (it == jobj.MemberEnd())
//    {
//        error("ParseJObject failed, could not find Condition\n");
//        delete meta;
//        return NULL_PTR;
//    }
//    meta->condition = (ModConditionType)it->value.GetInt();
//    
//    it = jobj.FindMember("ConParam");
//    if (it == jobj.MemberEnd())
//    {
//        error("ParseJObject failed, could not find ConParam\n");
//        delete meta;
//        return NULL_PTR;
//    }
//    meta->conParam = it->value.GetInt();
//    
//    it = jobj.FindMember("EffectiveTime");
//    if (it == jobj.MemberEnd())
//    {
//        error("ParseJObject failed, could not find EffectiveTime\n");
//        delete meta;
//        return NULL_PTR;
//    }
//    meta->effectiveTime = (BattleStateType)it->value.GetInt();
//    
//    it = jobj.FindMember("TriggerTimes");
//    if (it == jobj.MemberEnd())
//    {
//        error("ParseJObject failed, could not find TriggerTimes\n");
//        delete meta;
//        return NULL_PTR;
//    }
//    meta->triggerTimes = it->value.GetInt();
//
    
// others
    it = jobj.FindMember("BuffType");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find BuffType\n");
        delete meta;
        return NULL_PTR;
    }
    meta->BuffType = (BuffType)it->value.GetInt();
    
    
    it = jobj.FindMember("LastRound");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find LastRound\n");
        delete meta;
        return NULL_PTR;
    }
    meta->LastRound = it->value.GetInt();
    
    it = jobj.FindMember("MaxLayer");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find MaxLayer\n");
        delete meta;
        return NULL_PTR;
    }
    meta->MaxLayer = it->value.GetInt();
    
    it = jobj.FindMember("GroupId");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find GroupId\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size() && i < MAX_BUFF_GROUP_CNT; ++i)
    {
        meta->GroupId[i] = it->value[i].GetInt();
    }

    it = jobj.FindMember("GroupPriority");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find GroupPriority\n");
        delete meta;
        return NULL_PTR;
    }
    meta->GroupPriority = it->value.GetInt();
    
    it = jobj.FindMember("DuraType");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find DuraType\n");
        delete meta;
        return NULL_PTR;
    }
    meta->DuraType = (BuffDuraType)it->value.GetInt();

    it = jobj.FindMember("BeLegend");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find BeLegend\n");
        delete meta;
        return NULL_PTR;
    }
    meta->BeLegend = it->value.GetInt();

    it = jobj.FindMember("PeriodSnap");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PeriodSnap\n");
        delete meta;
        return NULL_PTR;
    }
    meta->PeriodSnap = it->value.GetInt();

    it = jobj.FindMember("SnapAttribute");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find SnapAttribute\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->SnapAttribute[i] = it->value[i].GetInt();
    }
    return meta;
}

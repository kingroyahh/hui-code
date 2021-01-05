#include "EffectMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void EffectMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_EFFECTS);
#endif
}

EffectMeta* EffectMetaSet::ParseJObject(Value::Object &jobj)
{
    EffectMeta* meta = new EffectMeta();
    
    //init
    memset(meta, 0, sizeof(EffectMeta));

    Value::ConstMemberIterator it;
    
    it = jobj.FindMember("id");
    if (it == jobj.MemberEnd())
    {
        error("EffectMeta ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();

    it = jobj.FindMember("DoEffect");
    if (it == jobj.MemberEnd())
    {
        error("EffectMeta ParseJObject failed, could not find DoEffect\n");
        delete meta;
        return NULL_PTR;
    }
    meta->DoEffect = it->value.GetInt();

    it = jobj.FindMember("TargetType");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find TargetType\n");
        delete meta;
        return NULL_PTR;
    }
    meta->TargetType = (TargetType)it->value.GetInt();
    
    it = jobj.FindMember("TargetFilter");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find TargetFilter\n");
        delete meta;
        return NULL_PTR;
    }
    meta->TargetFilter = (TargetFilter)it->value.GetInt();
    
    it = jobj.FindMember("TargetFilter1");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find TargetFilter1\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->TargetFilter1[i] = (TargetFilter1)it->value[i].GetInt();
    }

    it = jobj.FindMember("TargetFilter2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find TargetFilter1\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->TargetFilter2[i] = (TargetFilter2)it->value[i].GetInt();
    }

    it = jobj.FindMember("TargetFilter3");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find TargetFilter3\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->TargetFilter3[i] = (TargetFilter3)it->value[i].GetInt();
    }

    it = jobj.FindMember("TargetParam");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find TargetParam\n");
        delete meta;
        return NULL_PTR;
    }
    meta->TargetParam = it->value.GetInt();
    
    it = jobj.FindMember("TargetDmgParam");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find TargetDmgParam\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->TargetDmgParam[i] = it->value[i].GetFloat();
    }

    meta->NoDodge = 0;
    it = jobj.FindMember("NoDodge");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find NoDodge\n");
    }
    else
    {
        meta->NoDodge = it->value.GetInt();
    }
    
    

    it = jobj.FindMember("SubTarget");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find SubTarget\n");
        delete meta;
        return NULL_PTR;
    }
    meta->SubTarget = (SubTargetType)it->value.GetInt();
    
    it = jobj.FindMember("SubEffectsPara");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find SubEffectsPara\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->SubEffectsPara[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("BuffPara");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find BuffPara\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->BuffPara[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("BuffDuration");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find BuffDuration\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->BuffDuration[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("BuffChance");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find BuffChance\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->BuffChance[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("SpEffects");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find SpEffects\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->SpEffects[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("SpChance");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find SpChance\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->SpChance[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("SubTarget2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find SubTarget2\n");
        delete meta;
        return NULL_PTR;
    }
    meta->SubTarget2 = (SubTargetType)it->value.GetInt();
    
    it = jobj.FindMember("SubEffectsPara2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find SubEffectsPara2\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->SubEffectsPara2[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("BuffPara2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find BuffPara2\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->BuffPara2[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("BuffChance2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find BuffChance2\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->BuffChance2[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("SpEffects2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find SpEffects2\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->SpEffects2[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("SpChance2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find SpChance2\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->SpChance2[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("TriggerSeq");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find TriggerSeq\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->TriggerSeq[i] = it->value[i].GetInt();
    }

    it = jobj.FindMember("TriggerSeq2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find TriggerSeq2\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->TriggerSeq2[i] = it->value[i].GetInt();
    }
    
    return meta;
}

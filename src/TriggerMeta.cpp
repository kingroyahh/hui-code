#include "TriggerMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void TriggerMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_TRIGGER);
#endif
}

TriggerMeta* TriggerMetaSet::ParseJObject(Value::Object &jobj)
{
    TriggerMeta* meta = new TriggerMeta();

    //init
    memset(meta, 0, sizeof(TriggerMeta));

    Value::ConstMemberIterator it;

    it = jobj.FindMember("Id");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();

    it = jobj.FindMember("TriggerType");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find TriggerType\n");
        delete meta;
        return NULL_PTR;
    }
    meta->TriggerType = (ETriggerType)it->value.GetInt();

    it = jobj.FindMember("Event");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find Event\n");
        delete meta;
        return NULL_PTR;
    }
    meta->Event = (EEventType)it->value.GetInt();
    
    it = jobj.FindMember("Priority");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find Priority\n");
        delete meta;
        return NULL_PTR;
    }
    meta->Priority = it->value.GetInt();

    it = jobj.FindMember("TriggerChance");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find TriggerChance\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size() && i< TriggerMeta::TriggerChanceCnt; ++i)
    {
        meta->TriggerChance[i] = it->value[i].GetInt();
    }

    it = jobj.FindMember("TriggerTimes");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find TriggerTimes\n");
        delete meta;
        return NULL_PTR;
    }
    meta->TriggerTimes = it->value.GetInt();

    it = jobj.FindMember("EventParam1");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find EventParam1\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->EventParam1[i] = it->value[i].GetInt();
    }

    it = jobj.FindMember("EventParam2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find EventParam2\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->EventParam2[i] = it->value[i].GetInt();
    }

    it = jobj.FindMember("ConditionId");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ConditionId\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->ConditionId[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("ActionEcount");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ActionEcount\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->ActionEcount[i] = it->value[i].GetInt();
    }

    it = jobj.FindMember("ActionType");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ActionType\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->ActionType[i] = (TriggerActionType) it->value[i].GetInt();
    }

    it = jobj.FindMember("ActionParam");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ActionParam\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->ActionParam[i] = it->value[i].GetInt();
    }

    it = jobj.FindMember("ActionParam1");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ActionParam1\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->ActionParam1[i] = it->value[i].GetInt();
    }

    it = jobj.FindMember("ActionParam2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ActionParam2\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->ActionParam2[i] = it->value[i].GetInt();
    }

    it = jobj.FindMember("ActionParam3");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ActionParam2\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->ActionParam3[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("ActionParam4");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ActionParam2\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->ActionParam4[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("ActionReset");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ActionReset\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->ActionReset[i] = it->value[i].GetInt();
    }

    it = jobj.FindMember("ActionCaster");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ActionCaster\n");
        delete meta;
        return NULL_PTR;
    }
    for (unsigned int i = 0; i < it->value.Size(); ++i)
    {
        meta->ActionCaster[i] = (TriggerActionCaster)it->value[i].GetInt();
    }

    it = jobj.FindMember("ActionDelay");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find ActionDelay\n");
        delete meta;
        return NULL_PTR;
    }
    meta->ActionDelay = it->value.GetInt();

    return meta;
}

#include "FightCondMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void FightCondMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_FIGHT_COND);
#endif
}

FightCondMeta* FightCondMetaSet::ParseJObject(Value::Object &jobj)
{
    FightCondMeta* meta = new FightCondMeta();

    //init
    memset(meta, 0, sizeof(FightCondMeta));

    Value::ConstMemberIterator it;

    it = jobj.FindMember("Id");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();

    it = jobj.FindMember("Condition");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find Condition\n");
        delete meta;
        return NULL_PTR;
    }
    meta->Condition = (TriggerCondType)it->value.GetInt();
    
    it = jobj.FindMember("ConParam");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ConParam\n");
        delete meta;
        return NULL_PTR;
    }
    meta->ConParam = it->value.GetInt();

    it = jobj.FindMember("ConTarget");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ConTarget\n");
        delete meta;
        return NULL_PTR;
    }
    meta->ConTarget = (TriggerConTarget)it->value.GetInt();
    
    it = jobj.FindMember("ConTarget2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ConTarget\n");
        delete meta;
        return NULL_PTR;
    }
    meta->ConTarget2 = (TriggerConTarget)it->value.GetInt();

    it = jobj.FindMember("Relation");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ConTarget\n");
        delete meta;
        return NULL_PTR;
    }
    meta->Relation = (CondRelation)it->value.GetInt();
    
    it = jobj.FindMember("ConAttr");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ConTarget\n");
        delete meta;
        return NULL_PTR;
    }
    meta->ConAttr = (ConMatchType)it->value.GetInt();
    
    it = jobj.FindMember("ConAttr2");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ConTarget\n");
        delete meta;
        return NULL_PTR;
    }
    meta->ConAttr2 = (ConMatchType)it->value.GetInt();
    
    return meta;
}

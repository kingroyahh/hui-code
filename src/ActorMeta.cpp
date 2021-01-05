#include "ActorMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void ActorMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_ACTORS);
#endif
}

ActorMeta* ActorMetaSet::ParseJObject(Value::Object &jobj)
{
    ActorMeta* meta = new ActorMeta();

    //init
    memset(meta, 0, sizeof(ActorMeta));

    Value::ConstMemberIterator it;

    it = jobj.FindMember("Id");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();

    it = jobj.FindMember("SkillActorId");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find SkillActorId\n");
        delete meta;
        return NULL_PTR;
    }
    meta->SkillActorId = it->value.GetInt();

    it = jobj.FindMember("Nationality");
    if (it == jobj.MemberEnd())
    {
        error("ActorMeta ParseJObject failed, could not find Nationality\n");
        delete meta;
        return NULL_PTR;
    }
    meta->Nationality = (ENationality)it->value.GetInt();

    return meta;
}

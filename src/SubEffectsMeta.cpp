#include "SubEffectsMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void SubEffectsMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_SUB_EFFECTS);
#endif
}

SubEffectsMeta* SubEffectsMetaSet::ParseJObject(Value::Object &jobj)
{
    SubEffectsMeta* meta = new SubEffectsMeta();

    //init
    memset(meta, 0, sizeof(SubEffectsMeta));
    
    Value::ConstMemberIterator it;
    
    it = jobj.FindMember("id");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();

    it = jobj.FindMember("DoRage");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find DoRage\n");
        delete meta;
        return NULL_PTR;
    }
    meta->DoRage = it->value.GetInt();

    it = jobj.FindMember("DoEvent");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find DoEvent\n");
        delete meta;
        return NULL_PTR;
    }
    meta->DoEvent = it->value.GetInt();

    it = jobj.FindMember("FuncType");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find FuncType\n");
        delete meta;
        return NULL_PTR;
    }
    meta->funcType = (FuncType)it->value.GetInt();

    it = jobj.FindMember("FuncParams");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find FuncParams\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->funcParams[i] = it->value[i].GetFloat();
    }
    return meta;
}
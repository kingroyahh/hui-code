#include "CombatAttrMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void CombatAttrMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_COMBAT_ATTR);
#endif
}

CombatAttrMeta* CombatAttrMetaSet::ParseJObject(Value::Object &jobj)
{
    CombatAttrMeta* meta = new CombatAttrMeta();

    //init
    memset(meta, 0, sizeof(CombatAttrMeta));

    Value::ConstMemberIterator it;

    it = jobj.FindMember("Id");
    if (it == jobj.MemberEnd())
    {
        error("CombatAttrMeta ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();

    it = jobj.FindMember("Value");
    if (it == jobj.MemberEnd())
    {
        error("CombatAttrMeta ParseJObject failed, could not find Value\n");
        delete meta;
        return NULL_PTR;
    }
    meta->atValue = it->value.GetFloat();

    return meta;
}
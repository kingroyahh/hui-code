#include "AffixMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void AffixMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_AFFIX);
#endif
}

AffixMeta* AffixMetaSet::ParseJObject(Value::Object &jobj)
{
    AffixMeta* meta = new AffixMeta();
    
    //init
    memset(meta, 0, sizeof(AffixMeta));

    Value::ConstMemberIterator it;

    it = jobj.FindMember("Id");
    if (it == jobj.MemberEnd())
    {
        error("AffixMeta ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();

    it = jobj.FindMember("AttrList");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find AttrList\n");
        delete meta;
        return NULL_PTR;
    }

    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->attrList[i].attrId = it->value[i].GetInt();
    }

    it = jobj.FindMember("ValueList");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ValueList\n");
        delete meta;
        return NULL_PTR;
    }

    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->attrList[i].attrValue = it->value[i].GetFloat();
    }

    it = jobj.FindMember("NeedPower");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find NeedPower\n");
        delete meta;
        return NULL_PTR;
    }

    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->attrList[i].attrPower = it->value[i].GetInt();
    }

    return meta;
}

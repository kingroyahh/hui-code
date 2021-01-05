#include "ValueMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void ValueMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_VALUE);
#endif
}

ValueMeta* ValueMetaSet::ParseJObject(Value::Object &jobj)
{
    ValueMeta* meta = new ValueMeta();

    Value::ConstMemberIterator it;

    it = jobj.FindMember("Id");
    if (it == jobj.MemberEnd())
    {
        error("ValueMeta ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();

    it = jobj.FindMember("FloatValue");
    if (it == jobj.MemberEnd())
    {
        error("ValueMeta ParseJObject failed, could not find FloatValue\n");
        delete meta;
        return NULL_PTR;
    }
    meta->floatValue = it->value.GetFloat();
    

    it = jobj.FindMember("NumValue");
    if (it == jobj.MemberEnd())
    {
        error("ValueMeta ParseJObject failed, could not find NumValue\n");
        delete meta;
        return NULL_PTR;
    }
    meta->NumValue = it->value.GetInt();

    it = jobj.FindMember("FloatValueArry");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find FloatValueArry\n");
        delete meta;
        return NULL_PTR;
    }
    meta->FloatValueArry = new float[it->value.Size()];
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->FloatValueArry[i] = it->value[i].GetFloat();
    }

    it = jobj.FindMember("NumValueArry");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find NumValueArry\n");
        delete meta;
        return NULL_PTR;
    }
    meta->NumValueArry = new int[it->value.Size()];
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->NumValueArry[i] = it->value[i].GetInt();
    }
    return meta;
}


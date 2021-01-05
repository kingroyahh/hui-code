#include "BattleModeMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void BattleModeMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_BATTLEMODE);
#endif
}

BattleModeMeta* BattleModeMetaSet::ParseJObject(Value::Object &jobj)
{
    BattleModeMeta* meta = new BattleModeMeta();
    //init
    memset(meta, 0, sizeof(BattleModeMeta));

    Value::ConstMemberIterator it;

    it = jobj.FindMember("Id");
    if (it == jobj.MemberEnd())
    {
        error("BattleModeMeta ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();

    it = jobj.FindMember("TargetType_Attacker");
    if (it == jobj.MemberEnd())
    {
        error("BattleModeMeta ParseJObject failed, could not find TargetType_Attacker\n");
        delete meta;
        return NULL_PTR;
    }
    meta->TargetType_Attacker = (ESelectTarget)it->value.GetInt();

    it = jobj.FindMember("TargetType_Defender");
    if (it == jobj.MemberEnd())
    {
        error("BattleModeMeta ParseJObject failed, could not find TargetType_Defender\n");
        delete meta;
        return NULL_PTR;
    }
    meta->TargetType_Defender = (ESelectTarget)it->value.GetInt();

    it = jobj.FindMember("SwitchAutoBattle");
    if (it == jobj.MemberEnd())
    {
        error("BattleModeMeta failed, could not find SwitchAutoBattle\n");
        delete meta;
        return NULL_PTR;
    }
    meta->SwitchAutoBattle = (ESwitchToAuto)it->value.GetInt();

    it = jobj.FindMember("BattleType");
    if (it == jobj.MemberEnd())
    {
        error("BattleModeMeta failed, could not find BattleType\n");
        delete meta;
        return NULL_PTR;
    }
    meta->BattleType = (EBattleType)it->value.GetInt();

    it = jobj.FindMember("ComboRule_Attacker");
    if (it == jobj.MemberEnd())
    {
        error("BattleModeMeta failed, could not find ComboRule_Attacker\n");
        delete meta;
        return NULL_PTR;
    }
    meta->ComboRule_Attacker = (EBMCombo)it->value.GetInt();

    it = jobj.FindMember("ComboRule_Defender");
    if (it == jobj.MemberEnd())
    {
        error("BattleModeMeta failed, could not find ComboRule_Defender\n");
        delete meta;
        return NULL_PTR;
    }
    meta->ComboRule_Defender = (EBMCombo)it->value.GetInt();
    
    it = jobj.FindMember("RoundAffix");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find FuncParams\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->RoundAffix[i] = it->value[i].GetInt();
    }
    
    it = jobj.FindMember("RoundAffixPower");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find FuncParams\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->RoundAffixPower[i] = it->value[i].GetInt();
    }
    
    return meta;
}

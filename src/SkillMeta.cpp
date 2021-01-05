#include "SkillMeta.h"
#include "Log.h"
#include "Meta.h"
#include "Define.h"
#include "MetaAlias.h"

#include <string>

using namespace std;
using namespace rapidjson;

void SkillMetaSet::AssignPath()
{
    fpath = string(GetMetaRootPath());
#ifdef GRAV_UNITY_PLUGIN
    fpath.append(dbfname);
#else
    fpath.append(META_ALIAS_SKILLS);
#endif
}

SkillMeta* SkillMetaSet::ParseJObject(Value::Object &jobj)
{
    SkillMeta* meta = new SkillMeta();

    //init
    memset(meta, 0, sizeof(SkillMeta));
    
    Value::ConstMemberIterator it;
    
    it = jobj.FindMember("id");
    if (it == jobj.MemberEnd())
    {
        error("SkillMeta ParseJObject failed, could not find id\n");
        delete meta;
        return NULL_PTR;
    }
    meta->id = it->value.GetInt();
    
    it = jobj.FindMember("SkillType");
    if (it == jobj.MemberEnd())
    {
        error("SkillMeta ParseJObject failed, could not find SkillType\n");
        delete meta;
        return NULL_PTR;
    }
    meta->skillType = (SkillType)it->value.GetInt();
    
    it = jobj.FindMember("ComplexSkillId");
    if (it == jobj.MemberEnd())
    {
        error("SkillMeta ParseJObject failed, could not find ComplexSkillId\n");
        delete meta;
        return NULL_PTR;
    }
    meta->complexSkillId = it->value.GetInt();
    
    /*it = jobj.FindMember("PrepareEffects");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PrepareEffects\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->prepareEffects[i] = it->value[i].GetInt();
    }*/
    
    it = jobj.FindMember("PartnerId");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PartnerId\n");
        delete meta;
        return NULL_PTR;
    }
    meta->PartnerId = it->value.GetInt();
    
    it = jobj.FindMember("AttackEffects");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find AttackEffects\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->AttackEffects[i] = it->value[i].GetInt();
    }
    
    /*it = jobj.FindMember("EndEffects");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find EndEffects\n");
        delete meta;
        return NULL_PTR;
    }
    for (int i = 0; i < it->value.Size(); ++i)
    {
        meta->endEffects[i] = it->value[i].GetInt();
    }*/
    
    it = jobj.FindMember("PartnerId");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find PartnerId\n");
        delete meta;
        return NULL_PTR;
    }
    meta->PartnerId = it->value.GetInt();
    
    it = jobj.FindMember("RedoRate");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find RedoRate\n");
        delete meta;
        return NULL_PTR;
    }
    meta->RedoRate = it->value.GetFloat();
    
    it = jobj.FindMember("ReChoose");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ReChoose\n");
        delete meta;
        return NULL_PTR;
    }
    meta->ReChoose = it->value.GetInt();
    
    it = jobj.FindMember("ExpChange");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find ExpChange\n");
        delete meta;
        return NULL_PTR;
    }
    meta->ExpChange = it->value.GetInt();
    
    it = jobj.FindMember("Atk123Time");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find Atk123Time\n");
        delete meta;
        return NULL_PTR;
    }
    meta->Atk123Time = it->value.GetInt();

    it = jobj.FindMember("StartScrollTime");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find StartScrollTime\n");
        delete meta;
        return NULL_PTR;
    }
    meta->StartScrollTime = it->value.GetInt();

    it = jobj.FindMember("StartComboTime");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find StartComboTime\n");
        delete meta;
        return NULL_PTR;
    }
    meta->StartComboTime = it->value.GetInt();

    it = jobj.FindMember("EndComboTime");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find EndComboTime\n");
        delete meta;
        return NULL_PTR;
    }
    meta->EndComboTime = it->value.GetInt();

    it = jobj.FindMember("EndScrollTime");
    if (it == jobj.MemberEnd())
    {
        error("ParseJObject failed, could not find EndScrollTime\n");
        delete meta;
        return NULL_PTR;
    }
    meta->EndScrollTime = it->value.GetInt();

    return meta;
}

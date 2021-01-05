#ifndef _GRAV_SKILL_META_H_
#define _GRAV_SKILL_META_H_

#include "Enum.h"
#include "Type.h"
#include "MetaSet.h"
#include "Singleton.h"

#include "rapidjson/document.h"

using namespace rapidjson;

struct SkillMeta
{
    int id;
    SkillType skillType;
    int AttackEffects[MAX_EFFECT_COUNT];
    int complexSkillId;
    int PartnerId;
    float RedoRate;
    int ReChoose;
    int ExpChange;
    int Atk123Time;
    int StartScrollTime;
    int StartComboTime;
    int EndComboTime;
    int EndScrollTime;
};

class SkillMetaSet : public BaseMetaSet<SkillMeta>, public Singleton<SkillMetaSet>
{
protected:
    void AssignPath() override;
    SkillMeta* ParseJObject(Value::Object &jobj) override;
};

#endif

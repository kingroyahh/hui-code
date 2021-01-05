#ifndef _GRAV_ACTOR_SKILL_H_
#define _GRAV_ACTOR_SKILL_H_
#include "Type.h"
#include "Enum.h"

class ActorSkill
{
public:
    ActorPtr GetOwner();
    void InitOwner(ActorPtr actor);
    void InitDefaultSkill(int skillId, int skillLv);
    void InitLegendSkill(int skillId, int skillLv);
    void InitXpSkill(int skillId, int skillLv);
    void InitCombineSkill(int skillId, int skillLv);
    void InitDefaultSkill2(int skillId, int skillLv);

    bool AllowSkill();
    bool AllowComAtk();
    void CastSkillByType(SkillType sType, int targetPos);

    ActorSkill();
    virtual ~ActorSkill();

private:
    ActorPtr m_pOwner;
    ActorSkillSubPtr defaultSkill;
    ActorSkillSubPtr xpSkill;
    ActorSkillSubPtr combineSkill;
    ActorSkillSubPtr legendSkill;
};

#endif

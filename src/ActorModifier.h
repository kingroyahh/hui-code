#ifndef _GRAV_ACTOR_MOD_H_
#define _GRAV_ACTOR_MOD_H_
#include "Type.h"
#include "Enum.h"
using namespace battle;
class ActorModifier
{
public:
    ActorPtr owner;
    void InitOwner(ActorPtr actor);
    int legendSkillActive;
    int restrictAction;
    int restrictXpSkill;
    int restrictCommonSkill;
    int sealedLayer;

    void AddMod(ActorPtr caster, int modId, int level, int round);
    void RemoveModByImmune(ActorPtr caster, int groupId, int layerCount,int triggerID);
    void RemoveModByDispel(ActorPtr caster, int groupId, int layerCount, int triggerID);
    void RemoveModByType(ActorPtr caster, BuffType bType, int subEffectID);
    void RemoveModByTrigger(ActorPtr caster,ActorModifierSubPtr mod);
    void ModEffect(PeriodType sType, int triggerArea);
    void RoundCount();
    void AppendTempAffix(BuffAffixApply affixType, int skillLevel, TempAttrInfo &tempAttr);
    void AccquireAffixPowerInfo(BuffAffixApply affixType, int skillLevel,TempAffixPowerInfo& tempInfo);

    int GetModCntByGroupID(int groupId);
    bool IsRestrictAction();
    bool IsRestrictCommonSkill();
    bool IsSealedState();
    bool IsLegendSkillActive();
    float TriggerShieldDamage(ActorPtr caster, float damage, int damageID, ShieldInfo *pShieldInfo);
    bool CheckIsByImmune(ActorPtr caster, int groupId);
    void CheckDeleteMods();
    ActorModifier();
    virtual ~ActorModifier();
    ActorModifierSubPtr GetValidModSubByIndex(int index);
private:
    ActorModifierSubPtr modSubArray[MAX_MOD_COUNT];
    void RemoveModByGroupId(ActorPtr caster, int groupId, int layerCount, BuffVanishType vanish, int triggerID);
    void RemoveMod(ActorModifierSubPtr mod);
    int GetModLayerCount(int modId);
};

#endif
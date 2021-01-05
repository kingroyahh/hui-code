#ifndef _GRAV_ACTOR_SKILL_SUB_H_
#define _GRAV_ACTOR_SKILL_SUB_H_
#include "Type.h"
#include "SkillMeta.h"
#include "EffectMeta.h"
#define MaxSubEffectCnt 64
#define MaxBuffCnt 64
class EffectSub;
class ActorSkillSub
{
public:
    const SkillMeta* skillMeta;
    int skillId;
    int skillLv;
    ActorSkillPtr ActorSkill;
    int xpChange;
    bool DoSkill(int targetPos,SkillInfo& sInfo);
    bool DoSkill(int targetPos, SkillInfo&& sInfo);
    bool InitSkill(int skillId, int skillLv,bool appXpValue);
    
    ActorSkillSub(ActorSkillPtr skill);
    virtual ~ActorSkillSub();
    int RegisterSubeffectID(int subEffID);
    int RegisterBuffID(int buffID);
private:
    int SubEffectIDArray[MaxSubEffectCnt];
    int BuffIDArray[MaxBuffCnt];
    EffectSub* AttackEffects[MAX_EFFECT_COUNT];
    int AttackEffectCnt;
};

class EffectTrigger
{
public:
    virtual ~EffectTrigger();
    EffectSub* GetParent(){ return effParent;}
    virtual void Init(EffectSub * parent, int triggerID,bool first)
    {
        effTriggerID = triggerID;
        effParent = parent;
        firstSubTarget = first;
    }
    void ExecuteTrigger(ActorPtr target, float tarDmgParam, int skillLv, bool isMain, bool isLowestXp, bool isHighestXp, SkillType skillType);
    int effTriggerID; // 这个就是SubEffect Id
protected:
    bool CheckSubTarget(ActorPtr target, bool isMain, bool isLowestXp, bool isHighestXp);
    bool CheckSubTarget(SubTargetType subTarget, bool isMain, bool isLowestXp, bool isHighestXp);
    virtual void ExecuteTriggerImpl(ActorPtr target,bool isMainTarget, float tarDmgParam, int skillLv, SkillType skillType) = 0;
    
    bool firstSubTarget;
    EffectSub * effParent; //这个Trigger所属的EffectSub
};

class SubEffectTrigger :public EffectTrigger
{
public:
    void Init(EffectSub * parent, int triggerID,bool firstSubTarget);
protected:
    void ExecuteTriggerImpl(ActorPtr target, bool isMainTarget,float tarDmgParam, int skillLv, SkillType skillType);
    int effOrderID;
};

class SpEffectTrigger :public SubEffectTrigger
{
public:
    void InitProbability(int probableValue)
    {
        hitProbability = probableValue;
    }
protected:
    void ExecuteTriggerImpl(ActorPtr target, bool isMainTarget, float tarDmgParam, int skillLv, SkillType skillType);
private:
    int hitProbability;
};
class BuffEffectTrigger :public EffectTrigger
{
public:
    void InitProbability(int probableValue)
    {
        hitProbability = probableValue;
    }
    void Init(EffectSub * parent, int triggerID, bool firstSubTarget);
protected:
    void ExecuteTriggerImpl(ActorPtr target, bool isMainTarget ,float tarDmgParam, int skillLv, SkillType skillType);
private:
    int hitProbability;
    int buffOrderID;
};

class EffectSub
{
public:
    using ModiyAffixPair = std::tuple<int, int>;
    using ExBuffChancePair = std::pair<int, int>;
    EffectSub(ActorPtr caster, int eId, int sLv);
    void GetEffectTarget(int targetPos, ActorList *c1, ActorList *c2, Actor* buffCaster, Actor *buffOwner);
    void ExecuteEffect(const SkillInfo& sInfo);
    void InitSkill(ActorSkillSubPtr skill)
    {
        m_pSkillSub = skill;
    }
    
    void MakeTrace(TriggerTrace td){
        traceData.skillId = td.skillId;
        traceData.subEffectId = td.subEffectId;
        traceData.buffId = td.buffId;
        traceData.effectId = td.effectId;
    }
    void InitOriginBuffID(int buffID)
    {
        originBuffID = buffID;
    }
    int GetOriginBuffID()
    {
        return originBuffID;
    }
    ActorSkillSubPtr GetSkill()
    {
        return m_pSkillSub;
    }
    ActorPtr GetMainTarget()
    {
        return m_pMainTarget;
    }
    const EffectMeta* GetEffectMeta()
    {
        return m_pEffectMeta;
    }
    ActorPtr GetCaster()
    {
        return m_pCaster;
    }
    void ModifyExtraAffix(Actor* pActor, int affixID, int affixBase, int affixGrow);
    
    //No.1 method to calculate buff probality
    void ModifyBuffProbality(int buffIndex, int chanceBase,int chanceGrow)
    {
        auto realLevel = skillLv;
        if (realLevel >= 1)
        {
            realLevel--;
        }
        auto affixPower = chanceBase + chanceGrow * realLevel;
        exBuffChanceList.push_back(std::make_pair(buffIndex, affixPower));
    }
    
    int GetExChanceValue(int buffIndex)
    {
        for (auto & tBuff : exBuffChanceList)
        {
            if (tBuff.first == buffIndex)
            {
                return tBuff.second;
            }
        }
        return 0;
    }
    void SetCurOrder(int order);
    int RegisterSubeffectID(int subID);
    int RegisterBuffID(int buffID);
    virtual ~EffectSub();
    inline void InjectMainTarget(Actor* pMainTarget)
    {
        m_pMainTarget = pMainTarget;
    }
    inline void InjectTargetList(ActorList& targetList)
    {
        auto targetSize = sizeof(m_pTargetList);
        memset(m_pTargetList, 0, targetSize);
        for (int i = 0, index = 0; i < ActorList::Size; i++)
        {
            if (targetList.list[i] != nullptr)
            {
                m_pTargetList[index++] = targetList.list[i];
            }
        }
    }
    
    TriggerTrace  traceData;
    int EffectId() {
        return GetEffectMeta()->id;
    }
private:
    ActorPtr GetEffectTarget(int casterPos, TargetType tType, TargetFilter tFilter, int tNum, int targetPos, ActorPtr *targetList);
    void AppendTriggerEffect(EffectTrigger* effect);
    void AppendSubEffTrigger(int triggerID, bool first);
    void AppendSpSubEffTrigger(int triggerID, int probability, bool first);
    void AppendBuffEffTrigger(int triggerID, int probability, bool first);
    void executeEffectForTarget(ActorPtr target, float tarDmgParam, int skillLv, bool isMain, bool isLowestXp, bool isHighestXp, SkillType skillType);
    ActorPtr m_pCaster;
    ActorPtr m_pMainTarget;
    ActorPtr m_pTargetList[MAX_DMG_TARGET_COUNT];
    EffectTrigger* m_pTriggerEffectList[MaxSubEffectCnt];
    ActorSkillSubPtr m_pSkillSub; 
    const EffectMeta* m_pEffectMeta;
    int originBuffID;
    int skillLv;
    int m_iEffectId;
    int m_iTriggerIndex;
    struct ActorAffix
    {
        Actor * pActor;
        TempAttrInfo tempInfo;
        vector<std::pair<int, int>> attrInfoList;
        ActorAffix() :pActor(nullptr){}
    };
    std::vector<ActorAffix> modifyAffixList;
    std::vector<ExBuffChancePair> exBuffChanceList;
};

#endif

#ifndef _GRAV_BATTLE_TRIGGER_H_
#define _GRAV_BATTLE_TRIGGER_H_

#include "Type.h"
#include "Enum.h"
#include "TriggerMeta.h"
#include "FightCondMeta.h"
#include <list>
#include "PrivateChecker.h"
class BattleTrigger;
class ActorModifierSub;
class TriggerAction;
class TriggerCondition;
class CheckTriggerPrivate;
class ITriggerElemFactory
{
public:
    virtual TriggerAction* CreateAction(TriggerActionType aType) = 0;
    virtual void DestroyAction(TriggerActionType aType, TriggerAction *pAction) = 0;
    virtual TriggerCondition* CreateCondition(TriggerCondType aType) = 0;
    virtual void DestroyCondition(TriggerCondType aType, TriggerCondition *pCondition) = 0;
    virtual CheckTriggerPrivate* CreatePrivateChecker(PrivateCheckType aType) = 0;
    virtual void DestroyPrivateChecker(PrivateCheckType aType, CheckTriggerPrivate* pPrivateChecker) = 0;
};

class BattleTrigger
{
public: 
    BattleTrigger();
    virtual ~BattleTrigger();
    inline EEventType GetEventType()
    {
        return (triggerMeta != NULL) ? triggerMeta->Event : EEventType::Invalid;
    }
    inline int GetTriggerID()
    {
        if (triggerMeta != NULL)
        {
            return triggerMeta->id;
        }
        return 0;
    }
    inline int GetInstID()
    {
        return instanceID;
    }
    inline void NotifyInvalid()
    {
        invalid = true;
    }
    inline bool IsInvalid()
    {
        return invalid;
    }
    inline int GetPriority()
    {
        if (triggerMeta != NULL)
        {
            return triggerMeta->Priority;
        }
        return 0;
    }
    const TriggerMeta* GetMeta();
    inline ActorModifierSub* GetOriginBuff(){ return pOriginBuff; }
    void InitTrigger(TriggerMeta *pMeta, int instID, ITriggerElemFactory *pFactory);
    void InitOriginBuff(ActorModifierSub* buff);
    bool CheckPrivate(const TriggerParam& param);
    bool CheckProbability();
    bool CheckRoundTimesLimit();
    bool CheckAllConditions(const TriggerParam& param);
    void ExecuteActions(TriggerParam& param);
    bool HasEffect();
    void ResetTimesLimit();
    virtual void OnDestroy();
    virtual bool CheckEvtParams(const TriggerParam& param);
protected:
    virtual void CreatePrivateChecker() = 0;
protected:
    CheckTriggerPrivate* pPrivateChecker;
    ActorModifierSub* pOriginBuff;
    int triggerTimes;
    int instanceID;
    bool invalid;
    TriggerMeta *triggerMeta;
    ITriggerElemFactory *pTriggerElemFactory;
    typedef std::list<TriggerAction*> ActionList;
    ActionList actionList;
    typedef std::list<TriggerCondition*> ConditionList;
    ConditionList conditionList;
};

class BattleTriggerLess
{
public :
    bool operator()(BattleTrigger* first, BattleTrigger* second) const
    {
        return first->GetPriority() > second->GetPriority();
    }
};
template<int checkType>
class PrivateCheckTrigger : public BattleTrigger
{
protected:
    virtual void CreatePrivateChecker()override
    {
        if (pTriggerElemFactory != NULL)
        {
            pPrivateChecker = pTriggerElemFactory->CreatePrivateChecker(PrivateCheckType(checkType));
            if (pPrivateChecker != NULL)
            {
                pPrivateChecker->Init(PrivateCheckType(checkType));
            }
        }
    }
};
class BattleCauseDamageTrigger :public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class BattleSufferDamageTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class BattleCauseHealTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class BattleSufferHealTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class BattleCastSkillTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
public:
    virtual bool CheckEvtParams(const TriggerParam& param);
};

class BattleSufferSkillTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1List>
{
public:
    virtual bool CheckEvtParams(const TriggerParam& param);
};

class BattleActorDeadTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class BattleGainBuffTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
public:
    virtual bool CheckEvtParams(const TriggerParam& param);
};

class BattleBuffOverTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
public:
    virtual bool CheckEvtParams(const TriggerParam& param);
};

class BattleRoundBeginTrigger : public PrivateCheckTrigger<PrivateCheckType::Neglect>
{
};

class BattleDodgeTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};
class BattleMissTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};
class BattleComingCauseDamageTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};
class BattleComingSufferDamageTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};
class BattleComingCauseHealTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class BattleComingSufferHealTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class HealSetTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class RoundSmallBeginTrigger : public PrivateCheckTrigger<PrivateCheckType::Neglect>
{
public:
    virtual bool CheckEvtParams(const TriggerParam& param);
};

class RoundSmallEndTrigger : public PrivateCheckTrigger<PrivateCheckType::Neglect>
{
public:
    virtual bool CheckEvtParams(const TriggerParam& param);
};

class BeforeEffectAddBuffTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class BeforeEffectAddSpEffectTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class BeforeSkillTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class AfterSkillTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class BeforeCauseSubEffectTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class AfterCauseSubEffectTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class BeforeSufferSubEffectTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};

class AfterSufferSubEffectTrigger : public PrivateCheckTrigger<PrivateCheckType::CheckC1>
{
};


#endif /* _GRAV_BATTLE_TRIGGER_H_ */

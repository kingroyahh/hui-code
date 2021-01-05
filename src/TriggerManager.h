#ifndef _GRAV_BATTLE_TRIGGER_ACTION_MANAGER_H_
#define _GRAV_BATTLE_TRIGGER_ACTION_MANAGER_H_

#include "Type.h"
#include "Enum.h"
#include "TriggerMeta.h"
#include "BattleTrigger.h"
#include "TriggerAction.h"
#include "TriggerCondition.h"
#include "GeneralFactory.h"
#include <list>
#include <stack>
class BattleTriggerManager : public ITriggerElemFactory
{
public:
    BattleTriggerManager();
    virtual ~BattleTriggerManager();
    BattleTrigger* AddTrigger(int triggerID);
    void RemoveTrigger(EEventType evtType,int instID);
    bool PreFireEvent(TriggerParam & param);
    void FireEvent(TriggerParam & param);
    EEventType GetTopEvtType();
    int GetTopTriggerID();
    Actor* GetTopDeadActor();
    virtual TriggerAction* CreateAction(TriggerActionType aType);
    virtual void DestroyAction(TriggerActionType aType, TriggerAction *pAction);
    virtual TriggerCondition* CreateCondition(TriggerCondType aType);
    virtual void DestroyCondition(TriggerCondType aType, TriggerCondition *pCondition);
    virtual CheckTriggerPrivate* CreatePrivateChecker(PrivateCheckType aType);
    virtual void DestroyPrivateChecker(PrivateCheckType aType, CheckTriggerPrivate* pPrivateChecker);

    void ResetTimesLimitPerRound();
private:
    void CleanInvalidTriggers();
    typedef GeneralPoolFactoryMgr<TriggerActionType, TriggerAction> ActionFactoryMgr;
    ActionFactoryMgr actionFactoryMgr;
    typedef GeneralPoolFactoryMgr<TriggerCondType, TriggerCondition> ConditionFactoryMgr;
    ConditionFactoryMgr conditionFactoryMgr;
    typedef GeneralPoolFactoryMgr<EEventType, BattleTrigger> TriggerFactorMgr;
    TriggerFactorMgr triggerFactorMgr;
    typedef GeneralPoolFactoryMgr<PrivateCheckType, CheckTriggerPrivate> PrivateCheckerFactoryMgr;
    PrivateCheckerFactoryMgr privateCheckerFactoryMgr;
    typedef std::list<BattleTrigger*> TriggerList;
    TriggerList triggerListArray[EEventType::Count];
    TriggerList invalidTriggerList;
    typedef std::stack<EEventType> EventStack;
    EventStack eventStack;
    typedef std::stack<int> TriggerStack;
    TriggerStack triggerStack;
    typedef std::stack<Actor*> ActorStack;
    ActorStack deadActorStack;
    int nextInstID;
};

#endif /* _GRAV_BATTLE_TRIGGER_H_ */

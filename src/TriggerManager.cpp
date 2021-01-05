#include "TriggerManager.h"
#include "ActorModifierSub.h"
#include "Actor.h"
#include "PrivateChecker.h"
#include "TriggerRelationCondition.h"
#include "EffectUtil.h"

BattleTriggerManager::~BattleTriggerManager()
{
    for (auto & tl : triggerListArray)
    {
        for (auto trigger : tl)
        {
            trigger->OnDestroy();
            triggerFactorMgr.DestroyInst(trigger->GetEventType(), trigger);
        }
        tl.clear();
    }
}

BattleTriggerManager::BattleTriggerManager()
    :nextInstID(0)
{
    triggerFactorMgr.RegisterFactory<BattleCauseDamageTrigger>(EEventType::CauseDamage);
    triggerFactorMgr.RegisterFactory<BattleSufferDamageTrigger>(EEventType::SufferDamage);
    triggerFactorMgr.RegisterFactory<BattleCauseHealTrigger>(EEventType::CauseHeal);
    triggerFactorMgr.RegisterFactory<BattleSufferHealTrigger>(EEventType::SufferHeal);
    triggerFactorMgr.RegisterFactory<BattleCastSkillTrigger>(EEventType::CauseSkill);
    triggerFactorMgr.RegisterFactory<BattleSufferSkillTrigger>(EEventType::SufferSkill);
    triggerFactorMgr.RegisterFactory<BattleActorDeadTrigger>(EEventType::SomeActorDead);
    triggerFactorMgr.RegisterFactory<BattleGainBuffTrigger>(EEventType::GainBuff);
    triggerFactorMgr.RegisterFactory<BattleBuffOverTrigger>(EEventType::BuffOver);
    triggerFactorMgr.RegisterFactory<BattleRoundBeginTrigger>(EEventType::RoundBegin);
    triggerFactorMgr.RegisterFactory<BattleDodgeTrigger>(EEventType::Dodge);
    triggerFactorMgr.RegisterFactory<BattleMissTrigger>(EEventType::Miss);
    triggerFactorMgr.RegisterFactory<BattleComingCauseDamageTrigger>(EEventType::ComingCauseDamage);
    triggerFactorMgr.RegisterFactory<BattleComingSufferDamageTrigger>(EEventType::ComingSufferDamage);
    triggerFactorMgr.RegisterFactory<BattleComingCauseHealTrigger>(EEventType::ComingCauseHeal);
    triggerFactorMgr.RegisterFactory<BattleComingSufferHealTrigger>(EEventType::ComingSufferHeal);
    triggerFactorMgr.RegisterFactory<HealSetTrigger>(EEventType::NearDeath);
    triggerFactorMgr.RegisterFactory<RoundSmallBeginTrigger>(EEventType::RoundSmallBegin);
    triggerFactorMgr.RegisterFactory<RoundSmallEndTrigger>(EEventType::RoundSmallEnd);
    triggerFactorMgr.RegisterFactory<BeforeCauseSubEffectTrigger>(EEventType::BeforeCauseSubEffect);
    triggerFactorMgr.RegisterFactory<AfterCauseSubEffectTrigger>(EEventType::AfterCauseSubEffect);
    triggerFactorMgr.RegisterFactory<BeforeSufferSubEffectTrigger>(EEventType::BeforeSufferSubEffect);
    triggerFactorMgr.RegisterFactory<AfterSufferSubEffectTrigger>(EEventType::AfterSufferSubEffect);
    
    triggerFactorMgr.RegisterFactory<BeforeEffectAddBuffTrigger>(EEventType::BeforeEffectAddBuff);
    triggerFactorMgr.RegisterFactory<BeforeEffectAddSpEffectTrigger>(EEventType::BeforeEffectAddSpEffect);
    
    triggerFactorMgr.RegisterFactory<BeforeSkillTrigger>(EEventType::BeforeSkill);
    triggerFactorMgr.RegisterFactory<AfterSkillTrigger>(EEventType::AfterSkill);
    

    actionFactoryMgr.RegisterFactory<DoEffectAction>(TriggerActionType::DoEffect);
    actionFactoryMgr.RegisterFactory<RemoveSrcBuffAction>(TriggerActionType::RemoveBuff);
    actionFactoryMgr.RegisterFactory<RemoveTriggerAction>(TriggerActionType::RemoveTrigger);
    actionFactoryMgr.RegisterFactory<ExeEffectAffixAction>(TriggerActionType::ExecEffectAffix);
    actionFactoryMgr.RegisterFactory<Old_ExeEffectBuffChanceAction>(TriggerActionType::Old_ExecEffectBuffChance);
    actionFactoryMgr.RegisterFactory<ExeEffectBuffChanceAction>(TriggerActionType::ExecEffectBuffChance);
    actionFactoryMgr.RegisterFactory<ExecSpEffectChanceAction>(TriggerActionType::ExecSpEffectChance);
    actionFactoryMgr.RegisterFactory<ChangeFuncParamsAction>(TriggerActionType::ChangeFuncParams);
    actionFactoryMgr.RegisterFactory<ChangeSkillRedoRateAction>(TriggerActionType::SkillRedoRate);
    
    conditionFactoryMgr.RegisterFactory<CondHpPercentGreaterThan>(TriggerCondType::HpPercentGreaterThan);
    conditionFactoryMgr.RegisterFactory<CondHpPercentLessEqual>(TriggerCondType::HpPercentLessEqual);
    conditionFactoryMgr.RegisterFactory<CondXpPercentGreateThan>(TriggerCondType::XpPercentGreaterThan);
    conditionFactoryMgr.RegisterFactory<CondXpPercentLessEqual>(TriggerCondType::XpPercentLessEqual);
    conditionFactoryMgr.RegisterFactory<CondTargetContainsBuffGroup>(TriggerCondType::TargetHasBuffGroup);
    conditionFactoryMgr.RegisterFactory<CondTargetIsEnemy>(TriggerCondType::TargetIsEnemy);
    conditionFactoryMgr.RegisterFactory<CondTargetIsAlly>(TriggerCondType::TargetIsAlly);
    conditionFactoryMgr.RegisterFactory<CondTargetIsSelf>(TriggerCondType::TargetIsMySelf);
    conditionFactoryMgr.RegisterFactory<CondTargetIsOwner>(TriggerCondType::ConTargetIsOwner);
    conditionFactoryMgr.RegisterFactory<CondResult1EqualParam>(TriggerCondType::Result1EqualParam);
    conditionFactoryMgr.RegisterFactory<CondResult2EqualParam>(TriggerCondType::Result2EqualParam);
    conditionFactoryMgr.RegisterFactory<CondResult3EqualParam>(TriggerCondType::Result3EqualParam);
    conditionFactoryMgr.RegisterFactory<CondResult4EqualParam>(TriggerCondType::Result4EqualParam);
    conditionFactoryMgr.RegisterFactory<CondResult5EqualParam>(TriggerCondType::Result5EqualParam);
    conditionFactoryMgr.RegisterFactory<CondResult6EqualParam>(TriggerCondType::Result6EqualParam);
    conditionFactoryMgr.RegisterFactory<CondResult7EqualParam>(TriggerCondType::Result7EqualParam);
    conditionFactoryMgr.RegisterFactory<CondResult8ContainParam>(TriggerCondType::Result8ContainParam);
    conditionFactoryMgr.RegisterFactory<CondTargetGreatThanParam>(TriggerCondType::TargetResultGreaterThanParam);
    conditionFactoryMgr.RegisterFactory<CondTargetLessEqualParam>(TriggerCondType::TargetResultLessEqualParam);
    conditionFactoryMgr.RegisterFactory<CondTargetNotEqualParam>(TriggerCondType::TargetResultNotEqualParam);
    conditionFactoryMgr.RegisterFactory<CondTargetBelongToBackRowParam>(TriggerCondType::TargetBelongToBackRow);
    conditionFactoryMgr.RegisterFactory<CondTargetBelongToFrontRowParam>(TriggerCondType::TargetBelongToFrontRow);
    conditionFactoryMgr.RegisterFactory<ConTargetBelongToTrggerActorColumn>(TriggerCondType::TargetBelongToTriggerActorColumn);
    conditionFactoryMgr.RegisterFactory<ConTargetBelongToTrggerActorRow>(TriggerCondType::TargetBelongToTriggerActorRow);
    conditionFactoryMgr.RegisterFactory<CurrentEnemyCountEqualToConParam>(TriggerCondType::CurrentEnemyCountEqualToParam);
    conditionFactoryMgr.RegisterFactory<CurrentEnemyContainsBuffGroup>(TriggerCondType::CurrentEnemyContainsBuffGroupParam);
    conditionFactoryMgr.RegisterFactory<CondEqualtoFirstAttackSiseParam>(TriggerCondType::ContargetEqualtoFirstAttackSiseParam);
    conditionFactoryMgr.RegisterFactory<OwnerHpPercentGreaterThanContargetParam>(TriggerCondType::OwnerHpPercentGreaterThanContarget);
    conditionFactoryMgr.RegisterFactory<OwnerHpPercentLessEqualContargetParam>(TriggerCondType::OwnerHpPercentLessEqualContarget);
    conditionFactoryMgr.RegisterFactory<MainTargetColumnContainOneParam>(TriggerCondType::MainTargetColumnContainOne);
    
    conditionFactoryMgr.RegisterFactory<ConTargetMatchParam>(TriggerCondType::ConTargetMatch);
    privateCheckerFactoryMgr.RegisterFactory<CheckTriggerPrivateC1>(PrivateCheckType::CheckC1);
    privateCheckerFactoryMgr.RegisterFactory<CheckTriggerPrivateC1List>(PrivateCheckType::CheckC1List);
    privateCheckerFactoryMgr.RegisterFactory<CheckTriggerPrivateNeglect>(PrivateCheckType::Neglect);
}

BattleTrigger* BattleTriggerManager::AddTrigger(int triggerID)
{
    TriggerMeta *pMeta = (TriggerMeta*)TriggerMetaSet::instance()->Get(triggerID);
    if (pMeta == NULL)
    {
        error("TriggerMetaID [%d] is nost exist", triggerID);
        return NULL;
    }
    BattleTrigger* trigger = triggerFactorMgr.CreateInst(pMeta->Event);
    if (trigger == NULL)
    {
        error("TriggerFactor id[%d] event[%d] is not registered", triggerID, pMeta->Event);
        return NULL;
    }

    int instID = ++nextInstID;
    trigger->InitTrigger(pMeta, instID, this);
    triggerListArray[pMeta->Event].push_back(trigger);
    return trigger;
}

void BattleTriggerManager::RemoveTrigger(EEventType evtType,int instID)
{
    BattleTrigger* trigger = nullptr;
    auto & triggerList = triggerListArray[evtType];
    TriggerList::iterator it = triggerList.begin();
    for (; it != triggerList.end(); ++it)
    {
        if ((*it)->GetInstID() == instID)
        {
            trigger = *it;
            break;
        }
    }
    if (trigger == nullptr)
    {
        error("TriggerID[%d] is nullptr", instID);
        return;
    }
    if (trigger->IsInvalid())
    {
        error("TriggerID[%d] is invalid", instID);
        return;
    }
    
    trigger->NotifyInvalid();
    if (eventStack.size() > 0)
    {
        invalidTriggerList.push_back(trigger);
    }
    else
    {
        trigger->OnDestroy();
        triggerFactorMgr.DestroyInst(trigger->GetEventType(), trigger);
        triggerList.erase(it);
    }
}

//just check can fire event
bool BattleTriggerManager::PreFireEvent(TriggerParam & param)
{
    if (eventStack.size() > MAX_TRIGGER_STACK_LENGTH)
    {
        return false ;
    }
    list<BattleTrigger*> filterTriggers;
    auto & triggerList = triggerListArray[param.evtType];
    for (auto trigger : triggerList)
    {
        if (trigger->IsInvalid())
        {
            continue;
        }
        if (trigger->CheckPrivate(param))
        {
            filterTriggers.push_back(trigger);
        }
    }
    if (filterTriggers.empty())
    {
        return false;
    }
    if (filterTriggers.size() > 1)
    {
        filterTriggers.sort(BattleTriggerLess());
    }
    for (auto trigger : filterTriggers)
    {
        if (trigger->IsInvalid())
        {
            continue;
        }
        if (!trigger->CheckEvtParams(param))
        {
            continue;
        }
        if (!trigger->CheckProbability())
        {
            continue;
        }
        if (!trigger->CheckRoundTimesLimit())
        {
            continue;
        }
        if (!trigger->CheckAllConditions(param))
        {
            continue;
        }
        
        return true;
    }
    return false;
}

void BattleTriggerManager::FireEvent(TriggerParam & param)
{
    if (eventStack.size() > MAX_TRIGGER_STACK_LENGTH)
    {
        return;
    }
    list<BattleTrigger*> filterTriggers;
    auto & triggerList = triggerListArray[param.evtType];
    for (auto trigger : triggerList)
    {
        if (trigger->IsInvalid())
        {
            continue;
        }
        if (trigger->CheckPrivate(param))
        {
            filterTriggers.push_back(trigger);
        }
    }
    if (filterTriggers.empty())
    {
        return;
    }
    if (filterTriggers.size() > 1)
    {
        filterTriggers.sort(BattleTriggerLess());
    }
    eventStack.push(param.evtType);
    if (param.evtType == EEventType::SomeActorDead)
    {
        deadActorStack.push(param.c2.list[0]);
    }
    for (auto trigger : filterTriggers)
    {
        if (trigger->IsInvalid())
        {
            continue;
        }
        if (!trigger->CheckEvtParams(param))
        {
            continue;
        }
        if (!trigger->CheckProbability())
        {
            continue;
        }
        if (!trigger->CheckRoundTimesLimit())
        {
            continue;
        }
        if (!trigger->CheckAllConditions(param))
        {
            continue;
        }
        
        if (param.evtType == EEventType::NearDeath)
        {
            param.hasResult = true;
            param.c1.list[0]->ReAlive();
        }
        
        triggerStack.push(trigger->GetTriggerID());
        trigger->ExecuteActions(param);
        //if the actions not contains DoEffect type,it means the trigger id is not contains in cmd
        if(!trigger->HasEffect()){
            ActorPtr caster = param.c1.list[0];
            if(caster == nullptr){
                caster = param.c2.list[0];
            }
            if(caster != nullptr){
                EffectCmd *subEffect = CreateSubEffect(caster);
                if (subEffect == NULL)
                {
                    if (param.c1.list[0]->battleField->inited)
                    {
                        error("FillDamageCmd subEffect == NULL");
                    }
                    return;
                }
                subEffect->set_only_trigger(true);
                subEffect->set_caster_id(caster->actorId);
                subEffect->set_triggercaster_id(caster->actorId);
                subEffect->set_trigger_id(trigger->GetTriggerID());
                subEffect->set_triggerevt(EEventType::ComingCauseDamage);
                subEffect->set_target_id(caster->actorId);
            }
        }
        triggerStack.pop();
    }
    if (param.evtType == EEventType::SomeActorDead)
    {
        deadActorStack.pop();
    }
    eventStack.pop();
    if (eventStack.size() == 0)
    {
        CleanInvalidTriggers();
    }
    
    
}

void BattleTriggerManager::CleanInvalidTriggers()
{
    if (invalidTriggerList.size() > 0)
    {
        TriggerList::iterator it = invalidTriggerList.begin();
        for (; it != invalidTriggerList.end();++it)
        {
            BattleTrigger* trigger = *it;
            auto & triggerList = triggerListArray[trigger->GetEventType()];
            TriggerList::iterator itm = triggerList.begin();
            for (;itm != triggerList.end(); ++itm)
            {
                if (*itm == trigger)
                {
                    trigger->OnDestroy();
                    triggerFactorMgr.DestroyInst(trigger->GetEventType(), trigger);
                    triggerList.erase(itm);
                    break;
                }
            }
        }
        invalidTriggerList.clear();
    }
}

TriggerAction* BattleTriggerManager::CreateAction(TriggerActionType aType)
{
    return actionFactoryMgr.CreateInst(aType);
}

void BattleTriggerManager::DestroyAction(TriggerActionType aType, TriggerAction *pAction)
{
    actionFactoryMgr.DestroyInst(aType, pAction);
}

TriggerCondition* BattleTriggerManager::CreateCondition(TriggerCondType aType)
{
    return conditionFactoryMgr.CreateInst(aType);
}

void BattleTriggerManager::DestroyCondition(TriggerCondType aType, TriggerCondition *pCondition)
{
    conditionFactoryMgr.DestroyInst(aType, pCondition);
}

EEventType BattleTriggerManager::GetTopEvtType()
{
    if (eventStack.size() > 0)
    {
        return eventStack.top();
    }
    return EEventType::Invalid;
}

int BattleTriggerManager::GetTopTriggerID()
{
    if (triggerStack.size() > 0)
    {
        return triggerStack.top();
    }
    return 0;
}

Actor* BattleTriggerManager::GetTopDeadActor()
{
    if (deadActorStack.size() > 0)
    {
        return deadActorStack.top();
    }
    return NULL;
}


void BattleTriggerManager::ResetTimesLimitPerRound()
{
    for (auto & triggerList : triggerListArray)
    {
        for (auto trigger : triggerList)
        {
            if (trigger->IsInvalid())
            {
                int actorID = -1;
                if (trigger->GetOriginBuff() != NULL)
                {
                    actorID = trigger->GetOriginBuff()->owner->actorId;
                }
                error("trigger is Invalid when RoundReset! ID:[%d] Type[%d] ActorID[%d]", trigger->GetInstID(), trigger->GetEventType(), actorID);
                continue;
            }
            trigger->ResetTimesLimit();
        }
    }
}

CheckTriggerPrivate* BattleTriggerManager::CreatePrivateChecker(PrivateCheckType aType)
{
    return privateCheckerFactoryMgr.CreateInst(aType);
}

void BattleTriggerManager::DestroyPrivateChecker(PrivateCheckType aType, CheckTriggerPrivate* pPrivateChecker)
{
    privateCheckerFactoryMgr.DestroyInst(aType, pPrivateChecker);
}

#include "BattleTrigger.h"
#include "ActorModifierSub.h"
#include "TriggerAction.h"
#include "TriggerCondition.h"
#include "Actor.h"
#include "Type.h"
#include "BattleField.h"
#include "PrivateChecker.h"

BattleTrigger::BattleTrigger()
    :pOriginBuff(NULL)
    , pTriggerElemFactory(NULL)
    , triggerMeta(NULL)
    , invalid(false)
    , instanceID(0)
    , triggerTimes(0)
{

}

BattleTrigger::~BattleTrigger()
{

}

void BattleTrigger::OnDestroy()
{
    pOriginBuff = nullptr;
    if (pPrivateChecker != NULL)
    {
        pTriggerElemFactory->DestroyPrivateChecker(pPrivateChecker->GetType(), pPrivateChecker);
        pPrivateChecker = NULL;
    }
    ActionList::iterator it = actionList.begin();
    for (; it != actionList.end();++it)
    {
        pTriggerElemFactory->DestroyAction((*it)->GetActionType(), (*it));
    }
    actionList.clear();

    ConditionList::iterator ii = conditionList.begin();
    for (; ii != conditionList.end();++ii)
    {
        pTriggerElemFactory->DestroyCondition((*ii)->GetConditionType(), (*ii));
    }
    conditionList.clear();
}

void BattleTrigger::InitTrigger(TriggerMeta *pMeta, int instID, ITriggerElemFactory *pFactory)
{
    pOriginBuff = NULL;
    invalid = false;
    triggerMeta = pMeta;
    instanceID = instID;
    if (conditionList.size() > 0)
    {
        error("conditionList.size() != 0");
        conditionList.clear();
    }
    if (actionList.size() > 0)
    {
        error("actionList.size() != 0");
        actionList.clear();
    }
    pTriggerElemFactory = pFactory;
    if (pTriggerElemFactory)
    {
        CreatePrivateChecker();

        for (int i = 0; i < MAX_TRIGGER_ACTION_COUNT; i++)
        {
            int conditionID = triggerMeta->ConditionId[i];
            if (conditionID == NULL || conditionID == 0)
            {
                break;
            }
            FightCondMeta *condMeta = (FightCondMeta*)FightCondMetaSet::instance()->Get(conditionID);
            if (condMeta == NULL)
            {
                error("FightCondMeta [%d] is not exist", conditionID);
                continue;
            }
            TriggerCondition* pCondition = pTriggerElemFactory->CreateCondition(condMeta->Condition);
            if (pCondition == NULL)
            {
                error("CreateCondition failed condiType[%d]", condMeta->Condition);
                continue;
            }
            pCondition->Init(this,condMeta);
            conditionList.push_back(pCondition);
        }

        for (int i = 0; i < MAX_TRIGGER_ACTION_COUNT; i++)
        {
            TriggerActionType aType = triggerMeta->ActionType[i];
            if (aType == TriggerActionType::Invalid)
            {
                break;
            }
            TriggerAction *pAction = pTriggerElemFactory->CreateAction(aType);
            if (pAction == NULL)
            {
                error("CreateAction failed actionType[%d]", aType);
                continue;
            }
            pAction->Init(this, aType, triggerMeta->ActionEcount[i], triggerMeta->ActionParam[i], triggerMeta->ActionParam1[i], triggerMeta->ActionParam2[i],triggerMeta->ActionParam3[i],triggerMeta->ActionParam4[i],triggerMeta->ActionReset[i], triggerMeta->ActionCaster[i]);
            pAction->InitDelayExec(triggerMeta->ActionDelay);
            actionList.push_back(pAction);
        }
    }
}

void BattleTrigger::ExecuteActions(TriggerParam& param)
{
    triggerTimes++;
    if (IsInvalid())
    {
        return;
    }
    ActionList::iterator it = actionList.begin();
    for (; it != actionList.end(); ++it)
    {
        if (IsInvalid())
        {
            break;
        }
        TriggerAction* action = *it;
        if (!action->NextExceute(param))
        {
            break;
        }
    }
}

bool BattleTrigger::HasEffect()
{
    ActionList::iterator it = actionList.begin();
    for (; it != actionList.end(); ++it)
    {
        if (IsInvalid())
        {
            return false;
        }
        TriggerAction* action = *it;
        if (action->GetActionType() == TriggerActionType::DoEffect){
            return true;
        }
    }
    return false;
}

bool BattleTrigger::CheckAllConditions(const TriggerParam& param)
{
    if (IsInvalid())
    {
        return false;
    }
    ConditionList::iterator it = conditionList.begin();
    for (; it != conditionList.end();++it)
    {
        TriggerCondition* condition = *it;
        if (!condition->CheckCondition(param))
        {
            return false;
        }
    }
    return true;
}

void BattleTrigger::InitOriginBuff(ActorModifierSub* buff)
{
    pOriginBuff = buff;
}

bool BattleTrigger::CheckProbability()
{
    if (triggerMeta == NULL)
    {
        return false;
    }
    if (pOriginBuff == NULL)
    {
        return false;
    }
    auto level = pOriginBuff->modLevel;
    if (level >= 1)
    {
        level--;
    }
    auto rate = pOriginBuff->owner->battleField->random.RandForWidth(100) * 100;
    auto triggerChance = triggerMeta->TriggerChance[0] + triggerMeta->TriggerChance[1] * level;
    if (rate >= triggerChance)
    {
        return false;
    }
    return true;
}

bool BattleTrigger::CheckRoundTimesLimit()
{
    if (triggerMeta != NULL)
    {
        if (triggerTimes < triggerMeta->TriggerTimes || triggerMeta->TriggerTimes == 0)
        {
            return true;
        }
    }
    return false;
}

void BattleTrigger::ResetTimesLimit()
{
    triggerTimes = 0;
}

bool BattleTrigger::CheckEvtParams(const TriggerParam& param)
{
    return true;
}

const TriggerMeta* BattleTrigger::GetMeta()
{
    return triggerMeta;
}

bool BattleTrigger::CheckPrivate(const TriggerParam& param)
{
    if (pPrivateChecker != NULL)
    {
        return pPrivateChecker->CheckPrivate(this, param);
    }
    return false;
}

bool BattleCastSkillTrigger::CheckEvtParams(const TriggerParam& param)
{
    if (triggerMeta != NULL)
    {
        for (int i = 0; i < MAX_TRIGGER_EVENT_PARAM_COUNT; i++)
        {
            if (triggerMeta->EventParam1[i] == 0)
            {
                if (i == 0)
                {
                    return true;
                }
                else
                {
                    break;
                }
            }
            if (triggerMeta->EventParam1[i] == (int)param.skillType)
            {
                return true;
            }
        }
    }
    return false;
}

bool BattleSufferSkillTrigger::CheckEvtParams(const TriggerParam& param)
{
    if (triggerMeta != NULL)
    {
        for (int i = 0; i < MAX_TRIGGER_EVENT_PARAM_COUNT; i++)
        {
            if (triggerMeta->EventParam1[i] == 0)
            {
                if (i == 0)
                {
                    return true;
                }
                else
                {
                    break;
                }
            }
            if (triggerMeta->EventParam1[i] == (int)param.skillType)
            {
                return true;
            }
        }
    }
    return false;
}

bool BattleGainBuffTrigger::CheckEvtParams(const TriggerParam& param)
{
    if (triggerMeta != NULL && param.pBuff != NULL)
    {
        for (int i = 0; i < MAX_TRIGGER_EVENT_PARAM_COUNT; i++)
        {
            if (triggerMeta->EventParam2[i] == 0)
            {
                if (i == 0)
                {
                    return true;
                }
                break;
            }
            int targetGroupID = triggerMeta->EventParam2[i];
            for (auto groupID : param.pBuff->modMeta->GroupId)
            {
                if (groupID == 0)
                {
                    break;
                }
                if (groupID == targetGroupID)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool BattleBuffOverTrigger::CheckEvtParams(const TriggerParam& param)
{
    if (triggerMeta != NULL && param.pBuff != NULL)
    {
        if (triggerMeta->EventParam2[0] == 0)
        {
            return true;
        }
        int overType = triggerMeta->EventParam1[0];
        if (overType == 1 && param.normalBuffOver)
        {
            return false;
        }
        if (overType == 2 && !param.normalBuffOver)
        {
            return false;
        }
        for (int i = 0; i < MAX_TRIGGER_EVENT_PARAM_COUNT; i++)
        {
            int targetGroupID = triggerMeta->EventParam2[i];
            for (auto groupID : param.pBuff->modMeta->GroupId)
            {
                if (groupID == 0)
                {
                    break;
                }
                if (groupID == targetGroupID)
                {
                    return true;
                }
            }
        }
    }
    return false;
}


bool RoundSmallBeginTrigger::CheckEvtParams(const TriggerParam& param)
{
    if (triggerMeta != NULL)
    {
        for (int i = 0; i < MAX_TRIGGER_EVENT_PARAM_COUNT; i++)
        {
            auto sideIndex = triggerMeta->EventParam1[i];
            auto areaIndex = pOriginBuff->owner->areaIndex;
            if(sideIndex == 1){
                if (param.intParam == areaIndex)
                {
                    return true;
                }
            }
            else if(sideIndex == 2)
            {
                if (param.intParam != areaIndex)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool RoundSmallEndTrigger::CheckEvtParams(const TriggerParam& param)
{
    if (triggerMeta != NULL)
    {
        for (int i = 0; i < MAX_TRIGGER_EVENT_PARAM_COUNT; i++)
        {
            auto sideIndex = triggerMeta->EventParam1[i];
            auto areaIndex = pOriginBuff->owner->areaIndex;
            if(sideIndex == 1){
                if (param.intParam == areaIndex)
                {
                    return true;
                }
            }
            else if(sideIndex == 2)
            {
                if (param.intParam != areaIndex)
                {
                    return true;
                }
            }
        }
    }
    return false;
}


#include "PrivateChecker.h"
#include "BattleTrigger.h"
#include "ActorModifierSub.h"
#include "TriggerMeta.h"

bool CheckTriggerComPrivate::CheckCommonPrivate(BattleTrigger* trigger, bool & outPass)
{
    if (trigger != NULL)
    {
        if (trigger->IsInvalid())
        {
            outPass = false;
            return false;
        }
        if (trigger->GetOriginBuff() == NULL)
        {
            outPass = false;
            return false;
        }
        if (trigger->GetMeta() == NULL)
        {
            outPass = false;
            return false;
        }
        if (trigger->GetMeta()->TriggerType == ETriggerType::EPublic)
        {
            outPass = true;
            return false;
        }
    }
    return true;
}

bool CheckTriggerPrivateC1::CheckPrivate(BattleTrigger* trigger, const TriggerParam& param)
{
    bool lastRet = false;
    if (!CheckCommonPrivate(trigger, lastRet))
    {
        return lastRet;
    }
    Actor *c1 = param.c1.list[0];
    if (trigger->GetOriginBuff() != NULL)
    {
        if (c1 != trigger->GetOriginBuff()->owner)
        {
            return false;
        }
    }
    return true;
}

bool CheckTriggerPrivateC1List::CheckPrivate(BattleTrigger* trigger, const TriggerParam& param)
{
    bool lastRet = false;
    if (!CheckCommonPrivate(trigger, lastRet))
    {
        return lastRet;
    }
    if (trigger->GetOriginBuff() != NULL)
    {
        Actor* actor = trigger->GetOriginBuff()->owner;
        if (actor != NULL)
        {
            for (int i = 0; i < ActorList::Size; i++)
            {
                if (param.c1.list[i] == actor)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool CheckTriggerPrivateNeglect::CheckPrivate(BattleTrigger* trigger, const TriggerParam& param)
{
    return true;
}

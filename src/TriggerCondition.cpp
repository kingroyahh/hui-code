#include "TriggerCondition.h"
#include "BattleTrigger.h"
#include "ActorModifier.h"
#include "ActorModifierSub.h"
#include "Actor.h"
#include "BattleField.h"
#include "BattleArea.h"
#include "ActorDatas.h"
#include "ActorSkillSub.h"
TriggerCondition::TriggerCondition()
    :conType(0)
    , pParentTrigger(NULL)
    , conParam(0)
    , conTarget(0)
{

}

TriggerCondition::~TriggerCondition()
{

}

void TriggerCondition::Init(BattleTrigger* pParent,FightCondMeta *condmeta)
{
    pParentTrigger = pParent;
    condMeta = condmeta;
    conType = condMeta->Condition;
    conParam = condMeta->ConParam;
    conTarget = condMeta->ConTarget;
}

Actor* TriggerCondition::GetTargetActor(const TriggerParam & param)
{
    return GetTargetActorByTarget(param,conTarget);
}

Actor* TriggerCondition::GetTargetActorByTarget(const TriggerParam & param,TriggerConTarget cont)
{
    switch (cont)
    {
        case  TriggerConTarget::C1:
        {
            return param.c1.list[0];
        }
        case  TriggerConTarget::C2:
        {
            return param.c2.list[0];
        }
        case  TriggerConTarget::BuffCaster:
        {
            ActorModifierSub *pBuff = pParentTrigger->GetOriginBuff();
            if (pBuff != NULL)
            {
                return pBuff->casterIn;
            }
            break;
        }
        case  TriggerConTarget::BuffOwner:
        {
            ActorModifierSub *pBuff = pParentTrigger->GetOriginBuff();
            if (pBuff != NULL)
            {
                return pBuff->owner;
            }
            break;
        }
    }
    return NULL;
}

int TriggerCondition::GetTargetValue(const TriggerParam & param)
{
    return GetTargetValueByType(param,conTarget);
}

int TriggerCondition::GetTargetValueByType(const TriggerParam & param,TriggerConTarget cont)
{
    switch (cont)
    {
        case  TriggerConTarget::R1:
        {
            return param.r1;
        }
        case  TriggerConTarget::R2:
        {
            return param.r2;
        }
        case  TriggerConTarget::R3:
        {
            return param.r3;
        }
        case  TriggerConTarget::R4:
        {
            return param.r4;
        }
        case  TriggerConTarget::R5:
        {
            return param.r5;
        }
        case  TriggerConTarget::R6:
        {
            return param.r6;
        }
        case  TriggerConTarget::R7:
        {
            return param.r7;
        }
        case  TriggerConTarget::R8:
        {
            return param.r8;
        }
        case  TriggerConTarget::ConParam:
        {
            return condMeta->ConParam;
        }
        case  TriggerConTarget::RoundCount:
        {
            ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
            if (pBuff != NULL)
            {
                Actor* pOwner = pBuff->owner;
                return pOwner->battleField->roundCount;
            }
            error("buff no owener %d",pParentTrigger->GetMeta()->id);
            return 0;
        }
        case  TriggerConTarget::R4TotalPercent:
        {
            ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
            if (pBuff != NULL)
            {
                Actor* pOwner = pBuff->casterIn;
                return param.r4/pOwner->actorDatas.hpMax*10000;
            }
            error("buff no target %d",pParentTrigger->GetMeta()->id);
            return 0;
        }
        default:
            break;
    }
    return 0;
}

bool CondHpPercentGreaterThan::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pTarget != NULL)
    {
        int curHp = pTarget->actorDatas.hp;
        int maxHp = pTarget->actorDatas.hpMax;
        if (maxHp <= 0)
        {
            maxHp = 1;
        }
        float targetPercent = conParam / 10000.0f;
        return (curHp * 1.0f / maxHp) > targetPercent;
    }
    return false;
}

bool CondHpPercentLessEqual::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pTarget != NULL)
    {
        int curHp = pTarget->actorDatas.hp;
        int maxHp = pTarget->actorDatas.hpMax;
        if (maxHp <= 0)
        {
            maxHp = 1;
        }
        float targetPercent = conParam / 10000.0f;
        return (curHp * 1.0f / maxHp) <= targetPercent;
    }
    return false;
}

bool CondXpPercentGreateThan::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pTarget != NULL)
    {
        int curXp = pTarget->actorDatas.xp;
        int maxXp = pTarget->actorDatas.xpMax;
        if (maxXp <= 0)
        {
            maxXp = 1;
        }
        float targetPercent = conParam / 10000.0f;
        return (curXp * 1.0f / maxXp) > targetPercent;
    }
    return false;
}

bool CondXpPercentLessEqual::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pTarget != NULL)
    {
        int curXp = pTarget->actorDatas.xp;
        int maxXp = pTarget->actorDatas.xpMax;
        if (maxXp <= 0)
        {
            maxXp = 1;
        }
        float targetPercent = conParam / 10000.0f;
        return (curXp * 1.0f / maxXp) <= targetPercent;
    }
    return false;
}

bool CondTargetContainsBuffGroup::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pTarget != NULL)
    {
        if (pTarget->actorMod != NULL)
        {
            return pTarget->actorMod->GetModCntByGroupID(conParam) > 0;
        }
    }
    return false;
}

bool CondTargetIsEnemy::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            Actor* pOwner = pBuff->owner;
            if (pOwner != NULL && pTarget != NULL)
            {
                return pOwner->areaIndex != pTarget->areaIndex;
            }
        }
    }
    return false;
}

bool CondTargetIsAlly::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            Actor* pOwner = pBuff->owner;
            if (pOwner != NULL && pTarget != NULL)
            {
                return pOwner->areaIndex == pTarget->areaIndex;
            }
        }
    }
    return false;
}

bool CondTargetIsSelf::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            Actor* pOwner = pBuff->owner;
            if (pOwner != NULL && pTarget != NULL)
            {
                return pOwner == pTarget;
            }
        }
    }
    return false;
}

bool CondTargetIsCaster::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            Actor* pCaster = pBuff->casterIn;
            if (pCaster != NULL && pTarget != NULL)
            {
                return pCaster == pTarget;
            }
        }
    }
    return false;
}

bool CondTargetIsOwner::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            Actor* pOwner = pBuff->owner;
            if (pOwner != NULL && pTarget != NULL)
            {
                return pOwner == pTarget;
            }
        }
    }
    return false;
}

bool CondResult1EqualParam::CheckCondition(const TriggerParam & param)
{
    return conParam == param.r1;
}

bool CondResult2EqualParam::CheckCondition(const TriggerParam & param)
{
    return conParam == param.r2;
}

bool CondResult3EqualParam::CheckCondition(const TriggerParam & param)
{
    return conParam == param.r3;
}

bool CondResult4EqualParam::CheckCondition(const TriggerParam & param)
{
    return conParam == param.r4;
}

bool CondResult5EqualParam::CheckCondition(const TriggerParam & param)
{
    return conParam == param.r5;
}
bool CondResult6EqualParam::CheckCondition(const TriggerParam & param)
{
    return conParam == param.r6;
}
bool CondResult7EqualParam::CheckCondition(const TriggerParam & param)
{
    return conParam == param.r7;
}
bool CondResult8ContainParam::CheckCondition(const TriggerParam & param)
{
    error("CondResult8ContainParam %d",param.r8);
    if(param.r8 <= 0){
        return false;
    }
    const ModifierMeta* modMeta = ModifierMetaSet::instance()->Get(param.r8);
    if (modMeta != NULL_PTR)
    {
        return false;
    }
    auto group = modMeta->GroupId;
    int length = sizeof(group);
    for (int i =0; i<length; i++) {
        int groupId = group[i];
        error("groupId %d conParam %d ",groupId,conParam);
        if(groupId == conParam)
            return true;
    }
    return false;
}

bool CondTargetBelongToFrontRowParam::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pTarget != NULL)
    {
        return pTarget->posIndex < 3;
    }
    return false;
}

bool CondTargetBelongToBackRowParam::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pTarget != NULL)
    {
        return pTarget->posIndex >= 3;
    }
    return false;
}

bool CondTargetNotEqualParam::CheckCondition(const TriggerParam & param)
{
    int targetValue = GetTargetValue(param);
    return targetValue != conParam;
}

bool CondTargetLessEqualParam::CheckCondition(const TriggerParam & param)
{
    int targetValue = GetTargetValue(param);
    return targetValue <= conParam;
}

bool CondTargetGreatThanParam::CheckCondition(const TriggerParam & param)
{
    int targetValue = GetTargetValue(param);
    return targetValue > conParam;
}


bool ConTargetBelongToTrggerActorColumn::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            Actor* pOwner = pBuff->owner;
            if (pOwner != NULL && pTarget != NULL)
            {
                auto ownerColumn = pOwner->posIndex % 3;
                auto targetColumn = pTarget->posIndex % 3;
                if (ownerColumn == targetColumn)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ConTargetBelongToTrggerActorRow::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            Actor* pOwner = pBuff->owner;
            if (pOwner != NULL && pTarget != NULL)
            {
                auto ownerRow = pOwner->posIndex / 3;
                auto targetRow = pTarget->posIndex / 3;
                if (ownerRow == targetRow && pOwner->areaIndex == pTarget->areaIndex)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool CurrentEnemyCountEqualToConParam::CheckCondition(const TriggerParam & param)
{
    if (conParam > 0)
    {
        if (pParentTrigger != NULL)
        {
            ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
            if (pBuff != NULL)
            {
                Actor* pOwner = pBuff->owner;
                auto battleFiele = pOwner->battleField;
                if (battleFiele != nullptr)
                {
                    BattleArea* area = battleFiele->GetEnemyArea(battleFiele->lastAttackArea);
                    if (area != nullptr)
                    {
                        int aliveEnemyCount = 0;
                        for (auto & actor : area->actors)
                        {
                            if (actor != nullptr && actor->IsAlive())
                            {
                                aliveEnemyCount++;
                            }
                        }
                        if (aliveEnemyCount == conParam)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool CurrentEnemyContainsBuffGroup::CheckCondition(const TriggerParam & param)
{
    if (conParam > 0)
    {
        if (pParentTrigger != NULL)
        {
            ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
            if (pBuff != NULL)
            {
                Actor* pOwner = pBuff->owner;
                auto battleFiele = pOwner->battleField;
                if (battleFiele != nullptr)
                {
                    BattleArea* area = battleFiele->GetEnemyArea(battleFiele->lastAttackArea);
                    if (area != nullptr)
                    {
                        for (auto & actor : area->actors)
                        {
                            if (actor != nullptr && actor->IsAlive())
                            {
                                if (actor->actorMod->GetModCntByGroupID(conParam) > 0)
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool CondEqualtoFirstAttackSiseParam::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pTarget != NULL)
    {
        if (conParam == 0)
        {
            return pTarget->battleField->firstArea == pTarget->areaIndex;
        }
        else
        {
            return pTarget->battleField->firstTargetArea == pTarget->areaIndex;
        }
    }
    return false;
}

bool OwnerHpPercentGreaterThanContargetParam::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            Actor* pOwner = pBuff->owner;
            if (pOwner != NULL && pTarget != NULL)
            {
                return pOwner->actorDatas.GetHpPercent() > pTarget->actorDatas.GetHpPercent();
            }
        }
    }
    return false;
}

bool OwnerHpPercentLessEqualContargetParam::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActor(param);
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            Actor* pOwner = pBuff->owner;
            if (pOwner != NULL && pTarget != NULL)
            {
                return pOwner->actorDatas.GetHpPercent() <= pTarget->actorDatas.GetHpPercent();
            }
        }
    }
    return false;
}

bool MainTargetColumnContainOneParam::CheckCondition(const TriggerParam & param)
{
    if (param.evtType == EEventType::CauseSkill)
    {
        if (param.pEffect != nullptr)
        {
            auto mainTarget = param.pEffect->GetMainTarget();
            if (mainTarget != nullptr)
            {
                auto mainTargetPos = mainTarget->posIndex;
                int columnPartnerPos = -1;
                if (mainTargetPos < MAX_COLUMN_SIZE)
                {
                    columnPartnerPos = mainTargetPos + MAX_COLUMN_SIZE;
                }
                else
                {
                    columnPartnerPos = mainTargetPos - MAX_COLUMN_SIZE;
                }
                auto targetArea = mainTarget->battleField->GetOwnArea(mainTarget->areaIndex);
                if (targetArea != nullptr)
                {
                    auto partnerActor = targetArea->GetActorByPos(columnPartnerPos);
                    if (partnerActor == nullptr)
                    {
                        return true;
                    }
                    if (!partnerActor->IsAlive())
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


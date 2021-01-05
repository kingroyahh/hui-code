#include "Actor.h"
#include "BattleArea.h"
#include "BattleField.h"
#include "ActorModifier.h"
#include "EffectMeta.h"
#include "Log.h"
#include "Random.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "BattleModeMeta.h"
#include "EffectUtil.h"
#include "ModifierUtils.h"

#define ExistReturn(targetPos) if (actors[targetPos] != NULL_PTR && actors[targetPos]->IsAlive())\
{\
    return actors[targetPos];\
}


BattleArea::BattleArea(int index, BattleFieldPtr bfIn)
{
    bf = bfIn;
    areaIndex = index;
    curPos = 0;
    damageAll = 0;
    memset(actors, 0, sizeof(ActorPtr)*BATTLE_POS_MAX);
}

BattleArea::~BattleArea()
{
//    error("~battlearea");
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if(actors[i] != 0)
        {
            delete actors[i];
            actors[i] = NULL;
        }
    }
}

bool BattleArea::HasAlive()
{
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL && actors[i]->IsAlive())
        {
            return true;
        }
    }
    return false;
}

int BattleArea::AliveCount()
{
    auto aliveCount = 0;
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL && actors[i]->IsAlive())
        {
            aliveCount ++;
        }
    }
    return aliveCount;
}

bool BattleArea::AddActor(ActorPtr actor)
{
    if (actors[actor->posIndex])
    {
        return false;
    }
    bf->AddActorInfoCmd(actor);
    actors[actor->posIndex] = actor;
    return true;
}

bool BattleArea::ActorIsExist(int skillActorID)
{
    TRACE("check combine partner id : %d", skillActorID);
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL && actors[i]->IsAlive() && actors[i]->metaData->SkillActorId == skillActorID)
        {
            return true;
        }
    }
    return false;
}

ActorPtr BattleArea::GetActorByPos(int posIndex)
{
    if (actors[posIndex] != NULL && actors[posIndex]->IsAlive())
    {
        return actors[posIndex];
    }
    return NULL;
}

ActorPtr BattleArea::GetActorById(int tempId)
{
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] && actors[i]->IsAlive() && actors[i]->actorId == tempId)
        {
            return actors[i];
        }
    }
    return NULL_PTR;
}

void BattleArea::ResetAction()
{
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL_PTR)
        {
            actors[i]->actionCount = 0;
        }
    }
}

bool BattleArea::FrontRowIsHaveAlive()
{
    bool isExist = false;
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] && actors[i]->IsAlive() && actors[i]->posIndex < MAX_COLUMN_SIZE)
        {
            isExist = true;
            break;
        }
    }
    return isExist;
}

void BattleArea::GetRandomTargets(int maxSize, ActorPtr *targetList)
{
    TRACE("BattleArea::GetRandomTargets maxSize[%d]", maxSize);
    int index = 0;
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL && actors[i]->IsAlive())
        {
            targetList[index] = actors[i];
            index++;
        }
    }

    if (index > maxSize)
    {
        int numToRm = index - maxSize;
        int intArry[BATTLE_POS_MAX] = { 0 };
        for (int i = 0; i < BATTLE_POS_MAX; ++i)
        {
            intArry[i] = i;
        }
        bf->random.RandIntArray(intArry, index);
        for (int i = 0; i < numToRm; ++i)
        {
            targetList[intArry[i]] = NULL;
        }
    }
}

void BattleArea::GetOtherFirstTargets(int maxSize, ActorPtr *targetList,int myArea, int posIndex)
{
    TRACE("BattleArea::GetOtherFirstTargets maxSize[%d] myArea[%d] posIndex[%d]", maxSize, myArea, posIndex);
    int index = 0;
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL && actors[i]->IsAlive())
        {
            if (myArea == areaIndex)
            {
                if (actors[i]->posIndex != posIndex)
                {
                    targetList[index] = actors[i];
                    index++;
                }
            }
            else
            {
                targetList[index] = actors[i];
                index++;
            }
        }
    }
    if (index > maxSize)
    {
        int numToRm = index - maxSize;
        
        int intArry[BATTLE_POS_MAX];
        
        for (int i = 0; i < BATTLE_POS_MAX; ++i)
        {
            intArry[i] = i;
        }
        
        bf->random.RandIntArray(intArry, index);
        
        for (int i = 0; i < numToRm; ++i)
        {
            targetList[intArry[i]] = NULL;
        }
        return;
    }
    if (index < maxSize)
    {
        for (int i = 0; i < BATTLE_POS_MAX; ++i)
        {
            if (actors[i] != NULL && actors[i]->IsAlive() && actors[i]->posIndex == posIndex)
            {
                targetList[index] = actors[i];
                break;
            }
        }
    }
}

void BattleArea::GetAllTargets(ActorPtr *targetList)
{
    TRACE("BattleArea::GetAllTargets");
    int index = 0;
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL_PTR && actors[i]->IsAlive())
        {
            targetList[index] = actors[i];
            index++;
        }
    }
}

ActorPtr BattleArea::GetLowestHpTarget()
{
    TRACE("BattleArea::GetLowestHpTarget");
    ActorPtr target = NULL_PTR;
    ActorPtr tempTargets[6] = { 0 };
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL_PTR && actors[i]->IsAlive())
        {
            if (target == NULL_PTR)
            {
                target = actors[i];
            }
            else if( 0 < actors[i]->actorDatas.hp && actors[i]->actorDatas.GetHpPercent() < target->actorDatas.GetHpPercent())
            {
                target = actors[i];
            }
        }
    }
    int index = 0;
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL_PTR && actors[i]->IsAlive())
        {
            if(0 < actors[i]->actorDatas.hp && actors[i]->actorDatas.GetHpPercent() == target->actorDatas.GetHpPercent())
            {
                tempTargets[index] = actors[i];
                index++;
            }
        }
    }
    int pos = bf->random.RandForWidthFromZero(index);
    target = tempTargets[pos];
    return target;
}

ActorPtr BattleArea::GetHighestXpTarget()
{
    TRACE("BattleArea::GetHighestXpTarget");
    ActorPtr target = NULL_PTR;
    ActorPtr tempTargets[6];
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL_PTR && actors[i]->IsAlive())
        {
            if (target == NULL_PTR)
            {
                target = actors[i];
            }
            else if( 0 < actors[i]->actorDatas.hp && actors[i]->actorDatas.xp > target->actorDatas.xp)
            {
                target = actors[i];
            }
        }
    }
    int index = 0;
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL_PTR && actors[i]->IsAlive())
        {
            if(0 < actors[i]->actorDatas.hp && actors[i]->actorDatas.xp == target->actorDatas.xp)
            {
                tempTargets[index] = actors[i];
                index++;
            }
        }
    }
    int pos = bf->random.RandForWidthFromZero(index);
    target = tempTargets[pos];
    return target;
}

ActorPtr BattleArea::GetLowestXpTarget()
{
    ActorPtr target = NULL_PTR;
    ActorPtr tempTargets[6];
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL_PTR && actors[i]->IsAlive())
        {
            if (target == NULL_PTR)
            {
                target = actors[i];
            }
            else if( 0 < actors[i]->actorDatas.hp && actors[i]->actorDatas.xp <= target->actorDatas.xp)
            {
                target = actors[i];
            }
        }
    }
    int index = 0;
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL_PTR && actors[i]->IsAlive())
        {
            if(0 < actors[i]->actorDatas.hp && actors[i]->actorDatas.xp == target->actorDatas.xp)
            {
                tempTargets[index] = actors[i];
                index++;
            }
        }
    }
    int pos = bf->random.RandForWidthFromZero(index);
    target = tempTargets[pos];
    return target;
}


ActorPtr BattleArea::GetActorByCAR(int column, int row)
{
    TRACE("BattleArea::GetActorByCAR column[%d],row[%d]", column, row);
    //TRICK
    if (row > 1)
    {
        return NULL_PTR;
    }
    int posIndex = column + row * MAX_ROW_SIZE;
    return actors[posIndex];
}

void BattleArea::GetOneColumnTargets(int posIndex, ActorPtr *targetList)
{
    TRACE("BattleArea::GetOneColumnTargets posIndex[%d]", posIndex);
    if (actors[posIndex] != NULL && actors[posIndex]->IsAlive())
    {
        targetList[0] = actors[posIndex];
    }
    if (posIndex < 3)
    {
        int secPos = posIndex + 3;
        if (actors[secPos] != NULL && actors[secPos]->IsAlive())
        {
            targetList[1] = actors[secPos];
        }
    } 
    else
    {
        int secPos = posIndex - 3;
        if (actors[secPos] != NULL && actors[secPos]->IsAlive())
        {
            targetList[1] = actors[secPos];
        }
    }
}

void BattleArea::GetCrossTargets(int posIndex, ActorPtr *targetList)
{
    TRACE("BattleArea::GetCrossTargets posIndex[%d]", posIndex);
    targetList[0] = actors[posIndex];
    ActorPtr target = NULL;

    int column = posIndex % MAX_COLUMN_SIZE;
    int row = posIndex / MAX_ROW_SIZE;
    int index = 1;

    if (column - 1 >= 0)
    {
        target = GetActorByCAR(column - 1, row);
        if (target != NULL && target->IsAlive())
        {
            targetList[index] = target;
            index++;
        }
    }

    if (column + 1 < MAX_COLUMN_SIZE)
    {
        target = GetActorByCAR(column + 1, row);
        if (target != NULL && target->IsAlive())
        {
            targetList[index] = target;
            index++;
        }
    }

    if (row + 1 < MAX_ROW_SIZE)
    {
        target = GetActorByCAR(column, row + 1);
        if (target != NULL_PTR && target->IsAlive())
        {
            targetList[index] = target;
            index++;
        }
    }
}
void BattleArea::GetRowTargets(int row, ActorPtr *targetList)
{
    TRACE("BattleArea::GetRowTargets row[%d]", row);
    int index = 0;
    for (int i = 0; i < MAX_COLUMN_SIZE; ++i)
    {
        ActorPtr target = GetActorByCAR(i, row);
        if (target != NULL && target->IsAlive())
        {
            targetList[index] = target;
            index++;
        }
    }

    if (index == 0)
    {
        if (row == 0)
        {
            row = 1;
        }
        else
        {
            row = 0;
        }
        for (int i = 0; i < MAX_COLUMN_SIZE; ++i)
        {
            ActorPtr target = GetActorByCAR(i, row);
            if (target != NULL && target->IsAlive())
            {
                targetList[index] = target;
                index++;
            }
        }
    }
}
void BattleArea::GetTargetAndRlTargets(int posIndex, ActorPtr *targetList)
{
    TRACE("BattleArea::GetTargetAndRlTargets posIndex[%d]", posIndex);
    int index = 0;
    int column = posIndex % MAX_COLUMN_SIZE;
    int row = posIndex / MAX_ROW_SIZE;
    ActorPtr target = GetActorByPos(posIndex);
    if (target != NULL && target->IsAlive())
    {
        targetList[index] = target;
        index++;
    }

    if (column - 1 >= 0)
    {
        target = GetActorByCAR(column - 1, row);
        if (target != NULL && target->IsAlive())
        {
            targetList[index] = target;
            index++;
        }
    }

    if (column + 1 < MAX_COLUMN_SIZE)
    {
        target = GetActorByCAR(column + 1, row);
        if (target != NULL && target->IsAlive())
        {
            targetList[index] = target;
            index++;
        }
    }
}
void BattleArea::GetSelfRlTargets(int posIndex, ActorPtr *targetList)
{
    TRACE("BattleArea::GetSelfRlTargets posIndex[%d]", posIndex);
    ActorPtr target = NULL;
    int index = 0;
    int column = posIndex % MAX_COLUMN_SIZE;
    int row = posIndex / MAX_ROW_SIZE;
    
    if (column - 1 >= 0)
    {
        target = GetActorByCAR(column - 1, row);
        if (target != NULL && target->IsAlive())
        {
            targetList[index++] = target;
        }
    }

    targetList[index++] = actors[posIndex];

    if (column + 1 < MAX_COLUMN_SIZE)
    {
        target = GetActorByCAR(column + 1, row);
        if (target != NULL && target->IsAlive())
        {
            targetList[index++] = target;
        }
    }
}

void BattleArea::GetDeadTargets(ActorPtr *targetList)
{
    TRACE("BattleArea::GetDeadTargets");
    int index = 0;
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL_PTR && !actors[i]->IsAlive())
        {
            targetList[index] = actors[i];
            index++;
        }
    }
}

void BattleArea::GetSelfRow(int posIndex, ActorPtr *targetList)
{
    TRACE("BattleArea::GetSelfRow");
    int index = 0;
    int row = posIndex / MAX_ROW_SIZE;
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (i == posIndex)
        {
            continue;
        }
        if (actors[i] != NULL_PTR && actors[i]->IsAlive())
        {
            int rowActor = actors[i]->posIndex / MAX_ROW_SIZE;
            if (rowActor == row)
            {
                targetList[index] = actors[i];
                index++;
            }
        }
    }
}

void BattleArea::ModRoundCount()
{
    TRACE("BattleArea::ModRoundCount");
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (actors[i] != NULL_PTR && actors[i]->IsAlive())
        {
            actors[i]->actorMod->RoundCount();
        }
    }
}


ActorPtr BattleArea::GetNewMainTarget(int casterPos, int targetPos)
{
    TRACE("BattleArea::GetNewMainTarget,casterPos[%d],targetPos[%d]",casterPos,targetPos);
    ActorPtr mainTarget = NULL;
    
    if (bf->SkillTestMode)
    {
        mainTarget = GetActorByPos(targetPos);
        if (mainTarget == NULL)
        {
            error("mainTarget == null,targetPos[%d]", targetPos);
        }
    }
    else
    {
        if (bf->lastAttackArea == 0)
        {
            
            mainTarget = GetNextMainTarget(casterPos, targetPos, bf->CurBattleMode->TargetType_Attacker);
            if (mainTarget == NULL)
            {
                error("mainTarget == null,TargetType_Attacker[%d]", bf->CurBattleMode->TargetType_Attacker);
            }
        }
        else
        {
            mainTarget = GetNextMainTarget(casterPos, targetPos,bf->CurBattleMode->TargetType_Defender);
            if (mainTarget == NULL)
            {
                error("mainTarget == null,TargetType_Defender[%d]", bf->CurBattleMode->TargetType_Defender);
            }
        }
    }
    return mainTarget;
}

ActorPtr BattleArea::GetNextMainTarget(int casterPos, int targetPos, ESelectTarget etarget)
{
    TRACE("BattleArea::GetNextMainTarget,casterPos[%d],targetPos[%d],etarget[%d]", casterPos, targetPos,etarget);
    if (etarget == BM_Sequence)
    {
        if (targetPos >= 0 && targetPos < BATTLE_POS_MAX)
        {
            if (targetPos < MAX_COLUMN_SIZE)
            {
                if (actors[targetPos] != NULL_PTR && actors[targetPos]->IsAlive())
                {
                    return actors[targetPos];
                }
            }
            else
            {
                for (int i = 0; i < MAX_COLUMN_SIZE; ++i)
                {
                    if (actors[i] != NULL_PTR && actors[i]->IsAlive())
                    {
                        return actors[i];
                    }
                }

                if (actors[targetPos] != NULL_PTR && actors[targetPos]->IsAlive())
                {
                    return actors[targetPos];
                }
            }
        }
        
        for (int i = 0; i < BATTLE_POS_MAX; ++i)
        {
            if (actors[i] != NULL_PTR && actors[i]->IsAlive())
            {
                return actors[i];
            }
        }
    } 
    else if (etarget == BM_ERelevant)
    {
        bool frontRowPrior = false;
        //TODO 这个MAX_ROW_SIZE = MAX_COLUMN_SIZE, MAX_COLUMN_SIZE
        for (int i = 0; i < MAX_ROW_SIZE; i++)
        {
            if (actors[i] != NULL_PTR && actors[i]->IsAlive())
            {
                frontRowPrior = true;
                break;
            }
        }

        targetPos = (casterPos >= MAX_ROW_SIZE) ? (casterPos - MAX_ROW_SIZE) : casterPos;
        if (frontRowPrior)
        {
            ExistReturn(targetPos)
            if (targetPos + 1 < MAX_ROW_SIZE)
            {
                ExistReturn(targetPos + 1)
            }
            if (targetPos - 1 >= 0)
            {
                ExistReturn(targetPos - 1)
            }
        }
        else
        {
            targetPos += MAX_ROW_SIZE;
            ExistReturn(targetPos)
            if (targetPos + 1 < BATTLE_POS_MAX)
            {
                ExistReturn(targetPos + 1)
            }
            if (targetPos - 1 >= MAX_ROW_SIZE)
            {
                ExistReturn(targetPos - 1)
            }
        }

        for (int i = 0; i < BATTLE_POS_MAX; ++i)
        {
            if (actors[i] != NULL_PTR && actors[i]->IsAlive())
            {
                return actors[i];
            }
        }
    }
    else if (etarget == BM_ERandom)
    {
        int targetIndexs[BATTLE_POS_MAX] = { 0 };
        bool frontRowPrior = false;
        for (int i = 0; i < MAX_ROW_SIZE; i++)
        {
            if (actors[i] != NULL_PTR && actors[i]->IsAlive())
            {
                frontRowPrior = true;
                break;
            }
        }
        int validCnt = 0;
        for (int i = 0; i < BATTLE_POS_MAX; i++)
        {
            if (actors[i] != NULL_PTR && actors[i]->IsAlive())
            {
                if (frontRowPrior && i < MAX_ROW_SIZE)
                {
                    targetIndexs[validCnt] = i;
                    validCnt++;
                }
                else
                {
                    targetIndexs[validCnt] = i;
                    validCnt++;
                }
            }
        }
        if (validCnt == 0)
        {
            return NULL;
        }
        int randIndex = bf->random.RandForWidthFromZero(validCnt);
        return actors[targetIndexs[randIndex]];
    }
    return NULL;
}

void BattleArea::GetSameSide(int casterPos,ActorPtr *targetList)
{
    TRACE("BattleArea::GetSameSide");
    int validCnt = 0;
    ActorPtr ownActor = actors[casterPos];

    for (int i = 0; i < BATTLE_POS_MAX;i++)
    {
        if (i == casterPos)
        {
            continue;
        }
        ActorPtr curActor = actors[i];
        if (curActor != NULL && curActor->IsAlive() && curActor->GetNationality() == ownActor->GetNationality())
        {
            targetList[validCnt++] = actors[i];
        }
    }
}

bool BattleArea::CheckRoundOver()
{
    TRACE("BattleArea::CheckRoundOver");
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        ActorPtr actor = actors[i];
        if (actor != NULL && actor->IsAlive() && actor->AllowComAtk())
        {
            return false;
        }
    }
    return true;
}

void BattleArea::AddRoundAffix(int affixId,int power)
{
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        ActorPtr actor = actors[i];
        if (actor != NULL && actor->IsAlive())
        {
            RoundAttrInfo actorAttr;
            ApplyAffix(actor, affixId, power, actorAttr);
            actorAffixs[i] =actorAttr;
        }
    }
}

void BattleArea::ClearRoundAffix()
{
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        ActorPtr actor = actors[i];
        if (actor != NULL && actor->IsAlive())
        {
            RoundAttrInfo attrInfo = actorAffixs[i];
            RemoveAffix(actor, attrInfo);
            actorAffixs[i].clear();
        }
    }
}

void BattleArea::OnBuffTrigger(int triggerArea)
{
    TRACE("BattleArea::OnBuffTrigger triggerArea[%d]", triggerArea);
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        ActorPtr actor = actors[i];
        if (actor != NULL && actor->IsAlive())
        {
            if (bf->roundState == RS_Started)
            {
                actor->actorMod->ModEffect(PAllRoundStart, triggerArea);
            }
            else if (bf->roundState == RS_Ended)
            {
                actor->actorMod->ModEffect(PAllRoundEnd, triggerArea);
            }
        }
    }
}

void BattleArea::OnBuffCount()
{
    TRACE("BattleArea::OnBuffCount ");
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        ActorPtr actor = actors[i];
        if (actor != NULL && actor->IsAlive())
        {
            actor->actorMod->RoundCount();
        }
    }
}

void BattleArea::RoundXpRecovery()
{
    TRACE("BattleArea::RoundBeginXpRecovery ");
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        ActorPtr actor = actors[i];
        if (actor != NULL && actor->IsAlive())
        {
            int xpRecovery = actor->GetAttrByType(RoundBeginXpRecovery) * actor->GetAttrByType(XpGainRate);
            if (xpRecovery > 0)
            {
                if (actor->ChangeXp(xpRecovery))
                {
                    auto param = FillXpChangeParam(actor, actor, 0, xpRecovery);
                    param.recovery = XpRecoveryType::RoundBegin;
                    FillXpChangeCmd(param);
                }
            }
        }
    }
}

void BattleArea::CheckDeleteMods()
{
    TRACE("BattleArea::CheckDeleteMods");
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        ActorPtr actor = actors[i];
        if (actor != NULL && actor->IsAlive())
        {
            actor->actorMod->CheckDeleteMods();
        }
    }
}

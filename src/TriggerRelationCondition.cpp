#include "math.h"
#include "Actor.h"
#include "BattleField.h"
#include "TriggerCondition.h"
#include "TriggerRelationCondition.h"
#include "TriggerCondition.h"
#include "BattleTrigger.h"
#include "ActorModifier.h"
#include "ActorModifierSub.h"
#include "BattleArea.h"
#include "ActorDatas.h"
#include "ActorSkillSub.h"

void ConTargetMatchParam::Init(BattleTrigger* pParent,FightCondMeta *condmeta)
{
    TriggerCondition::Init(pParent,condmeta);
    conTarget2 = condMeta->ConTarget2;
    relation = condMeta->Relation;
    conAttr = condMeta->ConAttr;
    conAttr2 = condMeta->ConAttr2;
}

double ConTargetMatchParam::GetConAttrValue(Actor *pTarget , const TriggerParam & param,ConMatchType conType){
    
    if (pTarget == NULL)
    {
        return 0;
    }
    
    double val = 0;
    switch (conType) {
        case ConMatchType::HPPercent:
            val = pTarget->actorDatas.hp/pTarget->actorDatas.hpMax*10000;
            break;
        case ConMatchType::XPPercent:
            val = pTarget->actorDatas.xp/pTarget->actorDatas.xpMax*10000;
            break;
        case ConMatchType::ActionOrder:
            if (pTarget->battleField->firstArea == pTarget->areaIndex)
                val = 0;
            else
                val =1;
            break;
        case ConMatchType::MainTargetCount:
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
                        if (partnerActor == nullptr || !partnerActor->IsAlive())
                        {
                            val = 1;
                        }
                        else
                        {
                            val =2;
                        }
                    }
                }
            }
            break;
        case ConMatchType::OurAliveCount:
            if (pTarget->battleField != nullptr)
            {
                BattleArea* area = pTarget->battleField->GetEnemyArea(pTarget->battleField->lastAttackArea);
                val = area->AliveCount();
            }
            break;
        case ConMatchType::OppAliveCount:
            if (pTarget->battleField != nullptr)
            {
                BattleArea* area = pTarget->battleField->GetEnemyArea(pTarget->areaIndex);
                val = area->AliveCount();
            }
            break;
        default:
            break;
    }
    return val;
}

bool ConTargetMatchParam::CheckCondition(const TriggerParam & param)
{
    Actor *pTarget = GetTargetActorByTarget(param, conTarget);
    double val = -1;
    if (pTarget != NULL)
    {
        val = GetConAttrValue(pTarget, param, conAttr);
    }else if (conTarget == TriggerConTarget::ConAttrVal)
    {
        val = conAttr;
    }else
    {
        val = GetTargetValueByType(param,conTarget);
    }
    
    Actor *pTarget2 = GetTargetActorByTarget(param, conTarget2);
    double val2 = -1;
    if (pTarget2 != NULL)
    {
        val2 = GetConAttrValue(pTarget2, param, conAttr2);
    }else if (conTarget2 == TriggerConTarget::ConAttrVal)
    {
        val2 = conAttr2;
    }else
    {

        val2 = GetTargetValueByType(param,conTarget2);
    }
    const double RATE = 0.0001;
    switch (relation) {
        case CondRelation::Equal :
            return fabs((double)(val - val2))<RATE;
        case CondRelation::NotEqual :
            return fabs((double)(val - val2))>RATE;
        case CondRelation::GreatThen :
            return (val - val2)>RATE;
        case CondRelation::LessThen :
            return (val2  - val)>RATE;
        case CondRelation::GreatOrEqual :
            return fabs((double)(val - val2))<RATE || (val - val2)>RATE;
        case CondRelation::LessOrEqual :
            return fabs((double)(val - val2))<RATE || (val2  - val)>RATE;
    }
    error("error relation %d",relation);
    return false;
}

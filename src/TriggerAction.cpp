#include "TriggerAction.h"
#include "ActorSkillSub.h"
#include "BattleTrigger.h"
#include "ActorModifier.h"
#include "ActorModifierSub.h"
#include "Actor.h"
#include "BattleField.h"
#include "SubEffectPerformer.h"
TriggerAction::TriggerAction()
    :actionType(TriggerActionType::Invalid)
    , pParentTrigger(NULL)
    , eCountMeta(0)
    , resetMeta(0)
    , eCount(0)
    , delayAction(TriggerActionDelay::Default)
{

}

TriggerAction::~TriggerAction()
{

}

bool TriggerAction::NextExceute(TriggerParam & param)
{
    eCount++;
    if (eCount >= eCountMeta)
    {
        if (resetMeta != 0)
        {
            eCount = 0;
        }
        return NextExceuteImpl(param);
    }
    return true;
}

void TriggerAction::Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4,int reset, TriggerActionCaster caster)
{
    pParentTrigger = pParent;
    actionType = aType;
    eCountMeta = eCnt;
    resetMeta = reset;
    eCount = 0;
}


bool DoEffectAction::NextExceuteImpl(TriggerParam & param)
{
    
    Actor* pCaster = NULL;
    switch (casterMeta)
    {
    case TriggerActionCaster::C1 :
        {

            pCaster = param.c1.list[0];
        }
        break;
    case TriggerActionCaster::C2:
        {
            pCaster = param.c2.list[0];
        }
        break;
    case TriggerActionCaster::BuffCaster:
        {
            if (pParentTrigger != NULL)
            {
                ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
                if (pBuff != NULL)
                {
                    pCaster = pBuff->casterIn;
                }
            }
        }
        break;
    case  TriggerActionCaster::BuffOwner:
        {
            if (pParentTrigger != NULL)
            {
                ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
                if (pBuff != NULL)
                {
                    pCaster = pBuff->owner;
                }
            }
        }
        break;
    default:
        break;
    };

    if (pCaster != NULL && pCaster->IsAlive())
    {
        
        if (pParentTrigger->GetOriginBuff() != NULL)
        {
            
            Actor* buffCaster = pParentTrigger->GetOriginBuff()->casterIn;
            Actor* buffOwner = pParentTrigger->GetOriginBuff()->owner;
            if (delayAction == TriggerActionDelay::Default)
            {
                EffectSub * effect = new EffectSub(pCaster, effectID, pParentTrigger->GetOriginBuff()->modLevel);
                effect->GetEffectTarget(0, &param.c1, &param.c2, buffCaster, buffOwner);
                effect->ExecuteEffect(SkillInfo());
                delete effect;
            }
            else
            {

                DelayDoAction delayAct;
                delayAct.c1 = param.c1;
                delayAct.c2 = param.c2;
                delayAct.effectCaster = pCaster;
                delayAct.effectID = effectID;
                delayAct.bufferCaster = buffCaster;
                delayAct.bufferOwner = buffOwner;
                delayAct.triggerID = pParentTrigger->GetTriggerID();
                delayAct.levelID = pParentTrigger->GetOriginBuff()->modLevel;
                if (delayAction == TriggerActionDelay::BigRoundStart)
                {
                    pCaster->battleField->AppendDelayAction(delayAct, true);
                }
                else
                {
                    pCaster->battleField->AppendDelayAction(delayAct, false);
                }
            }
        }
    }
    return true;
}

void DoEffectAction::Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster)
{
    TriggerAction::Init(pParent,aType, eCnt, param,param1,param2,param3,param4, reset, caster);
    effectID = param;
    casterMeta = caster;
}

DoEffectAction::DoEffectAction()
    :effectID(0)
    , casterMeta(0)
{

}

RemoveSrcBuffAction::RemoveSrcBuffAction()
{

}

bool RemoveSrcBuffAction::NextExceuteImpl(TriggerParam & param)
{
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            Actor* pOwner = pBuff->owner;
            if (pOwner != NULL)
            {
                pOwner->actorMod->RemoveModByTrigger(pOwner,pBuff);
            }
        }
    }
    return false;
}

RemoveTriggerAction::RemoveTriggerAction()
{

}

bool RemoveTriggerAction::NextExceuteImpl(TriggerParam & param)
{
    if (pParentTrigger != NULL)
    {
        ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
        if (pBuff != NULL)
        {
            pBuff->RemoveTrigger();
        }
    }
    return false;
}

bool ExeEffectAffixAction::NextExceuteImpl(TriggerParam & param)
{
    if (param.evtType == (int)EEventType::CauseSkill)
    {
        if (param.pEffect != nullptr)
        {
            param.pEffect->ModifyExtraAffix(param.c1.list[0], affixID, affixBase, affixGrow);
        }
    }
    else if (param.evtType == (int)EEventType::SufferSkill)
    {
        if (param.pEffect != nullptr)
        {
            if (pParentTrigger != NULL)
            {
                ActorModifierSub* pBuff = pParentTrigger->GetOriginBuff();
                if (pBuff != NULL)
                {
                    Actor* pOwner = pBuff->owner;
                    if (pOwner != NULL)
                    {
                        param.pEffect->ModifyExtraAffix(pOwner, affixID, affixBase, affixGrow);
                    }
                }
            }
        }
    }
    else if (param.evtType == (int)EEventType::ComingCauseDamage || param.evtType == (int)EEventType::ComingCauseHeal)
    {
        if (param.affixPerformer != nullptr)
        {
            param.affixPerformer->ApplyCasterAffix(param.c1.list[0], affixID, affixBase, affixGrow);
        }
    }
    else if (param.evtType == (int)EEventType::ComingSufferDamage || param.evtType == (int)EEventType::ComingSufferHeal)
    {
        if (param.affixPerformer != nullptr)
        {
            param.affixPerformer->ApplyTargerAffix(param.c1.list[0], affixID, affixBase, affixGrow);
        }
    }
    return true;
}

void ExeEffectAffixAction::Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster)
{
    TriggerAction::Init(pParent, aType, eCnt, param, param1,param2,param3,param4, reset, caster);
    affixID = param;
    affixBase = param1;
    affixGrow = param2;
}

bool Old_ExeEffectBuffChanceAction::NextExceuteImpl(TriggerParam & param)
{
    if (param.pEffect != nullptr)
    {
        if (param.evtType == (int)EEventType::CauseSkill)
        {
            param.pEffect->ModifyBuffProbality(buffIndex, extraChanceBase, extraChanceGrow);
        }
    }
    return true;
}

void Old_ExeEffectBuffChanceAction::Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster)
{
    TriggerAction::Init(pParent, aType, eCnt, param, param1,param2,param3,param4,reset, caster);
    buffIndex = param - 1;
    extraChanceBase = param1;
    extraChanceGrow = param2;
}

bool ExeEffectBuffChanceAction::NextExceuteImpl(TriggerParam & param)
{
    auto realLevel = param.level;
    if (realLevel >= 1)
    {
        realLevel--;
    }
    auto x = extraChanceBase + realLevel*extraChanceGrow;
    auto y = extraChanceBase2 + realLevel*extraChanceGrow2;
    param.resultDouble = x;
    param.resultDouble2 = y*100;
    param.hasResult = true;
    return true;
}

void ExeEffectBuffChanceAction::Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster)
{
    TriggerAction::Init(pParent, aType, eCnt, param, param1,param2,param3,param4,reset, caster);
    extraChanceBase = param/10000;
    extraChanceBase2 = param1/10000;
    extraChanceGrow = param2/10000;
    extraChanceGrow2 = param3/10000;
}

bool ExecSpEffectChanceAction::NextExceuteImpl(TriggerParam & param)
{
    auto realLevel = param.level;
    if (realLevel >= 1)
    {
        realLevel--;
    }
    auto x = extraChanceBase + realLevel*extraChanceGrow;
    auto y = extraChanceBase2 + realLevel*extraChanceGrow2;
    param.resultDouble = x;
    param.resultDouble2 = y*100;
    param.hasResult = true;
    return true;
}

void ExecSpEffectChanceAction::Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster)
{
    TriggerAction::Init(pParent, aType, eCnt, param, param1,param2,param3,param4,reset, caster);
    extraChanceBase = param/10000;
    extraChanceBase2 = param1/10000;
    extraChanceGrow = param2/10000;
    extraChanceGrow2 = param3/10000;
}


bool ChangeFuncParamsAction::NextExceuteImpl(TriggerParam & param)
{
    if(order<0)
        return false;
    auto doubleParams = param.doubleParams;
    auto oldVal = doubleParams[order];
    double x = extraChanceBase+ param.level*extraChanceGrow;
    double y =extraChanceBase2+param.level*extraChanceGrow2;
    param.doubleParams[order]= oldVal*x+y;
    param.hasResult = true;
    return true;
}

void ChangeFuncParamsAction::Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster)
{
    TriggerAction::Init(pParent, aType, eCnt, param, param1,param2,param3,param4,reset, caster);
    order = param;
    extraChanceBase = param1;
    extraChanceBase2 = param2;
    extraChanceGrow = param3;
    extraChanceGrow2 = param4;
}

bool ChangeSkillRedoRateAction::NextExceuteImpl(TriggerParam & param)
{
    double oldVal = param.doubleParams[0];
    double x = extraChanceBase+ param.level*extraChanceGrow;
    double y =extraChanceBase2+param.level*extraChanceGrow2;
    param.resultDouble= oldVal*x+(double)y/10000;
    param.hasResult = true;
    return true;
}

void ChangeSkillRedoRateAction::Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster)
{
    TriggerAction::Init(pParent, aType, eCnt, param, param1,param2,param3,param4,reset, caster);
    extraChanceBase = param;
    extraChanceBase2 = param1;
    extraChanceGrow = param2;
    extraChanceGrow2 = param3;
}

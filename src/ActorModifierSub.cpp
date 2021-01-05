#include "ActorModifierSub.h"
#include "ActorModifier.h"
#include "Actor.h"
#include "BattleField.h"
#include "ModifierMeta.h"
#include "EffectUtil.h"
#include "ModifierUtils.h"
#include "ModTrigger.h"
#include "EventData.h"
#include "ConditionCheck.h"
#include "ActorSkillSub.h"
#include "Calculator.h"
#include "Random.h"
#include "Log.h"
#include "Enum.h"
#include <stdio.h>
#include "ActorAttrs.h"
#include "NewActorAttrs.h"
#include "TriggerManager.h"

ActorModifierSub::ActorModifierSub(ActorPtr caster, int modId, ActorPtr actor, int level, int round)
    :triggerInstID(0)
    , triggerType(0)
    , snapShot(nullptr)
{
    TRACE("ActorModifierSub::ActorModifierSub caster[%d] modID[%d] round[%d]", caster->actorId, modId, round);
    deleteMark = false;
    shieldDamage = 0;
    casterIn = caster;
    owner = actor;
    attachRound = casterIn->battleField->activeRoundCount;
    modMeta = (ModifierMeta *)ModifierMetaSet::instance()->Get(modId);
    if (modMeta == NULL)
    {
        error("no buff id :%d", modId);
        return;
    }

    if (round <= -1)
    {
        remainActionCount = modMeta->LastRound;
    }
    else
    {
        remainActionCount = round;
    }
    
    //triggerTimes = modMeta->triggerTimes;
    
    debug("add mod actorId: %d --- modId: %d", owner->actorId, modId);
    modLevel = level;
    FillBuffAttachCmd(caster,owner, modId, modId, 0, false);
    OnAttachEffect();

    TriggerParam param;
    param.evtType = EEventType::GainBuff;
    param.c1.list[0] = owner;
    param.c2.list[0] = casterIn;
    param.pBuff = this;
    owner->battleField->FireEvent(param);

    ////debug("RegisterListener : %d ownerId: %d, eventType: %d\n", modId, owner->actorId, modMeta->event);
    //switch (modMeta->event) {
    //    case GetDamage:
    //        actor->battleField->getDamageTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case SetDamage:
    //        actor->battleField->sendDamageTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case DeadSelf:
    //        actor->battleField->deadSelfTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case DeadSomeone:
    //        actor->battleField->deadSomeoneTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case WhenCastSkill:
    //        actor->battleField->whenCastSkillTigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case WhenCastTypeSkill:
    //        actor->battleField->whenCastTypeSkillTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case SpecialEffect:
    //        actor->battleField->specialEffectTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case GetSpecialEffect:
    //        actor->battleField->getSpecialEffectTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case BattleStart:
    //        actor->battleField->battleStartTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case SpecialRoundStart:
    //        actor->battleField->specialRoundStartTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case SpecialRoundEnd:
    //        actor->battleField->specialRoundEndTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case WhenDodge:
    //        actor->battleField->whenDodgeTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case WhenStrike:
    //        actor->battleField->whenStrikeTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case WhenBlock:
    //        actor->battleField->whenBlockTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case WhenHeal:
    //        actor->battleField->whenHealTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case DamageHit:
    //        actor->battleField->damageHitTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case GetDamageHit:
    //        actor->battleField->getDamageHitTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case KillSomeone:
    //        actor->battleField->killSomeoneTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case DeadEnemy:
    //        actor->battleField->deadEnemyTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    case DeadFriend:
    //        actor->battleField->deadFriendTrigger.RegisterListener(this, owner->actorId, modMeta);
    //        break;
    //    default:
    //        break;
    //}
}

ActorModifierSub::~ActorModifierSub()
{
    TRACE("ActorModifierSub::~ActorModifierSub");
    if (snapShot != NULL)
    {
        delete snapShot;
        snapShot = NULL;
    }
}

void ActorModifierSub::OnAttachEffect()
{
    TRACE("ActorModifierSub::OnAttachEffect owner[%d] ModID[%d]", owner->actorId, modMeta->id);
    if (modMeta->AffixType == BuffAffixApply::Attach)
    {
        if (modMeta->Affix != 0)
        {
            int affixPower = modMeta->AffixPower[0] + (modLevel - 1)*modMeta->AffixPower[1];
            attrInfo.clear();
            ApplyAffix(owner, modMeta->Affix, affixPower, attrInfo);
        }
    }
    if (modMeta->BeLegend != 0)
    {
        owner->actorMod->legendSkillActive++;
    }
    switch ((LastEffectType)modMeta->LastEffect)
    {
    case Paralysis:
        owner->actorMod->restrictCommonSkill++;
        break;
    case Seal:
        owner->actorMod->sealedLayer++;
        break;
    case Dizzy:
        owner->actorMod->restrictAction++;
        break;
    case BuffShield:
        {
            float tempDamage = CalculateShield((ActorAttrType)(int)modMeta->LastParam[0], casterIn,owner,(modLevel - 1) * modMeta->LastParam[2] + modMeta->LastParam[1], (modLevel - 1) * modMeta->LastParam[4] + modMeta->LastParam[3]);
            if (tempDamage < 1)
            {
                tempDamage = 1;
            }
            shieldDamage = tempDamage;
        }
        break;
    case LastEffectType::Immune:
        for (int i = 0; i < MAX_DMG_PARAM; ++i)
        {
            if (modMeta->LastParam[i] != 0)
            {
                owner->actorMod->RemoveModByImmune(casterIn, (int)modMeta->LastParam[i], 0,modMeta->id);
            }
        }
        break;
    default:
        break;
    }
    if (modMeta->PeriodSnap > 0)
    {
        if (snapShot != nullptr)
        {
            delete snapShot;
        }
        snapShot = new NewAttrSnapShot{};
        casterIn->actorAttrs.CopyToActorData(modMeta->SnapAttribute, *snapShot);
        for (auto pair : *snapShot)
        {
            if (pair.first == DoubleHitAffect)
            {
                if(pair.second ==1)
                {
                    snapShot->push_back({ ActorAttrType::DoubleHitRate , owner->battleField->doubleHitRate});
                }
                break;
            }
        }
    }
    if (modMeta->Trigger > 0)
    {
        BattleTriggerManager * pTriggerMgr = owner->battleField->GetTriggerMgr();
        if (pTriggerMgr != NULL)
        {
            BattleTrigger* pTrigger = pTriggerMgr->AddTrigger(modMeta->Trigger);
            if (pTrigger != NULL)
            {
                pTrigger->InitOriginBuff(this);
                triggerInstID = pTrigger->GetInstID();
                triggerType = pTrigger->GetEventType();
            }
        }
    }
}

void ActorModifierSub::DoEffect(PeriodType sType, int triggerArea)
{
    TRACE("ActorModifierSub::DoEffect PeriodType[%d] triggerArea[%d]", sType, triggerArea);
    if (modMeta->PeriodType != sType)
    {
        return;
    }
    if (owner->areaIndex == casterIn->areaIndex)
    {
        if (owner->areaIndex != triggerArea)
        {
            return;
        }
    }
    else
    {
        if (casterIn->areaIndex != triggerArea)
        {
            return;
        }
    }
    
    float rate = casterIn->battleField->random.RandForWidth(100);
    if (rate > modMeta->PeriodChance)
    {
        return;
    }
    if (!CheckPeroidCondition(modMeta->PeriodCondType1,modMeta->PeriodCondParam1))
    {
        return;
    }
    if (!CheckPeroidCondition(modMeta->PeriodCondType2, modMeta->PeriodCondParam2))
    {
        return;
    }
    if (!CheckPeroidCondition(modMeta->PeriodCondType3, modMeta->PeriodCondParam3))
    {
        return;
    }
    OnEffect();
}

void ActorModifierSub::OnEffect()
{
    TRACE("ActorModifierSub::OnEffect: ModID[%d]", modMeta->id);

    switch (modMeta->PeriodEffect)
    {
        case PSubEffect:
            {
                const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(modMeta->PeriodParam[0]);
                if (subMeta != NULL_PTR)
                {
                    if (modMeta->PeriodSnap > 0)
                    {
                        if (snapShot == nullptr)
                        {
                            error("ActorModifierSub::OnEffect(),pAttrSnapShot == NULL BuffID[%d]",modMeta->id);
                        }
                        else
                        {
                            casterIn->actorAttrs.SetSnapShot(snapShot);
                            TriggerTrace tc;
                            tc.buffId = modMeta->id;
                            tc.subEffectId = subMeta->id;
                            makeEffectByType(casterIn, owner, subMeta, 0, modLevel, modMeta->id,SkillType::Invalid,tc);
                            casterIn->actorAttrs.SetSnapShot(nullptr);
                        }
                    }
                    else
                    {
                        TriggerTrace tc;
                        tc.buffId = modMeta->id;
                        tc.subEffectId = subMeta->id;
                        makeEffectByType(casterIn, owner, subMeta, 0, modLevel, modMeta->id,SkillType::Invalid,tc);
                    }
                }
                else
                {
                    error("subEffect meta cant find: %d\n", modMeta->PeriodParam[0]);
                }
            }
            break;
        case PSkill:
            {
                if (modMeta->PeriodParam[0] != 0)
                {
                    ActorSkillSubPtr cs = new ActorSkillSub(owner->actorSkill);
                    if (cs->InitSkill(modMeta->PeriodParam[0], modLevel,true))
                    {
                        owner->actionState = CastSkill;
                        cs->DoSkill(NULL_PTR, SkillInfo());
                    }
                    owner->actionState = NoneState;
                    delete cs;
                }
            }
            break;
        case  PEffect:
        {
            if (modMeta->PeriodParam[0] != 0)
            {
                int effectID = (int)modMeta->PeriodParam[0];
                ActorPtr caster = (modMeta->PeriodEffectCaster == 1) ? owner : casterIn;
                EffectSub * effect = new EffectSub(caster, effectID, modLevel);
                effect->GetEffectTarget(0,NULL,NULL,NULL,NULL);
                effect->InitOriginBuffID(modMeta->id);
                effect->ExecuteEffect(SkillInfo());
                delete effect;
            }
        }
        break;
        default:
            break;
    }
}

void ActorModifierSub::DoDetach()
{
    TRACE("ActorModifierSub::DoDetach modID[%d]", modMeta->id);
    RemoveTrigger();

    switch ((LastEffectType)modMeta->LastEffect)
    {
    case Paralysis:
        owner->actorMod->restrictCommonSkill--;
        break;
    case Seal:
        owner->actorMod->sealedLayer--;
        break;
    case Dizzy:
        owner->actorMod->restrictAction--;
        break;
    default:
        break;
    }
    if (modMeta->PeriodSnap > 0 && snapShot != NULL)
    {
        delete snapShot;
        snapShot = NULL;
    }
    if (modMeta->AffixType == BuffAffixApply::Attach)
    {
        if (modMeta->Affix != 0)
        {
            RemoveAffix(owner, attrInfo);
            attrInfo.clear();
        }
    }
    if (modMeta->BeLegend != 0)
    {
        owner->actorMod->legendSkillActive--;
    }
    /*switch (owner->battleField->roundState)
    {
    case RS_Started:
        owner->battleField->AddBeforeBuffCmd(owner->actorId, 0, subEffectID, 0, 0, 0, 0, modMeta->id, false, true, 0);
        break;
    case RS_Attacking:
        owner->battleField->AddSkillEffectCmd(owner->actorId, 0, subEffectID, 0, 0, 0, 0, modMeta->id, false, true, 0);
        break;
    case RS_Ended:
        owner->battleField->AddAfterBuffCmd(owner->actorId, 0, subEffectID, 0, 0, 0, 0, modMeta->id, false, true,0);
        break;
    default:
        break;
    }*/


    /*TRACE("ActorModifierSub::DoDetach UnregisterListener : %d ownerId : %d, eventType: %d\n", modMeta->id, owner->actorId, modMeta->event);
    switch (modMeta->event) {
        case GetDamage:
            owner->battleField->getDamageTrigger.UnregisterListener(this);
            break;
        case SetDamage:
            owner->battleField->sendDamageTrigger.UnregisterListener(this);
            break;
        case DeadSelf:
            owner->battleField->deadSelfTrigger.UnregisterListener(this);
            break;
        case DeadSomeone:
            owner->battleField->deadSomeoneTrigger.UnregisterListener(this);
            break;
        case WhenCastSkill:
            owner->battleField->whenCastSkillTigger.UnregisterListener(this);
            break;
        case WhenCastTypeSkill:
            owner->battleField->whenCastTypeSkillTrigger.UnregisterListener(this);
            break;
        case SpecialEffect:
            owner->battleField->specialEffectTrigger.UnregisterListener(this);
            break;
        case GetSpecialEffect:
            owner->battleField->getSpecialEffectTrigger.UnregisterListener(this);
            break;
        case BattleStart:
            owner->battleField->battleStartTrigger.UnregisterListener(this);
            break;
        case SpecialRoundStart:
            owner->battleField->specialRoundStartTrigger.UnregisterListener(this);
            break;
        case SpecialRoundEnd:
            owner->battleField->specialRoundEndTrigger.UnregisterListener(this);
            break;
        case WhenDodge:
            owner->battleField->whenDodgeTrigger.UnregisterListener(this);
            break;
        case WhenStrike:
            owner->battleField->whenStrikeTrigger.UnregisterListener(this);
            break;
        case WhenBlock:
            owner->battleField->whenBlockTrigger.UnregisterListener(this);
            break;
        case WhenHeal:
            owner->battleField->whenHealTrigger.UnregisterListener(this);
            break;
        case DamageHit:
            owner->battleField->damageHitTrigger.UnregisterListener(this);
            break;
        case GetDamageHit:
            owner->battleField->getDamageHitTrigger.UnregisterListener(this);
            break;
        case KillSomeone:
            owner->battleField->killSomeoneTrigger.UnregisterListener(this);
            break;
        case DeadEnemy:
            owner->battleField->deadEnemyTrigger.UnregisterListener(this);
            break;
        case DeadFriend:
            owner->battleField->deadFriendTrigger.UnregisterListener(this);
            break;
        default:
            break;
    }*/
}
//
//void ActorModifierSub::HandleNotification(EventDataPtr data)
//{
//    TRACE("ActorModifierSub::HandleNotification event: modId: %d, ownerId: %d, eventType : %d\n", modMeta->id, owner->actorId, modMeta->event);
//    bool isForEvent = true;
//    switch (modMeta->event) {
//        case GetDamage:
//            if (data->target->actorId != owner->actorId)
//            {
//                isForEvent = false;
//            }
//            break;
//        case SetDamage:
//            if (data->caster->actorId != owner->actorId)
//            {
//                isForEvent = false;
//            }
//            break;
//        case DeadSelf:
//            if (data->target->actorId != owner->actorId)
//            {
//                isForEvent = false;
//            }
//            break;
//        case DeadSomeone:
//            break;
//        case WhenCastSkill:
//            {
//                WhenCastSkillEventData* lData = (WhenCastSkillEventData*)data;
//                if (lData->skillId != modMeta->eventParam[0])
//                {
//                    isForEvent = false;
//                }
//            }
//            break;
//        case WhenCastTypeSkill:
//            {
//                WhenCastTypeSkillEventData* lData = (WhenCastTypeSkillEventData*)data;
//                if (modMeta->eventParam[0] == 1)
//                {
//                    if (lData->skillType != CommonSkillType)
//                    {
//                        isForEvent = false;
//                    }
//                }
//                else if (modMeta->eventParam[0] == 2)
//                {
//                    if (lData->skillType != LegendSkillType)
//                    {
//                        isForEvent = false;
//                    }
//                }
//                else if (modMeta->eventParam[0] == 3)
//                {
//                    if (lData->skillType != XpSkillType)
//                    {
//                        isForEvent = false;
//                    }
//                }
//                else if (modMeta->eventParam[0] == 4)
//                {
//                    if (lData->skillType != CombineSkillType)
//                    {
//                        isForEvent = false;
//                    }
//                }
//                else if (modMeta->eventParam[0] == 5)
//                {
//                    if (lData->skillType != XpSkillType && lData->skillType != CombineSkillType)
//                    {
//                        isForEvent = false;
//                    }
//                }
//                
//            }
//        case SpecialEffect:
//            {
//                SpecialEffectEventData* lData = (SpecialEffectEventData*)data;
//                if (lData->fType != modMeta->eventParam[0])
//                {
//                    isForEvent = false;
//                }
//            }
//            break;
//        case GetSpecialEffect:
//            {
//                GetSpecialEffectEventData* lData = (GetSpecialEffectEventData*)data;
//                if (lData->fType != modMeta->eventParam[0] || lData->target->actorId != owner->actorId)
//                {
//                    isForEvent = false;
//                }
//            }
//            break;
//        case BattleStart:
//            
//            break;
//        case SpecialRoundStart:
//            {
//                if (modMeta->eventParam[0] != 0)
//                {
//                    SpecialRoundStartEventData* lData = (SpecialRoundStartEventData*)data;
//                    if (lData->roundCount != modMeta->eventParam[0])
//                    {
//                        isForEvent = false;
//                    }
//                }
//            }
//            break;
//        case SpecialRoundEnd:
//            {
//                if (modMeta->eventParam[0] != 0)
//                {
//                    SpecialRoundEndEventData* lData = (SpecialRoundEndEventData*)data;
//                    if (lData->roundCount != modMeta->eventParam[0])
//                    {
//                        isForEvent = false;
//                    }
//                }
//            }
//            break;
//        case WhenDodge:
//            if (data->target->actorId != owner->actorId)
//            {
//                isForEvent = false;
//            }
//            break;
//        case WhenStrike:
//            if (data->caster->actorId != owner->actorId)
//            {
//                isForEvent = false;
//            }
//            break;
//        case WhenBlock:
//            if (data->target->actorId != owner->actorId)
//            {
//                isForEvent = false;
//            }
//            break;
//        case WhenHeal:
//            if (data->caster->actorId != owner->actorId)
//            {
//                isForEvent = false;
//            }
//            break;
//        case DamageHit:
//            if (data->caster->actorId != owner->actorId)
//            {
//                isForEvent = false;
//            }
//            break;
//        case GetDamageHit:
//            if (data->target->actorId != owner->actorId)
//            {
//                isForEvent = false;
//            }
//            break;
//        case KillSomeone:
//            if (data->caster->actorId != owner->actorId)
//            {
//                isForEvent = false;
//            }
//            break;
//        case DeadEnemy:
//            if (data->target->areaIndex == owner->areaIndex)
//            {
//                isForEvent = false;
//            }
//            break;
//        case DeadFriend:
//            if (data->target->areaIndex != owner->areaIndex)
//            {
//                isForEvent = false;
//            }
//            break;
//        default:
//            break;
//    }
//    
//    if (!isForEvent)
//    {
//        info("is not right type !!!\n");
//        return;
//    }
//    
//    info("check out for event type !!!\n");
//    // check condition
//    bool isEnough = true;
//    switch (modMeta->condition) {
//        case SelfMoreHp:
//            isEnough = CheckMoreHp(owner, modMeta->conParam);
//            break;
//        case SelfLessHp:
//            isEnough = CheckLessHp(owner, modMeta->conParam);
//            break;
//        case SelfMoreXp:
//            isEnough = CheckMoreXp(owner, modMeta->conParam);
//            break;
//        case SelfLessXp:
//            isEnough = CheckLessXp(owner, modMeta->conParam);
//            break;
//        case SelfHaveMod:
//            isEnough = CheckHaveSpecialBuff(owner, modMeta->conParam);
//            break;
//        case TargetMoreHp:
//            isEnough = CheckMoreHp(data->target, modMeta->conParam);
//            break;
//        case TargetLessHp:
//            isEnough = CheckLessHp(data->target, modMeta->conParam);
//            break;
//        case TargetMoreXp:
//            isEnough = CheckMoreXp(data->target, modMeta->conParam);
//            break;
//        case TargetLessXp:
//            isEnough = CheckLessXp(data->target, modMeta->conParam);
//            break;
//        case TargetHaveMod:
//            isEnough = CheckHaveSpecialBuff(data->target, modMeta->conParam);
//            break;
//        default:
//            break;
//    }
//    
//    if (!isEnough)
//    {
//        info("not enough condition : %d, ownerId : %d\n", modMeta->id, owner->actorId);
//        return;
//    }
//    
//    // calcu rate
//    if (modMeta->triggerChance != 0)
//    {
//        float rate = owner->battleField->random.RandForWidth(100);
//        if (rate > modMeta->triggerChance)
//        {
//            info("not hit the rate : %f , chance : %d\n", rate, modMeta->triggerChance);
//            return;
//        }
//    }
//    
//    data->owner = owner;
//    
//    if (modMeta->triggerTimes != 0)
//    {
//        if (triggerTimes <= 0)
//        {
//            debug("this buff dont have trigger times : %d, ownerId : %d\n", modMeta->id, owner->actorId);
//            return;
//        }
//        else
//        {
//            triggerTimes--;
//        }
//    }
//    
//    info("real trigger event. modId : %d, ownerId : %d, eventType : %d\n", modMeta->id, owner->actorId, modMeta->event);
//    
//    if (modMeta->effectiveTime == Immediately)
//    {
//        switch (modMeta->triggerEffect)
//        {
//            case SelfSubEffect:
//                data->ExecuteSubEffect(modMeta->effectsParam[0]);
//                break;
//            case SelfSkill:
//                data->ExecuteSkill(modMeta->effectsParam[0]);
//                break;
//            case SelfBuff:
//                data->Executebuff(modMeta->effectsParam[0]);
//                break;
//            case CasterSubEffect:
//                data->ExecuteSubForCaster(modMeta->effectsParam[0]);
//                break;
//            case CasterBuff:
//                data->ExecuteBuffForCaster(modMeta->effectsParam[0]);
//                break;
//            case TargetSubEffect:
//                data->ExecuteSubForTarget(modMeta->effectsParam[0]);
//                break;
//            case TargetBuff:
//                data->ExecuteBuffForTarget(modMeta->effectsParam[0]);
//                break;
//            case SelfApplyAffix:
//                data->ExecuteSelfApplyAffix(modMeta->effectsParam[0]);
//                break;
//            default:
//                break;
//        }
//    }
//    else
//    {
//        //cache skill
//        owner->AddCacheSkill(modMeta->effectsParam[0], modLevel);
//    }
//}

bool ActorModifierSub::CheckPeroidCondition(int periodCondition, float params[])
{
    BuffPeriodConType conType = (BuffPeriodConType)periodCondition;
    switch (conType)
    {
    case BuffPeriodConType::Default:
    {
        return true;
    }
    break;
    case BuffPeriodConType::Ignore:
    {
        return true;
    }
    break;
    case BuffPeriodConType::RoundCheck:
    {
        int curRound = casterIn->battleField->roundCount;
        for (int i = 0; i < MAX_DMG_PARAM; i++)
        {
            int roundConfig = (int)params[i];
            if (roundConfig == 0)
            {
                break;
            }
            if (roundConfig == curRound)
            {
                return true;
            }
        }
    }
    case BuffPeriodConType::RoundKeep:
    {
        int curRound = casterIn->battleField->roundCount;
        int startRound = (int)params[0];
        int endRound = startRound + (int)params[1];
        if (curRound >= startRound && curRound < endRound)
        {
            return true;
        }
    }
    break;
    default:
        break;
    }
    return false;
}

void ActorModifierSub::MarkDeleted()
{
    deleteMark = true;
}

int ActorModifierSub::GetAttachRound()
{
    return attachRound;
}

void ActorModifierSub::RemoveTrigger()
{
    if (triggerInstID > 0)
    {
        BattleTriggerManager * pTriggerMgr = owner->battleField->GetTriggerMgr();
        if (pTriggerMgr != NULL)
        {
            pTriggerMgr->RemoveTrigger(triggerType,triggerInstID);
        }
        triggerInstID = 0;
        triggerType = 0;
    }
}

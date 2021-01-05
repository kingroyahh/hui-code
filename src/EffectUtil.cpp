#include "EffectUtil.h"
#include "ActorModifier.h"
#include "Actor.h"
#include "BattleField.h"
#include "SubEffectsMeta.h"
#include "EffectMeta.h"
#include "ModifierUtils.h"
#include "BattleArea.h"
#include "Random.h"
#include "Enum.h"
#include "Log.h"
#include "Calculator.h"
#include "AnalysisUtil.h"
#include "EventData.h"
#include "SubEffectPerformer.h"
#include "TriggerManager.h"
#include "ActorSkillSub.h"
//#include "BattleReplay.pb.h"
using namespace battle;

void makeEffectByType(ActorPtr caster, ActorPtr target,const SubEffectsMeta* subMeta, float param, int skillLv, int buffId, SkillType skillType,TriggerTrace traceData)
{
    TRACE("makeEffectByType : caster[%d] target[%d] subeffect[%d] funcType[%d] buff[%d]", caster->actorId, target->actorId, subMeta->id, subMeta->funcType,buffId);
    if (!caster->IsAlive())
    {
        return;
    }
    if (!target->IsAlive())
    {
        return;
    }

    
    
    SubEffectsMeta meta;
    meta.id = subMeta->id;
    meta.funcType = subMeta->funcType;
    meta.DoRage = subMeta->DoRage;
    meta.DoEvent = subMeta->DoEvent;

    for (int i =0; i<MAX_DMG_PARAM; i++) {
        meta.funcParams[i]=subMeta->funcParams[i];
    }

    TriggerParam param1;
    param1.evtType = EEventType::BeforeCauseSubEffect;
    param1.c1.list[0] = caster;
    param1.c2.list[0] = target;
    param1.r5 = traceData.subEffectId;
    param1.r6 = traceData.effectId;
    param1.r7 = traceData.skillId;
    param1.r8 = traceData.buffId;
    param1.level = skillLv;
    for (int i =0; i<MAX_DMG_PARAM; i++) {
        param1.doubleParams[i]=meta.funcParams[i];
    }
    caster->battleField->FireEvent(param1);

    TriggerParam param2;
    param2.evtType = EEventType::BeforeSufferSubEffect;
    param2.c1.list[0] = target;
    param2.c2.list[0] = caster;
    param1.r5 = traceData.subEffectId;
    param2.r6 = traceData.effectId;
    param2.r7 = traceData.skillId;
    param2.r8 = traceData.buffId;
    param2.level = skillLv;
    for (int i =0; i<MAX_DMG_PARAM; i++) {
        param2.doubleParams[i]=meta.funcParams[i];
    }
    caster->battleField->FireEvent(param2);
    
    
    if(param1.hasResult){
        for (int i =0; i<MAX_DMG_PARAM; i++) {
            meta.funcParams[i] = param1.doubleParams[i];
        }
    }
    
    if(param2.hasResult){
        for (int i =0; i<MAX_DMG_PARAM; i++) {
            meta.funcParams[i] = param2.doubleParams[i];
        }
    }
    
    switch (subMeta->funcType)
    {
        case DamageFunc:
            {
                //if there is Immune,return
                if (target->IsImmuneThisTime)
                {
                    info("target : %d is immune damage this time only !!!", target->actorId);
                    target->IsImmuneThisTime = false;
                    return;
                }
                DamageSubPerformer dsp;
                dsp.MakeData(&meta);
                dsp.MakeTrace(traceData);
                dsp.originBuffId = buffId;
                dsp.skillType = skillType;
                dsp.Execute(caster, target, skillLv, param);
            }
            break;
        case HealFunc:
            {
                HealSubPerformer hsp;
                hsp.MakeData(&meta);
                hsp.MakeTrace(traceData);
                hsp.originBuffId = buffId;
                hsp.Execute(caster, target, skillLv, param);
            }
            break;
         case ChangeXpFunc:
            {
                auto xpOld = target->actorDatas.xp;
                
                ChangeXpPerformer csp;
                csp.MakeData(&meta);
                csp.MakeTrace(traceData);
                csp.originBuffId = buffId;
                csp.Execute(caster, target, skillLv, param);
                
                auto xpNew = target->actorDatas.xp;
                param1.r4 = param2.r4 = xpNew - xpOld;
            }
            break;
        case DispelFunc://
            {
                DispelPerformer dsp;
                dsp.MakeData(&meta);
                dsp.MakeTrace(traceData);
                dsp.originBuffId = buffId;
                dsp.Execute(caster, target, skillLv, param);
            }
            break;
        case ResilienceFunc:
            break;
        case HealFormHpMax:
            {
                HealForHpMaxPerformer dsp;
                dsp.MakeData(&meta);
                dsp.MakeTrace(traceData);
                dsp.originBuffId = buffId;
                dsp.Execute(caster, target, skillLv, param);
            }
            break;
        case HealSet:
        {
            HealSetPerformer dsp;
            dsp.MakeData(&meta);
            dsp.MakeTrace(traceData);
            dsp.originBuffId = buffId;
            dsp.Execute(caster, target, skillLv, param);
        }
            break;
        default:
            error("not have this funcType : %d, submetaid: %d", subMeta->funcType, subMeta->id);
            break;
    }
    
    param1.evtType = EEventType::AfterCauseSubEffect;
    param1.c1.list[0] = caster;
    param1.c2.list[0] = target;
    param1.r5 = traceData.subEffectId;
    param1.r6 = traceData.effectId;
    param1.r7 = traceData.skillId;
    param1.r8 = traceData.buffId;
    param1.level = skillLv;
    caster->battleField->FireEvent(param1);
    
    param2.evtType = EEventType::AfterSufferSubEffect;
    param2.c1.list[0] = target;
    param2.c2.list[0] = caster;
    param1.r5 = traceData.subEffectId;
    param2.r6 = traceData.effectId;
    param2.r7 = traceData.skillId;
    param2.r8 = traceData.buffId;
    param2.level = skillLv;
    caster->battleField->FireEvent(param2);
}
void FillTriggerEvt(EffectCmd* subEffect, ActorPtr target, ActorPtr caster)
{
    BattleTriggerManager* triggerMgr = target->battleField->GetTriggerMgr();
    if (triggerMgr != NULL)
    {
        EEventType topType = triggerMgr->GetTopEvtType();
        auto bigRoundDelayAct = target->battleField->IsBigRoundDelayActExecuting();
        auto smallRoundDelayAct = target->battleField->IsSmallRoundDelayActExecuting();
        if (topType != EEventType::Invalid || bigRoundDelayAct || smallRoundDelayAct)
        {
            if (caster != NULL)
            {
                subEffect->set_triggercaster_id(caster->actorId);
            }
            int triggerID = 0;
            if (bigRoundDelayAct)
            {
                subEffect->set_triggerevt(EEventType::BigRoundStart);
                triggerID = target->battleField->GetCurDelayActTriggerID();
            }
            else if (smallRoundDelayAct)
            {
                subEffect->set_triggerevt(EEventType::SmallRoundEnd);
                triggerID = target->battleField->GetCurDelayActTriggerID();
            }
            else
            {
                subEffect->set_triggerevt(topType);
                triggerID = triggerMgr->GetTopTriggerID();
            }
            if (triggerID == 0)
            {
                error("top TriggerID is 0 : eventType [%d]", topType);
            }
            else
            {
                subEffect->set_trigger_id(triggerID);
            }
            if (topType == EEventType::SomeActorDead)
            {
                Actor* pActor = triggerMgr->GetTopDeadActor();
                if (pActor == NULL)
                {
                    error("top Dead actor is NULL : eventType [%d]", topType);
                }
                else
                {
                    subEffect->set_triggerdeadactor_id(pActor->actorId);
                }
            }
        }
    }
}
//check if die send event
void CheckDeadEvt(ActorPtr caster, ActorPtr target, EffectType eType)
{
    bool isDead = !target->IsAlive();
    if (isDead && !target->DeadEvtRaised())
    {
        target->NotifyDeadEvtRaised();
        int killEnemyXpRecovery = caster->GetAttrByType(XpGainRate) * caster->GetAttrByType(KillSomeOneXpRecovery);
        if (killEnemyXpRecovery > 0)
        {
            if (caster->ChangeXp(killEnemyXpRecovery))
            {
                auto param = FillXpChangeParam(caster, target, 0, killEnemyXpRecovery);
                param.recovery = XpRecoveryType::KillEnemy;
                param.slainActorID = target->actorId;
                FillXpChangeCmd(param);
            }
        }

        caster->battleField->OnSomeOneKilled(target, caster);
        TriggerParam param;
        param.evtType = EEventType::SomeActorDead;
        param.c1.AppendUnique(caster);
        param.c2.AppendUnique(target);
        param.r2 = (int)eType;
        target->battleField->FireEvent(param);
    }
}

EffectCmd* CreateSubEffect(ActorPtr caster)
{
    TRACE("CreateSubEffect");
    EffectCmd* retEffect = NULL;
    RoundCmd* curRound = NULL;
    SkillCmd *curSkill = NULL;
    switch (caster->battleField->roundState)
    {
    case RS_Started:
        curRound = caster->battleField->curRoundCmd;
        if (curRound == NULL)
        {
            error("CreateSubEffect RoundCmd is NULL,RS_Started");
        }
        else
        {
            retEffect = curRound->add_prev_effect_cmds();
        }
        break;
    case RS_Attacking:
        curSkill = caster->battleField->curSkillCmd;
        if (curSkill == NULL)
        {
            error("CreateSubEffect SkillCmd is NULL,RS_Ended");
        }
        else
        {
            retEffect = curSkill->add_effect_cmds();
        }
        break;
    case RS_Ended:
        curRound = caster->battleField->curRoundCmd;
        if (curRound == NULL)
        {
            error("CreateSubEffect RoundCmd is NULL,RS_Ended");
        }
        else
        {
            retEffect = curRound->add_post_effect_cmds();
        }
        break;
    default:
        break;
    }
    return retEffect;
}
void FillDamageCmd(const FillDamageParam & param)
{
    TRACE("FillDamageCmd");
    EffectCmd *subEffect = CreateSubEffect(param.target);
    if (subEffect == NULL)
    {
        if (param.target->battleField->inited)
        {
            error("FillDamageCmd subEffect == NULL");
        }
        return;
    }
    subEffect->set_caster_id(param.caster->actorId);
    subEffect->set_target_id(param.target->actorId);
    subEffect->set_subeffect_id(param.subEffectId);
    subEffect->set_order(param.target->battleField->curOrder);
    subEffect->set_effect_type(param.showEffectType);
    if (param.buffId > 0)
    {
        subEffect->set_buff_meta_id(param.buffId);
    }
    EffectUpdateHp* pUpdateHp = subEffect->mutable_update_hp();
    pUpdateHp->set_update_hp(param.damage);
    if (!param.target->IsAlive())
    {
        pUpdateHp->set_is_dead(true);
    }
    if (param.resilience < 0)
    {
        pUpdateHp->set_resilience_hp(param.resilience);
    }
    if (param.bloodDrink >= 1)
    {
        pUpdateHp->set_blood_drinking(param.bloodDrink);
    }
    if (param.blockTherapy >= 1)
    {
        pUpdateHp->set_blocktherapy(param.blockTherapy);
    }
    if (param.strikeTherapy >= 1)
    {
        pUpdateHp->set_striketherapy(param.strikeTherapy);
    }
    if (param.s_damageTherapy >= 1)
    {
        pUpdateHp->set_s_damagetherapy(param.s_damageTherapy);
    }
    if (param.s_blockTherapy >= 1)
    {
        pUpdateHp->set_s_blocktherapy(param.s_blockTherapy);
    }
    if (param.s_strikeTherapy >= 1)
    {
        pUpdateHp->set_s_striketherapy(param.s_strikeTherapy);
    }
    for (int i = 0; i < MAX_SHIELD_INFO; i++)
    {
        if (param.pShieldInfo[i].buffID > 0)
        {
            EffectShield *pTargetShieldInfo = pUpdateHp->add_shield_infos();
            pTargetShieldInfo->set_buff_id(param.pShieldInfo[i].buffID);
            pTargetShieldInfo->set_damage_absorb(param.pShieldInfo[i].absorbValue);
            pTargetShieldInfo->set_is_over(param.pShieldInfo[i].isOver);
        }
    }
    FillTriggerEvt(subEffect, param.target, param.caster);
}

void FillRecoveryCmd(const FillRecoveryParam& param)
{
    TRACE("FillRecoveryCmd");
    EffectCmd *subEffect = CreateSubEffect(param.target);
    if (subEffect == NULL)
    {
        if (param.target->battleField->inited)
        {
            error("FillRecoveryCmd subEffect == NULL");
        }
        return;
    }
    subEffect->set_caster_id(param.caster->actorId);
    subEffect->set_target_id(param.target->actorId);
    subEffect->set_subeffect_id(param.subEffectId);
    subEffect->set_order(param.target->battleField->curOrder);
    subEffect->set_effect_type(param.showEffectType);
    if (param.originBuffID > 0)
    {
        subEffect->set_buff_meta_id(param.originBuffID);
    }
    EffectUpdateHp* pUpdateHp = subEffect->mutable_update_hp();
    pUpdateHp->set_update_hp(param.recovery);
    pUpdateHp->set_is_show(param.isShow);
    FillTriggerEvt(subEffect, param.target, param.caster);
}

void FillXpChangeCmd(const FillXpChangeParam& param)
{
    TRACE("FillXpChangeCmd");
    EffectCmd *subEffect = CreateSubEffect(param.target);
    if (subEffect == NULL)
    {
        if (param.target->battleField->inited)
        {
            error("FillXpChangeCmd subEffect == NULL");
        }
        return;
    }
    subEffect->set_caster_id(param.caster->actorId);
    subEffect->set_target_id(param.target->actorId);
    subEffect->set_subeffect_id(param.subEffectId);
    subEffect->set_order(param.target->battleField->curOrder);
    EffectUpdateXp* pUpdateXp = subEffect->mutable_update_xp();
    pUpdateXp->set_update_xp(param.xpChange);
    if (param.isShow)
    {
        pUpdateXp->set_is_show(param.isShow);
    }
    pUpdateXp->set_recoverytype(param.recovery);
    if (param.slainActorID > 0)
    {
        pUpdateXp->set_slainactorid(param.slainActorID);
    }
    if (param.originBuffID > 0)
    {
        subEffect->set_buff_meta_id(param.originBuffID);
    }
    FillTriggerEvt(subEffect, param.target, param.caster);
}

void FillDodgeCmd(ActorPtr caster, ActorPtr target, int subEffectId, int originBuffId)
{
    TRACE("FillDodgeCmd");
    EffectCmd *subEffect = CreateSubEffect(target);
    if (subEffect == NULL)
    {
        if (target->battleField->inited)
        {
            error("FillDodgeCmd subEffect == NULL");
        }
        return;
    }
    subEffect->set_caster_id(caster->actorId);
    subEffect->set_target_id(target->actorId);
    subEffect->set_subeffect_id(subEffectId);
    subEffect->set_order(target->battleField->curOrder);
    subEffect->set_effect_type(Dodge);
    if (originBuffId > 0)
    {
        subEffect->set_buff_meta_id(originBuffId);
    }
    FillTriggerEvt(subEffect, target,caster);
}

void FillBuffDetachCmd(ActorPtr caster, ActorPtr target, BuffVanishType vanishType, int subEffecID, int originBuffID, int detachBuffID)
{
    TRACE("FillBuffDetachCmd");
    EffectCmd *subEffect = CreateSubEffect(target);
    if (subEffect == NULL)
    {
        if (target->battleField->inited)
        {
            error("FillBuffDetachCmd subEffect == NULL");
        }
        return;
    }
    subEffect->set_caster_id(caster->actorId);
    subEffect->set_target_id(target->actorId);
    if (subEffecID > 0)
    {
        subEffect->set_subeffect_id(subEffecID);
    }
    if (originBuffID > 0)
    {
        subEffect->set_buff_meta_id(originBuffID);
    }
    subEffect->set_order(target->battleField->curOrder);
    EffectDetachBuff* pDetach = subEffect->mutable_detach_buff();
    pDetach->set_buff_meta_id(detachBuffID);
    pDetach->set_vanish_type(vanishType);
    FillTriggerEvt(subEffect, target,caster);
}

void FillBuffAttachCmd(ActorPtr caster, ActorPtr target, int originBuffId, int attachBuffID, int layerCnt, bool immune)
{
    TRACE("FillBuffAttachCmd");
    EffectCmd *subEffect = CreateSubEffect(target);
    if (subEffect == NULL)
    {
        if (target->battleField->inited)
        {
            error("FillBuffAttachCmd subEffect == NULL");
        }
        return;
    }
    subEffect->set_caster_id(caster->actorId);
    subEffect->set_target_id(target->actorId);
    if (originBuffId > 0)
    {
        subEffect->set_buff_meta_id(originBuffId);
    }
    subEffect->set_order(target->battleField->curOrder);
    if (immune)
    {
        subEffect->set_effect_type(pb::Immune);
    }
    
    EffectAttachBuff *pAttachBuff = subEffect->mutable_attach_buff();
    pAttachBuff->set_buff_meta_id(attachBuffID);
    pAttachBuff->set_layer_count(layerCnt);
    pAttachBuff->set_result(!immune);
    
    FillTriggerEvt(subEffect, target,caster);
}

void SelectEffectTargets(EffectSub* pEffectSub, int targetPos, ActorList* c1, ActorList *c2, Actor* buffCaster, Actor *buffOwner)
{
    BattleAreaPtr targetArea = NULL;
    auto pMeta = pEffectSub->GetEffectMeta();
    if (pMeta == nullptr)
    {
        return;
    }
    auto pCaster = pEffectSub->GetCaster();
    if (pCaster == nullptr)
    {
        return;
    }
    //--Choose whitch side--
    switch (pMeta->TargetType)
    {
    case Enemy:
        targetArea = pCaster->battleField->GetEnemyArea(pCaster->areaIndex);
        break;
    case Friend:
        targetArea = pCaster->battleField->GetOwnArea(pCaster->areaIndex);
        break;
    default:
        break;
    }
    
    if (targetArea == nullptr)
    {
        error("EffectSub::GetEffectTarget targetArea == NULL");
        return;
    }
    auto pMainTarget = targetArea->GetNewMainTarget(pCaster->posIndex, targetPos);
    if (pMainTarget == nullptr)
    {
        error("ActorSkill::GetTargetsByMainTarget mainTarget == NULL");
        return;
    }
    
    pEffectSub->InjectMainTarget(pMainTarget);

    //got the main target and to get the target list
    ActorList actorList;
    SelectEffectTargetsFilter1(pMeta->TargetFilter1, targetArea, pEffectSub, pCaster, pMainTarget, c1, c2, buffCaster, buffOwner, actorList);
    SelectEffectTargetsFilter2(pMeta->TargetFilter2, (pMeta->TargetParam == 0) ? 1 : pMeta->TargetParam, pCaster, actorList);
    SelectEffectTargetsFilter3(pMeta->TargetFilter3, targetArea, actorList);
    pEffectSub->InjectTargetList(actorList);
}

void SelectRowActors(BattleArea * pArea, ActorList & actorList, bool isFront)
{
    
    bool expectedRowHasActor = false;
    for (auto & pActor : pArea->actors)
    {
        if (pActor != nullptr && pActor->IsAlive())
        {
            bool valid = false;
            if (isFront)
            {
                valid = pActor->posIndex < 3;
            }
            else
            {
                valid = pActor->posIndex >= 3;
            }
            if (valid)
            {
                expectedRowHasActor = true;
            }
        }
    }
    for (auto & pActor : pArea->actors)
    {
        if (pActor != nullptr && pActor->IsAlive())
        {
            if (expectedRowHasActor)
            {
                bool valid = false;
                if (isFront)
                {
                    valid = pActor->posIndex < 3;
                }
                else
                {
                    valid = pActor->posIndex >= 3;
                }
                if (valid)
                {
                    actorList.AppendUnique(pActor);
                }
            }
            else
            {
                bool valid = false;
                if (isFront)
                {
                    valid = pActor->posIndex < 3;
                }
                else
                {
                    valid = pActor->posIndex >= 3;
                }
                if (!valid)
                {
                    actorList.AppendUnique(pActor);
                }
            }
        }
    }
}

//
void SelectEffectTargetsFilter1(const int filter1[], BattleArea * pArea, EffectSub* pEffectSub, Actor* pCaster, Actor* pMainTarget, ActorList* c1, ActorList *c2, Actor* buffCaster, Actor *buffOwner, ActorList &actorList)
{
    for (int i = 0; i < MAX_TARGET_FILTER_CNT; i++)
    {
        auto filter = filter1[i];
        if (filter == TargetFilter1::None)
        {
            break;
        }
        switch (filter)
        {
        case TargetFilter1::None:
            break;
        case TargetFilter1::FrontRowRel:
        {
            SelectRowActors(pArea, actorList, true);
            break;
        }
        case TargetFilter1::BackRowRel:
        {
            SelectRowActors(pArea, actorList, false);
            break;
        }
        case TargetFilter1::All:
        {
            for (auto & pActor : pArea->actors)
            {
                if (pActor != nullptr && pActor->IsAlive())
                {
                    actorList.AppendUnique(pActor);
                }
            }
            break;
        }
        case TargetFilter1::AllDead:
        {
            break;
        }
        case TargetFilter1::Self:
        {
            actorList.AppendUnique(pCaster);
            break;
        }
        case TargetFilter1::MainTarget:
        {
            actorList.AppendUnique(pMainTarget);
            break;
        }
        case TargetFilter1::C1:
        {
            if (c1 != nullptr)
            {
                for (auto & actor : c1->list)
                {
                    if (actor == nullptr)
                    {
                        continue;
                    }
                    actorList.AppendUnique(actor);
                }
            }
            break;
        }
        case TargetFilter1::C2:
        {
            if (c2 != nullptr)
            {
                for (auto & actor : c2->list)
                {
                    if (actor == nullptr)
                    {
                        continue;
                    }
                    actorList.AppendUnique(actor);
                }
            }
            break;
        }
        case TargetFilter1::BuffCaster:
        {
            if (buffCaster != nullptr)
            {
                actorList.AppendUnique(buffCaster);
            }
            break;
        }
        case TargetFilter1::BuffOwner:
        {
            if (buffOwner != nullptr)
            {
                actorList.AppendUnique(buffOwner);
            }
            break;
        }
        case TargetFilter1::OppoPos:
        {
            if (pArea != nullptr)
            {
                bool frontRowPrior = false;
                for (int i = 0; i < MAX_COLUMN_SIZE; i++)
                {
                    if (pArea->actors[i] != NULL_PTR && pArea->actors[i]->IsAlive())
                    {
                        frontRowPrior = true;
                        break;
                    }
                }
                auto casterPos = pCaster->posIndex;
                if (frontRowPrior && casterPos >= MAX_COLUMN_SIZE)
                {
                    casterPos = casterPos - MAX_COLUMN_SIZE;
                }
                if (pArea->actors[casterPos] != NULL_PTR && pArea->actors[casterPos]->IsAlive())
                {
                    actorList.AppendUnique(pArea->actors[casterPos]);
                }
                else
                {
                    if (casterPos < MAX_COLUMN_SIZE)
                    {
                        if (pArea->actors[FRONT_MIDDLE] != NULL_PTR && pArea->actors[FRONT_MIDDLE]->IsAlive())
                        {
                            actorList.AppendUnique(pArea->actors[FRONT_MIDDLE]);
                        }
                    }
                    else
                    {
                        if (pArea->actors[BACK_MIDDLE] != NULL_PTR && pArea->actors[BACK_MIDDLE]->IsAlive())
                        {
                            actorList.AppendUnique(pArea->actors[BACK_MIDDLE]);
                        }
                    }
                    for (int i = 0; i < BATTLE_POS_MAX; ++i)
                    {
                        if (pArea->actors[i] != NULL_PTR && pArea->actors[i]->IsAlive())
                        {
                            actorList.AppendUnique(pArea->actors[i]);
                            break;
                        }
                    }
                }
            }
            break;
        }case TargetFilter1::BackOne:
        {
            //特殊处理//
            if (pMainTarget->posIndex < MAX_COLUMN_SIZE)
            {
                bool backTargetValid = false;
                for (int i = 3; i < 5; i++)
                {
                    ActorPtr backActor = pArea->GetActorByPos(i);
                    if (backActor != NULL)
                    {
                        backTargetValid = true;
                        break;
                    }
                }
                if (backTargetValid)
                {
                    int targetIndex = pMainTarget->posIndex + MAX_COLUMN_SIZE;
                    ActorPtr backActor = pArea->GetActorByPos(targetIndex);
                    if (backActor != NULL)
                    {
                        pMainTarget = backActor;
                    }
                    else
                    {
                        bool largerTargetValid = false;
                        for (int i = targetIndex + 1; i < BATTLE_POS_MAX; i++)
                        {
                            backActor = pArea->GetActorByPos(i);
                            if (backActor != NULL)
                            {
                                pMainTarget = backActor;
                                largerTargetValid = true;
                                break;
                            }
                        }
                        if (!largerTargetValid)
                        {
                            for (int i = MAX_COLUMN_SIZE; i < BATTLE_POS_MAX; i++)
                            {
                                backActor = pArea->GetActorByPos(i);
                                if (backActor != NULL)
                                {
                                    pMainTarget = backActor;
                                }
                            }
                        }
                    }
                }
                pEffectSub->InjectMainTarget(pMainTarget);
                actorList.AppendUnique(pMainTarget);
            }
            break;
        }
        default:
            break;
        }
    }
    
}

void SelectEffectTargetsFilter2(const int filter2[], int requiredCnt, Actor* pCaster,ActorList &actorList)
{
    if (filter2[0] == TargetFilter2::Current)
    {
        return;
    }
    ActorList filterList(actorList);
    actorList.Reset();
    for (int i = 0; i < MAX_TARGET_FILTER_CNT; i++)
    {
        auto filter = filter2[i];
        if (filter == TargetFilter2::Current)
        {
            break;
        }
        switch (filter)
        {
        case  TargetFilter2::HpMost:
        {
            SelectTopMostActors<SelectActorDataValue::Hp, GeneralOperator::Greater, GeneralOperator::Less>(filterList, requiredCnt, actorList);
            break;
        }
        case  TargetFilter2::HpLest:
        {
            SelectTopMostActors<SelectActorDataValue::Hp, GeneralOperator::Less, GeneralOperator::Greater>(filterList, requiredCnt, actorList);
            break;
        }
        case  TargetFilter2::XpMost:
        {
            SelectTopMostActors<SelectActorDataValue::Xp, GeneralOperator::Greater, GeneralOperator::Less>(filterList, requiredCnt, actorList);
            break;
        }
        case  TargetFilter2::XpLest:
        {
            SelectTopMostActors<SelectActorDataValue::Xp, GeneralOperator::Less, GeneralOperator::Greater>(filterList, requiredCnt, actorList);
            break;
        }
        case  TargetFilter2::OtherFirst:
        {
            auto tempList(filterList);
            tempList.Remove(pCaster);
            auto tempSize = tempList.GetSize();
            if (tempSize > requiredCnt)
            {
                auto excessive = tempSize - requiredCnt;
                for (int i = 0; i < excessive; i++)
                {
                    auto validIndex = 0;
                    auto randSize = tempSize - i;
                    if (randSize > 1)
                    {
                        validIndex = pCaster->battleField->random.RandForWidthFromZero(randSize);
                    }
                    tempList.RemoveByValidIndex(validIndex);
                }
                actorList.Merge(tempList);
            }
            else
            {
                actorList.Merge(tempList);
                actorList.AppendUnique(pCaster);
            }
            break;
        }
        case  TargetFilter2::Random:
        {
            auto filterSize = filterList.GetSize();
            if (filterSize > requiredCnt)
            {
                auto excessive = filterSize - requiredCnt;
                auto tempList(filterList);
                for (int i = 0; i < excessive; i++)
                {
                    auto validIndex = 0;
                    auto randSize = filterSize - i;
                    if (randSize > 1)
                    {
                        validIndex = pCaster->battleField->random.RandForWidthFromZero(randSize);
                    }
                    tempList.RemoveByValidIndex(validIndex);
                }
                actorList.Merge(tempList);
            }
            else
            {
                actorList.Merge(filterList);
            }
            break;
        }
        case TargetFilter2::XpLestOtherFirst:{
            auto tempList(filterList);
            tempList.Remove(pCaster);
            SelectTopMostActors<SelectActorDataValue::Xp, GeneralOperator::Less, GeneralOperator::Greater>(tempList, requiredCnt, actorList);
            auto tempSize = tempList.GetSize();
            if (tempSize < requiredCnt)
            {
                actorList.Merge(tempList);
                actorList.AppendUnique(pCaster);
            }
            break;
        }
        default:
            break;
        }
    }
}
void AddBehindTargetActor(BattleArea * pArea, Actor* target, ActorList &actorList)
{
    if (target != nullptr && pArea != nullptr)
    {
        auto rowIndex = target->posIndex / MAX_COLUMN_SIZE;
        if (rowIndex == 0)
        {
            auto actor = pArea->GetActorByPos(target->posIndex + MAX_COLUMN_SIZE);
            actorList.AppendUnique(actor);
        }
    }
}
void AddFrontTargetActor(BattleArea * pArea, Actor* target, ActorList &actorList)
{
    if (target != nullptr && pArea != nullptr)
    {
        auto rowIndex = target->posIndex / MAX_COLUMN_SIZE;
        if (rowIndex == 1)
        {
            auto actor = pArea->GetActorByPos(target->posIndex - MAX_COLUMN_SIZE);
            actorList.AppendUnique(actor);
        }
    }
}
void AddLeftTargetActor(BattleArea * pArea, Actor* target, ActorList &actorList)
{
    if (target != nullptr && pArea != nullptr)
    {
        auto colIndex = target->posIndex % MAX_COLUMN_SIZE;
        if (colIndex != 0)
        {
            auto actor = pArea->GetActorByPos(target->posIndex - 1);
            actorList.AppendUnique(actor);
        }
    }
}
void AddRightTargetActor(BattleArea * pArea, Actor* target, ActorList &actorList)
{
    if (target != nullptr && pArea != nullptr)
    {
        auto colIndex = target->posIndex % MAX_COLUMN_SIZE;
        if (colIndex != MAX_COLUMN_SIZE - 1)
        {
            auto actor = pArea->GetActorByPos(target->posIndex + 1);
            actorList.AppendUnique(actor);
        }
    }
}
void SelectEffectTargetsFilter3(const int filter3[], BattleArea * pArea, ActorList &actorList)
{
    if (filter3[0] == TargetFilter3::Current)
    {
        return;
    }
    ActorList filterList(actorList);
    actorList.Reset();
    for (int i = 0; i < MAX_TARGET_FILTER_CNT; i++)
    {
        auto filter = filter3[i];
        if (filter == TargetFilter3::Current)
        {
            break;
        }
        switch (filter)
        {
        case TargetFilter3::TargetRow:
        {
            bool rowValids[MAX_ROW_SIZE_NEW] = { false, false };
            for (auto & actor : filterList.list)
            {
                if (actor == nullptr)
                {
                    continue;
                }
                auto index = actor->posIndex / MAX_COLUMN_SIZE;
                rowValids[index] = true;
            }
            for (auto & actor : pArea->actors)
            {
                if (actor != nullptr && actor->IsAlive())
                {
                    auto index = actor->posIndex / MAX_COLUMN_SIZE;
                    if (rowValids[index])
                    {
                        actorList.AppendUnique(actor);
                    }
                }
            }
            break;
        }
        case TargetFilter3::TargetColumn:
        {
            bool columnValids[MAX_COLUMN_SIZE] = { false, false, false };
            for (auto & actor : filterList.list)
            {
                if (actor == nullptr)
                {
                    continue;
                }
                auto index = actor->posIndex % MAX_COLUMN_SIZE;
                columnValids[index] = true;
            }
            for (auto & actor : pArea->actors)
            {
                if (actor != nullptr && actor->IsAlive())
                {
                    auto index = actor->posIndex % MAX_COLUMN_SIZE;
                    if (columnValids[index])
                    {
                        actorList.AppendUnique(actor);
                    }
                }
            }
            break;
        }
        case TargetFilter3::TargetAround:
        {
            for (auto & actor : filterList.list)
            {
                if (actor == nullptr)
                {
                    continue;
                }
                actorList.AppendUnique(actor);
                AddFrontTargetActor(pArea, actor, actorList);
                AddBehindTargetActor(pArea, actor, actorList);
                AddLeftTargetActor(pArea, actor, actorList);
                AddRightTargetActor(pArea, actor, actorList);
            }
            break;
        }
        case TargetFilter3::TargetLR:
        {
            for (auto & actor : filterList.list)
            {
                if (actor == nullptr)
                {
                    continue;
                }
                AddLeftTargetActor(pArea, actor, actorList);
                AddRightTargetActor(pArea, actor, actorList);
            }
            break;
        }
        case TargetFilter3::TargetBehind:
        {
            for (auto & actor : filterList.list)
            {
                if (actor == nullptr)
                {
                    continue;
                }
                AddBehindTargetActor(pArea, actor, actorList);
            }
            break;
        }
        case TargetFilter3::TargetFront:
        {
            for (auto & actor : filterList.list)
            {
                if (actor == nullptr)
                {
                    continue;
                }
                AddFrontTargetActor(pArea, actor, actorList);
            }
            break;
        }
        case TargetFilter3::TargetAndLR:
        {
            for (auto & actor : filterList.list)
            {
                if (actor == nullptr)
                {
                    continue;
                }
                actorList.AppendUnique(actor);
                AddLeftTargetActor(pArea, actor, actorList);
                AddRightTargetActor(pArea, actor, actorList);
            }
            break;
        }
        default:
            break;
        }
    }
}



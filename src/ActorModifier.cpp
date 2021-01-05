#include "ActorModifier.h"
#include "ActorModifierSub.h"
#include "BattleField.h"
#include "Actor.h"
#include "ModifierMeta.h"
#include "Random.h"
#include "Enum.h"
#include "Log.h"
#include <stdio.h>
#include "EffectUtil.h"
#include "ModifierUtils.h"

ActorModifier::ActorModifier()
{
    TRACE("ActorModifier::ActorModifier");
    owner = NULL_PTR;
    memset(modSubArray, 0, sizeof(ActorModifierSubPtr)*MAX_MOD_COUNT);
    restrictAction = 0;
    restrictXpSkill = 0;
    restrictCommonSkill = 0;
    sealedLayer = 0;
    legendSkillActive = 0;
}

ActorModifier::~ActorModifier()
{
    TRACE("ActorModifier::~ActorModifier");
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        if (modSubArray[i] != NULL)
        {
            delete modSubArray[i];
        }
        modSubArray[i] = NULL;
    }
}

void ActorModifier::InitOwner(ActorPtr actor)
{
     owner = actor;
}


void ActorModifier::AddMod(ActorPtr caster, int modId, int level, int round)
{
    TRACE("ActorModifier::AddMod caster[%d] modId[%d] round[%d]",caster->actorId, modId,round);
    const ModifierMeta* modToAdd = ModifierMetaSet::instance()->Get(modId);
    if (modToAdd == NULL)
    {
        error("no buff id :%d\n", modId);
        return;
    }
    for (auto groupID : modToAdd->GroupId)
    {
        if (groupID == 0)
        {
            break;
        }
        if (CheckIsByImmune(caster, groupID))
        {
            debug("mod is immune : actor: %d, modId: %d\n", caster->actorId, modToAdd->id);
            FillBuffAttachCmd(caster, owner, modId, modId, 0, true);
            return;
        }
    }
    
    if (modToAdd->MaxLayer <= 0)
    {
        error("mod maxlayer is 0!! actor: %d, modId: %d\n", caster->actorId, modToAdd->id);
        return;
    }
    if (modToAdd->MaxLayer <= GetModLayerCount(modId))
    {
        //exchange
        for (int i = 0; i < MAX_MOD_COUNT; ++i)
        {
            ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
            if (modSub != NULL)
            {
                ModifierMeta* modInList = modSub->modMeta;
                if(modToAdd->id == modInList->id)
                {
                    TriggerParam param;
                    param.evtType = EEventType::BuffOver;
                    param.c1.list[0] = modSub->owner;
                    param.c2.list[0] = modSub->casterIn;
                    param.pBuff = modSub;
                    param.normalBuffOver = false;
                    modSub->owner->battleField->FireEvent(param);
                    FillBuffDetachCmd(caster,owner, BuffVanishType::MaxLayerLimit,0,0, modSub->modMeta->id);
                    RemoveMod(modSub);
                    break;
                }
            }
        }
        debug("mod is maxLayer : actor: %d, modId: %d, layerCount : %d\n", caster->actorId, modToAdd->id, modToAdd->MaxLayer);
    }
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        if (modSubArray[i] == NULL)
        {
            modSubArray[i] = new ActorModifierSub(caster, modId, owner, level, round);
            if (modToAdd->LastRound <= 0)
            {
                TriggerParam param;
                param.evtType = EEventType::BuffOver;
                param.c1.list[0] = owner;
                param.c2.list[0] = caster;
                param.pBuff = modSubArray[i];
                param.normalBuffOver = false;
                owner->battleField->FireEvent(param);
                FillBuffDetachCmd(caster,owner, BuffVanishType::LastZeroRound,0,0, modSubArray[i]->modMeta->id);
                RemoveMod(modSubArray[i]);
            }
            return;
        }
    }
    warning("max mod is not enough!!!");
}

void ActorModifier::ModEffect(PeriodType sType,int triggerArea)
{
    TRACE("ActorModifier::ModEffect PeriodType[%d] triggerArea[%d]", sType, triggerArea);
    //sorted by priority first .TODO.
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
        if (modSub != NULL)
        {
            modSub->DoEffect(sType, triggerArea);
        }
    }
}

void ActorModifier::RoundCount()
{
    TRACE("ActorModifier::RoundCount");
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        ActorModifierSubPtr sub = GetValidModSubByIndex(i);
        if (sub != NULL_PTR)
        {
            bool roundCnt = false;
            if (sub->modMeta->DuraType == CasterRoundStart && owner->battleField->roundState == RS_Started)
            {
                if (sub->casterIn->areaIndex == owner->battleField->lastAttackArea)
                {
                    roundCnt = true;
                }
            }
            else if (sub->modMeta->DuraType == TargetRoundStart && owner->battleField->roundState == RS_Started)
            {
                if (sub->owner->areaIndex == owner->battleField->lastAttackArea)
                {
                    roundCnt = true;
                }
            }
            else if (sub->modMeta->DuraType == TargetRoundEnd && owner->battleField->roundState == RS_Ended)
            {
                if (sub->owner->areaIndex == owner->battleField->lastAttackArea)
                {
                    roundCnt = true;
                }
            }
            if (roundCnt && sub->GetAttachRound() != owner->battleField->activeRoundCount)
            {
                sub->remainActionCount--;
                if (sub->remainActionCount <= 0)
                {
                    TriggerParam param;
                    param.evtType = EEventType::BuffOver;
                    param.c1.list[0] = sub->owner;
                    param.c2.list[0] = sub->casterIn;
                    param.pBuff = sub;
                    param.normalBuffOver = true;
                    sub->owner->battleField->FireEvent(param);
                    FillBuffDetachCmd(owner, owner, BuffVanishType::Expired, 0, 0, sub->modMeta->id);
                    RemoveMod(sub);
                }
            }
        }
    }
}

void ActorModifier::RemoveMod(ActorModifierSubPtr mod)
{
    TRACE("ActorModifier::RemoveMod ownerID[%d] modID[%d]", owner->actorId, mod->modMeta->id);
    mod->DoDetach();
    mod->MarkDeleted();
}

bool ActorModifier::IsRestrictAction()
{
    if (restrictAction > 0)
    {
        return true;
    }
    return false;
}

bool ActorModifier::IsRestrictCommonSkill()
{
    if (restrictCommonSkill > 0)
    {
        return true;
    }
    return false;
}

bool ActorModifier::IsSealedState()
{
    if (sealedLayer > 0)
    {
        return true;
    }
    return false;
}

float ActorModifier::TriggerShieldDamage(ActorPtr caster, float damage, int damageID, ShieldInfo *pShieldInfo)
{
    TRACE("ActorModifier::TriggerShieldDamage damageID[%d]",damageID);
    float realDamage = damage;
    //TODO: NOW ONLY CHECK ONE SHIELD
    int shieldIndex = 0;
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
        if (modSub != NULL && modSub->modMeta->LastEffect == BuffShield)
        {
            realDamage = damage - modSub->shieldDamage;
            ShieldInfo *curShieldInfo = NULL;
            if (shieldIndex >= MAX_SHIELD_INFO)
            {
                error("ActorModifier::TriggerShieldDamage shieldIndex >= MAX_SHIELD_INFO");
            }
            else
            {
                curShieldInfo = &pShieldInfo[shieldIndex];
            }
            if (realDamage > 0)
            {
                if (curShieldInfo != NULL)
                {
                    curShieldInfo->absorbValue = modSub->shieldDamage;
                    curShieldInfo->buffID = modSub->modMeta->id;
                    curShieldInfo->isOver = 1;
                }
                TriggerParam param;
                param.evtType = EEventType::BuffOver;
                param.c1.list[0] = modSub->owner;
                param.c2.list[0] = modSub->casterIn;
                param.pBuff = modSub;
                param.normalBuffOver = false;
                modSub->owner->battleField->FireEvent(param);
                RemoveMod(modSub);
            }
            else
            {
                if (realDamage == 0)
                {
                    if (curShieldInfo != NULL)
                    {
                        curShieldInfo->absorbValue = damage;
                        curShieldInfo->buffID = modSub->modMeta->id;
                        curShieldInfo->isOver = 1;
                    }
                    TriggerParam param;
                    param.evtType = EEventType::BuffOver;
                    param.c1.list[0] = modSub->owner;
                    param.c2.list[0] = modSub->casterIn;
                    param.pBuff = modSub;
                    param.normalBuffOver = false;
                    modSub->owner->battleField->FireEvent(param);
                    RemoveMod(modSub);
                }
                else
                {
                    if (curShieldInfo != NULL)
                    {
                        curShieldInfo->absorbValue = damage;
                        curShieldInfo->buffID = modSub->modMeta->id;
                        curShieldInfo->isOver = 0;
                    }
                    modSub->shieldDamage = -realDamage;
                    realDamage = 0;
                }
                break;
            }
            damage = realDamage;
            shieldIndex++;
        }
    }
    return realDamage;
}

bool ActorModifier::CheckIsByImmune(ActorPtr caster, int groupId)
{
    TRACE("ActorModifier::CheckIsByImmune caster[%d] groupID[%d]", caster->actorId, groupId);
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
        if (modSub != NULL && modSub->modMeta->LastEffect == LastEffectType::Immune)
        {
            for (int j = 0; j < MAX_DMG_PARAM; ++j)
            {
                if(modSub->modMeta->LastParam[j] == groupId)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void FillModGroupCmd(ActorPtr caster, ActorPtr owner, BuffVanishType vanish, int triggerID,int buffID)
{
    if (vanish == BuffVanishType::RmByDispel)
    {
        FillBuffDetachCmd(caster, owner, vanish, triggerID, 0, buffID);
    }
    else if (vanish == BuffVanishType::Immune)
    {
        FillBuffDetachCmd(caster, owner, vanish, 0, triggerID, buffID);
    }
    else
    {
        error("not support BuffVanishType type[%d] by func RemoveModByGroupId", vanish);
    }
}
void ActorModifier::RemoveModByGroupId(ActorPtr caster, int groupId, int layerCount, BuffVanishType vanish, int triggerID)
{
    TRACE("ActorModifier::RemoveModByGroupId caster[%d] groupID[%d] layerCount[%d] BuffVanishType[%d] subEffect[%d]", caster->actorId, groupId, layerCount, vanish, triggerID);
    if (layerCount == 0)
    {
        for (int i = 0; i < MAX_MOD_COUNT; ++i)
        {
            ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
            if (modSub != NULL)
            {
                for (auto curGroupID : modSub->modMeta->GroupId)
                {
                    if (curGroupID == 0)
                    {
                        break;
                    }
                    if (curGroupID == groupId)
                    {
                        TriggerParam param;
                        param.evtType = EEventType::BuffOver;
                        param.c1.list[0] = modSub->owner;
                        param.c2.list[0] = modSub->casterIn;
                        param.pBuff = modSub;
                        param.normalBuffOver = false;
                        modSub->owner->battleField->FireEvent(param);
                        FillModGroupCmd(caster, owner, vanish, triggerID, modSub->modMeta->id);
                        RemoveMod(modSub);
                    }
                }
            }
        }
    }
    else
    {
        int removeCount = 0;
        for (int i = 0; i < MAX_MOD_COUNT; ++i)
        {
            ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
            if (modSub != NULL)
            {
                for (auto curGroupID : modSub->modMeta->GroupId)
                {
                    if (curGroupID == 0)
                    {
                        break;
                    }
                    if (curGroupID == groupId)
                    {
                        TriggerParam param;
                        param.evtType = EEventType::BuffOver;
                        param.c1.list[0] = modSub->owner;
                        param.c2.list[0] = modSub->casterIn;
                        param.pBuff = modSub;
                        param.normalBuffOver = false;
                        modSub->owner->battleField->FireEvent(param);
                        FillModGroupCmd(caster, owner, vanish, triggerID, modSub->modMeta->id);
                        RemoveMod(modSub);
                        removeCount++;
                    }
                }
                if (removeCount == layerCount)
                {
                    break;
                }
            }
        }
    }
}

void ActorModifier::RemoveModByType(ActorPtr caster, BuffType bType, int subEffectID)
{
    TRACE("ActorModifier::RemoveModByType caster[%d] BuffType[%d] subEffectID[%d]",caster->actorId,bType,subEffectID);
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
        if (modSub != NULL && modSub->modMeta->BuffType == bType)
        {
            TriggerParam param;
            param.evtType = EEventType::BuffOver;
            param.c1.list[0] = modSub->owner;
            param.c2.list[0] = modSub->casterIn;
            param.pBuff = modSub;
            param.normalBuffOver = false;
            modSub->owner->battleField->FireEvent(param);
            FillBuffDetachCmd(caster,owner, BuffVanishType::RmByType,subEffectID,0, modSub->modMeta->id);
            RemoveMod(modSub);
        }
    }
}


void ActorModifier::RemoveModByTrigger(ActorPtr caster,ActorModifierSubPtr modSub)
{
    TriggerParam param;
    param.evtType = EEventType::BuffOver;
    param.c1.list[0] = modSub->owner;
    param.c2.list[0] = modSub->casterIn;
    param.pBuff = modSub;
    param.normalBuffOver = false;
    modSub->owner->battleField->FireEvent(param);
    FillBuffDetachCmd(caster,owner, BuffVanishType::RmByTrigger, 0,0, modSub->modMeta->id);
    RemoveMod(modSub);
}


int ActorModifier::GetModCntByGroupID(int groupId)
{
    TRACE("ActorModifier::IsModExist groupID[%d]", groupId);
    int cnt = 0;
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
        if (modSub != NULL)
        {
            for (auto curGroupID : modSub->modMeta->GroupId)
            {
                if (curGroupID == 0)
                {
                    break;
                }
                if (curGroupID == groupId)
                {
                    cnt++;
                    break;
                }
            }
        }
    }
    return cnt;
}

int ActorModifier::GetModLayerCount(int modId)
{
    TRACE("ActorModifier::GetModLayerCount", modId);
    int layerCount = 0;
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
        if (modSub != NULL && modSub->modMeta->id == modId)
        {
            layerCount++;
        }
    }
    return layerCount;
}

bool ActorModifier::IsLegendSkillActive()
{
    return legendSkillActive > 0;
}

void ActorModifier::CheckDeleteMods()
{
    TRACE("ActorModifier::CheckDeleteMods ");
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        if (modSubArray[i] != NULL && modSubArray[i]->deleteMark)
        {
            delete modSubArray[i];
            modSubArray[i] = NULL;
        }
    }
}

ActorModifierSubPtr ActorModifier::GetValidModSubByIndex(int index)
{
    if (index < 0 || index >= MAX_MOD_COUNT)
    {
        return NULL;
    }
    if (modSubArray[index] != NULL && !modSubArray[index]->deleteMark)
    {
        return modSubArray[index];
    }
    return NULL;
}

void ActorModifier::AppendTempAffix(BuffAffixApply affixType, int skillLevel, TempAttrInfo &tempAttr)
{
    TRACE("ActorModifier::AppendTempAffix ");
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
        if (modSub != NULL && modSub->modMeta->AffixType == affixType)
        {
            int affixPower = modSub->modMeta->AffixPower[0] + (skillLevel - 1) * modSub->modMeta->AffixPower[1];
            ApplyAffix(owner, modSub->modMeta->Affix, affixPower, tempAttr);
        }
    }
}

void ActorModifier::AccquireAffixPowerInfo(BuffAffixApply affixType, int skillLevel, TempAffixPowerInfo& tempInfo)
{
    TRACE("ActorModifier::AccquireAffixPowerInfo ");
    for (int i = 0; i < MAX_MOD_COUNT; ++i)
    {
        ActorModifierSubPtr modSub = GetValidModSubByIndex(i);
        if (modSub != NULL && modSub->modMeta->AffixType == affixType)
        {
            int affixPower = modSub->modMeta->AffixPower[0] + (skillLevel - 1) * modSub->modMeta->AffixPower[1];
            tempInfo.push_back(TempAffixPowerInfo::value_type(modSub->modMeta->Affix, affixPower));
        }
    }
}

void ActorModifier::RemoveModByImmune(ActorPtr caster, int groupId, int layerCount, int triggerID)
{
    RemoveModByGroupId(caster, groupId, layerCount, BuffVanishType::Immune, triggerID);
}

void ActorModifier::RemoveModByDispel(ActorPtr caster, int groupId, int layerCount, int triggerID)
{
    RemoveModByGroupId(caster, groupId, layerCount, BuffVanishType::RmByDispel, triggerID);
}

#include "Actor.h"
#include "ActorAttrs.h"
#include "NewActorAttrs.h"
#include "ActorDatas.h"
#include "ActorSkill.h"
#include "ActorModifier.h"
#include "BattleField.h"
#include "ActorSkillSub.h"
#include "ModifierUtils.h"
#include "BattleParams.pb.h"
#include "Type.h"
#include "Enum.h"
#include "Log.h"
#include <stdio.h>

Actor::Actor(BattleFieldPtr bf)
    :actorAttrs(bf)
    , deadEvtRaised(false)
{
    battleField = bf;
    activeRound = 0;
    metaData = NULL;
}

void Actor::InitActor(const pb::InitActorInfo &actorInfo)
{
    info("init actor : %d\n", actorId);
    battleType = (int)battleField->GetBattleType();
    deadEvtRaised = false;
    posIndex = actorInfo.pos_index();
    areaIndex = actorInfo.area_index();
    templateId = actorInfo.meta_id();
    metaData = ActorMetaSet::instance()->Get(templateId);
    heroId = actorInfo.hero_id();
    fashionId = actorInfo.fashionid();
    level = actorInfo.level();
    awake = actorInfo.awake();
    advance = actorInfo.advance();
    halo = actorInfo.halo();
    
    name = actorInfo.name();
    for (int i = 0; i < actorInfo.attr_infos_size(); ++i)
    {
        actorAttrs.InitAttr(actorInfo.attr_infos(i));
    }
    int initXp = (int)actorAttrs.GetAttrByType(XpInit);

    if(actorInfo.xp()>=0){
        initXp = actorInfo.xp();
    }
    
    int xpMax = (int)actorAttrs.GetAttrByType(XpMax);
    
    if (initXp > xpMax)
    {
        initXp = xpMax;
    }
    double hpMax = actorAttrs.GetAttrByType(HpMax) * (1 + actorAttrs.GetAttrByType(HpAttrPercent)) * (1 + actorAttrs.GetAttrByType(HpSkillPercent)) * (1 + actorAttrs.GetAttrByType(LevelHpPercentAddition));
    double initHp = hpMax;
    if(actorInfo.hp() >=0){
        initHp = actorInfo.hp();
    }
    
    if (initHp > hpMax)
    {
        initHp = hpMax;
    }
    activeRound = actorInfo.activeround() + 1;
    actorDatas.Init(this,initHp, initXp, hpMax,xpMax);
    actorSkill = new ActorSkill();
    actorMod = new ActorModifier();
    actorSkill->InitOwner(this);
    actorMod->InitOwner(this);
    
    actionState = NoneState;
    
    IsImmuneThisTime = false;
    
    analysisActorInfo = NULL_PTR;
    
    //memset(cacheSkill, 0, sizeof(SkillData)*MAX_CACHE_SKILL_DATA);
    //ClearCacheAffix();
//    memset(cacheAffix, 0, sizeof(AffixData)*MAX_CACHE_AFFIX_DATA);
}

Actor::~Actor()
{
    TRACE("~actor %d\n", actorId);
    //ClearCacheAffix();
    if (actorMod != NULL)
    {
        delete actorMod;
        actorMod = NULL;
    }
    if (actorSkill != NULL)
    {
        delete actorSkill;
        actorSkill = NULL;
    }
    if (analysisActorInfo != 0)
    {
        analysisActorInfo->Clear();
    }
}

bool Actor::IsAlive()
{
    return actorDatas.IsAlive() && (activeRound <= battleField->activeRoundCount);
}

void Actor::ReAlive()
{
    actorDatas.ReAlive();
}

void Actor::CastSkillByType(SkillType sType, int targetPos)
{
    actionState = CastSkill;
    actorSkill->CastSkillByType(sType, targetPos);
}

bool Actor::ChangeXp(int param)
{
    TRACE("Actor::ChangeXp actor[%d] param[%d]", actorId, param);
    if (actorMod->IsSealedState())
    {
        debug("actor : %d  is seal\n", actorId);
        return false;
    }
    actorDatas.ChangeXp(param);
    return true;
}

void Actor::ChangeHp(double param)
{
    actorDatas.ChangeHp(param);
}

double Actor::GetAttrByType(ActorAttrType dType)
{
    return actorAttrs.GetAttrByType(dType);
}
//
//void Actor::AddCacheSkill(int skillId, int skillLv)
//{
//    for (int i = 0; i < MAX_CACHE_SKILL_DATA; ++i)
//    {
//        if (!cacheSkill[i].isSet)
//        {
//            info("add cache skill skillId : %d, actorId : %d\n", skillId, actorId);
//            cacheSkill[i].isSet = true;
//            cacheSkill[i].skillId = skillId;
//            cacheSkill[i].skillLv = skillLv;
//            break;
//        }
//    }
//}
//
//void Actor::CheckCacheSkill()
//{
//    TRACE("Actor::CheckCacheSkill actor[%d]", actorId);
//    for (int i = 0; i < MAX_CACHE_SKILL_DATA; ++i)
//    {
//        if (cacheSkill[i].isSet)
//        {
//            ActorSkillSubPtr cs = new ActorSkillSub(actorSkill);
//            if (cs->InitSkill(cacheSkill[i].skillId, cacheSkill[i].skillLv,true))
//            {
//                actionState = Extra;
//                battleField->InitOneReplayCmd();
//                info("Cache !!! Skill skillId : %d, actorId : %d\n", cacheSkill[i].skillId, actorId);
//                cs->DoSkill(NULL_PTR);
//            }
//            delete cs;
//            cacheSkill[i].isSet = false;
//        }
//    }
//    memset(cacheSkill, 0, sizeof(SkillData)*MAX_CACHE_SKILL_DATA);
//}
//
//void Actor::AddCacheAffix(int affixId)
//{
//    TRACE("Actor::AddCacheAffix actorID[%d] affixID[%d]", actorId, affixId);
//    for (int i = 0; i < MAX_CACHE_AFFIX_DATA; ++i)
//    {
//        if (!cacheAffix[i].isSet)
//        {
//            cacheAffix[i].isSet = true;
//            cacheAffix[i].affixId = affixId;
////            memset(&cacheAffix[i].modMaps, 0, sizeof(map <ActorAttrType, int>));
//
//            cacheAffix[i].modMaps.clear();
//            return;
//        }
//    }
//    error("cache affix is full!!! actorId: %d", actorId);
//}
//
//void Actor::ApplyCacheAffix()
//{
//    TRACE("Actor::ApplyCacheAffix");
//    for (int i = 0; i < MAX_CACHE_AFFIX_DATA; ++i)
//    {
//        if (cacheAffix[i].isSet)
//        {
//            //ApplyAffix(this, cacheAffix[i].affixId,cacheAffix[i].affixPower, cacheAffix[i].modMaps);
//        }
//    }
//}
//
//void Actor::RemoveCacheAffix()
//{
//    TRACE("Actor::RemoveCacheAffix");
//    for (int i = 0; i < MAX_CACHE_AFFIX_DATA; ++i)
//    {
//        if (cacheAffix[i].isSet)
//        {
//            cacheAffix[i].isSet = false;
//            //RemoveAffix(this, cacheAffix[i].modMaps);
//        }
//    }
//    ClearCacheAffix();
////    memset(cacheAffix, 0, sizeof(AffixData)*MAX_CACHE_AFFIX_DATA);
//}

//void Actor::ClearCacheAffix()
//{
//    TRACE("Actor::ClearCacheAffix");
//    for (int i = 0; i < MAX_CACHE_AFFIX_DATA; ++i)
//    {
//        cacheAffix[i].affixId = 0;
//        cacheAffix[i].affixPower = 0;
//        cacheAffix[i].modMaps.clear();
//    }
//}

bool Actor::AllowSkill()
{
    return actorSkill->AllowSkill();
}

bool Actor::AllowComAtk()
{
    return actorSkill->AllowComAtk();
}

ENationality Actor::GetNationality()
{
    if (metaData != NULL)
    {
        return metaData->Nationality;
    }
    return EN_None;
}

void Actor::CheckDeleteMods()
{
    actorMod->CheckDeleteMods();
}

bool Actor::DeadEvtRaised()
{
    return deadEvtRaised;
}

void Actor::NotifyDeadEvtRaised()
{
    deadEvtRaised = true;
}

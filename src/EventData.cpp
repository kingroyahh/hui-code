//
//  EventData.cpp
//  grav_test
//
//  Created by kingroy on 16/10/18.
//  Copyright © 2016年 Babeltime. All rights reserved.
//

#include "Type.h"
#include "Actor.h"
#include "EventData.h"
#include "ActorModifier.h"
#include "Log.h"
#include "EffectUtil.h"
#include "ActorSkillSub.h"
using namespace battle;
EventData::~EventData()
{
}

void EventData::ExecuteSubEffect(int subId)
{
    info("exeSubEffect casterid: %d, targetid: %d, subId: %d\n", caster->actorId, target->actorId, subId);
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(subId);
    if (subMeta != NULL_PTR)
    {
        //TODO : BUFF LV
        //makeEffectByType(owner, owner, subMeta, 0, 1, 0,);
    }
    else
    {
        error("subEffect meta cant find: %d\n", subId);
    }
}

void EventData::ExecuteSkill(int skillId)
{
////    info("exeSkill casterid: %d, targetid: %d, skillId: %d\n", caster->actorId, target->actorId, skillId);
//    if (skillId != 0)
//    {
//        ActorSkillSubPtr cs = new ActorSkillSub(owner->actorSkill);
//        if (cs->InitSkill(skillId, 1,true))
//        {
//            owner->actionState = CastSkill;
//            cs->DoSkill(NULL_PTR);
//        }
//        owner->actionState = NoneState;
//        delete cs;
//    }
}

void EventData::Executebuff(int modId)
{
    info("exeBuff casterid: %d, targetid: %d, modId: %d\n", caster->actorId, target->actorId, modId);
    //owner->actorMod->AddMod(owner, modId, 1, -1);
}

void EventData::ExecuteSubForCaster(int subId)
{
    info("exeSubForCaster casterid: %d, targetid: %d, subId: %d\n", caster->actorId, target->actorId, subId);
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(subId);
    if (subMeta != NULL_PTR)
    {
        //TODO : BUFF LV
        //makeEffectByType(owner, caster, subMeta, 0, 1, 0);
    }
    else
    {
        error("subEffect meta cant find: %d\n", subId);
    }
}

void EventData::ExecuteBuffForCaster(int modId)
{
    info("exeBuffForCaster casterid: %d, targetid: %d, modId: %d\n", caster->actorId, target->actorId, modId);
    //caster->actorMod->AddMod(owner, modId, 1, -1);
}

void EventData::ExecuteSubForTarget(int subId)
{
    info("exeSubForTarget casterid: %d, targetid: %d, subId: %d\n", caster->actorId, target->actorId, subId);
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(subId);
    if (subMeta != NULL_PTR)
    {
        //TODO : BUFF LV
        //makeEffectByType(owner, target, subMeta, 0, 1, 0);
    }
    else
    {
        error("subEffect meta cant find: %d\n", subId);
    }
}

void EventData::ExecuteBuffForTarget(int modId)
{
    info("exeBuffForTarget casterid: %d, targetid: %d, modId: %d\n", caster->actorId, target->actorId, modId);
   // target->actorMod->AddMod(owner, modId, 1, -1);
}

void EventData::ExecuteSelfApplyAffix(int affixId)
{
    info("exeApplyAffix ownerId: %d, affixId: %d\n", owner->actorId, affixId);
    //owner->AddCacheAffix(affixId);
}

void GetDamageEventData::ExecuteSubEffect(int subId)
{
    info("GetDamageEventData casterid: %d, targetid: %d, damage: %d, subId: %d\n", caster->actorId, target->actorId, damage, subId);
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(subId);
    if (subMeta != NULL_PTR)
    {
        //TODO : BUFF LV
        //makeEffectByType(owner, owner, subMeta, damage, 1, 0);
    }
    else
    {
        error("subEffect meta cant find: %d\n", subId);
    }
}

void GetDamageEventData::ExecuteSubForCaster(int subId)
{
    info("GetDamageEventData casterid: %d, targetid: %d, damage: %d, subId: %d\n", caster->actorId, target->actorId, damage, subId);
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(subId);
    if (subMeta != NULL_PTR)
    {
        //TODO : BUFF LV
        //makeEffectByType(owner, caster, subMeta, damage, 1, 0);
    }
    else
    {
        error("subEffect meta cant find: %d\n", subId);
    }
}

void GetDamageEventData::ExecuteSubForTarget(int subId)
{
    info("GetDamageEventData casterid: %d, targetid: %d, damage: %d, subId: %d\n", caster->actorId, target->actorId, damage, subId);
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(subId);
    if (subMeta != NULL_PTR)
    {
        //TODO : BUFF LV
       // makeEffectByType(owner, target, subMeta, damage, 1, 0);
    }
    else
    {
        error("subEffect meta cant find: %d\n", subId);
    }
}

void SetDamageEventData::ExecuteSubEffect(int subId)
{
    info("SetDamageEventData casterid: %d, targetid: %d, damage: %d, subId: %d\n", caster->actorId, target->actorId, damage, subId);
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(subId);
    if (subMeta != NULL_PTR)
    {
        //TODO : BUFF LV
       // makeEffectByType(owner, owner, subMeta, damage, 1, 0);
    }
    else
    {
        error("subEffect meta cant find: %d\n", subId);
    }
}

void SetDamageEventData::ExecuteSubForCaster(int subId)
{
    info("SetDamageEventData casterid: %d, targetid: %d, damage: %d, subId: %d\n", caster->actorId, target->actorId, damage, subId);
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(subId);
    if (subMeta != NULL_PTR)
    {
        //TODO : BUFF LV
        //makeEffectByType(owner, caster, subMeta, damage, 1, 0);
    }
    else
    {
        error("subEffect meta cant find: %d\n", subId);
    }
}

void SetDamageEventData::ExecuteSubForTarget(int subId)
{
    info("SetDamageEventData casterid: %d, targetid: %d, damage: %d, subId: %d\n", caster->actorId, target->actorId, damage, subId);
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(subId);
    if (subMeta != NULL_PTR)
    {
        //TODO : BUFF LV
        //makeEffectByType(owner, target, subMeta, damage, 1, 0);
    }
    else
    {
        error("subEffect meta cant find: %d\n", subId);
    }
}

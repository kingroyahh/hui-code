//
//  EventData.h
//  grav_test
//
//  Created by kingroy on 16/10/17.
//  Copyright © 2016年 Babeltime. All rights reserved.
//

#ifndef _GRAV_EVENT_DATA_H_
#define _GRAV_EVENT_DATA_H_
#include "Type.h"
#include "Enum.h"
#include "ModifierMeta.h"

class EventData
{
public:
    ActorPtr caster;
    ActorPtr target;
    ActorPtr owner;
    ModifierMeta* modMeta;
    
    virtual void ExecuteSubEffect(int subId);
    virtual void ExecuteSkill(int skillId);
    virtual void Executebuff(int modId);
    virtual void ExecuteSubForCaster(int subId);
    virtual void ExecuteBuffForCaster(int modId);
    virtual void ExecuteSubForTarget(int subId);
    virtual void ExecuteBuffForTarget(int modId);
    virtual void ExecuteSelfApplyAffix(int affixId);
    virtual ~EventData();
};

class GetDamageEventData : public EventData
{
public:
    int damage;
    DamageType dType;
    
    void ExecuteSubEffect(int subId) override;
    void ExecuteSubForCaster(int subId) override;
    void ExecuteSubForTarget(int subId) override;
};

class SetDamageEventData : public EventData
{
public:
    int damage;
    DamageType dType;
    
    void ExecuteSubEffect(int subId) override;
    void ExecuteSubForCaster(int subId) override;
    void ExecuteSubForTarget(int subId) override;
};

class DeadSelfEventData : public EventData
{
};

class DeadSomeoneEventData : public EventData
{
};

class WhenCastSkillEventData : public EventData
{
public:
    int skillId;
};

class WhenCastTypeSkillEventData : public EventData
{
public:
    SkillType skillType;
};

class SpecialEffectEventData : public EventData
{
public:
    FuncType fType;
    
};

class GetSpecialEffectEventData : public EventData
{
public:
    FuncType fType;
    
};

class BattleStartEventData : public EventData
{
public:
};

class SpecialRoundStartEventData : public EventData
{
public:
    int roundCount;
    
};

class SpecialRoundEndEventData : public EventData
{
public:
    int roundCount;
    
};

class WhenDodgeEventData : public EventData
{
public:
    DamageType dType;
    
};

class WhenStrikeEventData : public EventData
{
public:
    DamageType dType;
    
};

class WhenBlockEventData : public EventData
{
public:
    DamageType dType;
    
};

class DamageHitEventData : public EventData
{
};

class GetDamageHitEventData : public EventData
{
};

class KillSomeoneEventData : public EventData
{
public:
};

class WhenHealEventData : public EventData
{
public:
    int damage;
    DamageType dType;
    
};

class DeadEnemyEventData : public EventData
{
};

class DeadFriendEventData : public EventData
{
};

#endif

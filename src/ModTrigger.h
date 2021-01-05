//
//  ModTrigger.h
//  grav_test
//
//  Created by kingroy on 16/10/17.
//  Copyright © 2016年 Babeltime. All rights reserved.
//

#ifndef _GRAV_MOD_TRIGGER_H_
#define _GRAV_MOD_TRIGGER_H_

#include "Listener.h"
#include "EventData.h"

class GetDamageTrigger : public Notifier
{
public:
    void Trigger(GetDamageEventData *data)
    {
        Notify(data);
    }
};

class SendDamageTrigger : public Notifier
{
public:
    void Trigger(SetDamageEventData *data)
    {
        Notify(data);
    }
};

class DeadSelfTrigger : public Notifier
{
public:
    void Trigger(DeadSelfEventData *data)
    {
        Notify(data);
    }
};

class DeadSomeoneTrigger : public Notifier
{
public:
    void Trigger(DeadSomeoneEventData *data)
    {
        Notify(data);
    }
};

class WhenCastSkillTrigger : public Notifier
{
public:
    void Trigger(WhenCastSkillEventData *data)
    {
        Notify(data);
    }
};

class WhenCastTypeSkillTrigger : public Notifier
{
public:
    void Trigger(WhenCastTypeSkillEventData *data)
    {
        Notify(data);
    }
};

class SpecialEffectTrigger : public Notifier
{
public:
    void Trigger(SpecialEffectEventData *data)
    {
        Notify(data);
    }
};

class GetSpecialEffectTrigger : public Notifier
{
public:
    void Trigger(GetSpecialEffectEventData *data)
    {
        Notify(data);
    }
};

class BattleStartTrigger : public Notifier
{
public:
    void Trigger(BattleStartEventData *data)
    {
        Notify(data);
    }
};

class SpecialRoundStartTrigger : public Notifier
{
public:
    void Trigger(SpecialRoundStartEventData *data)
    {
        Notify(data);
    }
};

class SpecialRoundEndTrigger : public Notifier
{
public:
    void Trigger(SpecialRoundEndEventData *data)
    {
        Notify(data);
    }
};

class WhenDodgeTrigger : public Notifier
{
public:
    void Trigger(WhenDodgeEventData *data)
    {
        Notify(data);
    }
};

class WhenStrikeTrigger : public Notifier
{
public:
    void Trigger(WhenStrikeEventData *data)
    {
        Notify(data);
    }
};

class WhenBlockTrigger : public Notifier
{
public:
    void Trigger(WhenBlockEventData *data)
    {
        Notify(data);
    }
};

class DamageHitTrigger : public Notifier
{
public:
    void Trigger(DamageHitEventData *data)
    {
        Notify(data);
    }
};

class GetDamageHitTrigger : public Notifier
{
public:
    void Trigger(GetDamageHitEventData *data)
    {
        Notify(data);
    }
};

class KillSomeoneTrigger : public Notifier
{
public:
    void Trigger(KillSomeoneEventData *data)
    {
        Notify(data);
    }
};

class WhenHealTrigger : public Notifier
{
public:
    void Trigger(WhenHealEventData *data)
    {
        Notify(data);
    }
};

class DeadEnemyTrigger : public Notifier
{
public:
    void Trigger(DeadEnemyEventData *data)
    {
        Notify(data);
    }
};

class DeadFriendTrigger : public Notifier
{
public:
    void Trigger(DeadFriendEventData *data)
    {
        Notify(data);
    }
};

#endif

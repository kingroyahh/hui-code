#ifndef _GRAV_ACTOR_MODIFIER_SUB_H_
#define _GRAV_ACTOR_MODIFIER_SUB_H_
#include <map>
#include "Type.h"
#include "Enum.h"
#include "ModifierMeta.h"
#include "Listener.h"
#include "EventData.h"
#include "NewActorAttrs.h"

using namespace std;

class ActorAttrData;

class ActorModifierSub// : public IListener
{
public:
    ActorPtr owner;
    ActorPtr casterIn;
    ModifierMeta* modMeta;
    int modLevel;
    int remainActionCount;
    int shieldDamage;
    //int triggerTimes;
    //map<ActorAttrType, int> modMaps;
    //ActorAttrData *pAttrSnapShot;
    NewAttrSnapShot * snapShot;
    bool deleteMark;

    void DoEffect(PeriodType sType,int triggerArea);
    void DoDetach();
    void MarkDeleted();
    ActorModifierSub(ActorPtr caster, int modId, ActorPtr actor, int level, int round);
    virtual ~ActorModifierSub();
    int GetAttachRound();
    void RemoveTrigger();
private:
    int triggerInstID;
    EEventType triggerType;
    int attachRound;
    bool CheckPeroidCondition(int periodCondition, float params[]);
    void OnEffect();
    void OnAttachEffect();
    TempAttrInfo attrInfo;
};

#endif
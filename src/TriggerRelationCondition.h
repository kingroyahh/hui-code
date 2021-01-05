#ifndef _GRAV_BATTLE_TRIGGER_RELATION_CONDITION_H_
#define _GRAV_BATTLE_TRIGGER_RELATION_CONDITION_H_

#include "TriggerCondition.h"

class ConTargetMatchParam : public TriggerCondition
{
public:
    void Init(BattleTrigger* pParent,FightCondMeta *condMeta);
    bool CheckCondition(const TriggerParam & param);
    double GetConAttrValue(Actor *pTarget , const TriggerParam & param,ConMatchType conType);
};

#endif /* TriggerRelationCondition_hpp */

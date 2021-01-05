#ifndef _GRAV_BATTLE_TRIGGER_CONDITION_H_
#define _GRAV_BATTLE_TRIGGER_CONDITION_H_

#include "Type.h"
#include "Enum.h"
#include "TriggerMeta.h"
#include "FightCondMeta.h"
class BattleTrigger;
class TriggerCondition
{
public:
    TriggerCondition();
    virtual ~TriggerCondition();
    inline TriggerCondType GetConditionType(){ return conType; }
    virtual void Init(BattleTrigger* pParent,FightCondMeta *condMeta);
    virtual bool CheckCondition(const TriggerParam & param) = 0;
protected:
    Actor* GetTargetActor(const TriggerParam & param);
    Actor* GetTargetActorByTarget(const TriggerParam & param,TriggerConTarget ct);
    int GetTargetValue(const TriggerParam & param);
    int GetTargetValueByType(const TriggerParam & param,TriggerConTarget ct);
protected:
    BattleTrigger * pParentTrigger;
    FightCondMeta *condMeta;
    TriggerCondType conType;
    int conParam;
    TriggerConTarget conTarget;
    TriggerConTarget conTarget2;
    ConMatchType conAttr;
    ConMatchType conAttr2;
    CondRelation relation;
};

class CondHpPercentGreaterThan : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondHpPercentLessEqual : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondXpPercentGreateThan : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondXpPercentLessEqual : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetContainsBuffGroup : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetIsEnemy : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetIsAlly : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetIsSelf : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetIsCaster : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetIsOwner : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondResult1EqualParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondResult2EqualParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondResult3EqualParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondResult4EqualParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondResult5EqualParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondResult6EqualParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondResult7EqualParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondResult8ContainParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetGreatThanParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetLessEqualParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetNotEqualParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetBelongToBackRowParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondTargetBelongToFrontRowParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class ConTargetBelongToTrggerActorColumn : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class ConTargetBelongToTrggerActorRow : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CurrentEnemyCountEqualToConParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CurrentEnemyContainsBuffGroup :public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class CondEqualtoFirstAttackSiseParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class OwnerHpPercentGreaterThanContargetParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};
class OwnerHpPercentLessEqualContargetParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};

class MainTargetColumnContainOneParam : public TriggerCondition
{
public:
    bool CheckCondition(const TriggerParam & param);
};

#endif /* _GRAV_BATTLE_TRIGGER_H_ */


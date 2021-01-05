#ifndef _GRAV_FIGHTCOND_META_H_
#define _GRAV_FIGHTCOND_META_H_

#include "Enum.h"
#include "MetaSet.h"
#include "Type.h"
#include "Singleton.h"
#include "EnumMacro.h"
#include "rapidjson/document.h"

using namespace rapidjson;
DECLARE_ENUM(TriggerCondType)
Invalid = 0,
HpPercentGreaterThan = 1,
HpPercentLessEqual = 2,
XpPercentGreaterThan = 3,
XpPercentLessEqual = 4,
TargetHasBuffGroup = 5,
TargetIsEnemy = 6,
TargetIsAlly = 7,
TargetIsMySelf = 8,
ConTargetIsOwner = 9,
Result1EqualParam = 10,
Result2EqualParam = 11,
Result3EqualParam = 12,
Result4EqualParam = 13,
TargetResultGreaterThanParam = 14,
TargetResultLessEqualParam = 15,
TargetResultNotEqualParam = 16,
TargetBelongToBackRow = 17,
TargetBelongToFrontRow = 18,
TargetBelongToTriggerActorColumn = 19,
TargetBelongToTriggerActorRow = 20,
CurrentEnemyCountEqualToParam = 21,
CurrentEnemyContainsBuffGroupParam = 22,
ContargetEqualtoFirstAttackSiseParam = 23,
OwnerHpPercentGreaterThanContarget = 24,
OwnerHpPercentLessEqualContarget = 25,
MainTargetColumnContainOne = 26,
Result5EqualParam = 27,
Result6EqualParam = 28,
Result7EqualParam = 29,
Result8ContainParam = 30,
ConTargetMatch = 100
END_ENUM()

DECLARE_ENUM(TriggerConTarget)
    Invalid = 0,
    C1 = 1,
    C2 = 2,
    R1 = 3,
    R2 = 4,
    R3 = 5,
    R4 = 6,
    BuffCaster = 7,
    BuffOwner = 8,
    MainTarget = 9,
    ConParam =10,
    RoundCount =11,
    ConAttrVal =12,
    R5 = 15,
    R6 = 16,
    R7 = 17,
    R8 = 18,
    R4TotalPercent = 19,
END_ENUM()

DECLARE_ENUM(CondRelation)
Invalid = 0,
Equal = 1,
NotEqual = 2,
GreatThen = 3,
LessThen =4,
GreatOrEqual = 5,
LessOrEqual = 6,
END_ENUM()

DECLARE_ENUM(ConMatchType)
Invalid = 0,
HPPercent = 1,
XPPercent = 2,
ActionOrder =3,
MainTargetCount = 4,
OppAliveCount = 5,
OurAliveCount = 6,
END_ENUM()

struct FightCondMeta
{
    int id;
    TriggerCondType Condition;
    int ConParam;
    TriggerConTarget ConTarget;
    TriggerConTarget ConTarget2;
    CondRelation Relation;
    ConMatchType ConAttr;
    ConMatchType ConAttr2;
};

class FightCondMetaSet : public BaseMetaSet<FightCondMeta>, public Singleton < FightCondMetaSet >
{
protected:
    void AssignPath() override;
    FightCondMeta* ParseJObject(Value::Object &jobj) override;
};

#endif

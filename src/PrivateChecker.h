#ifndef _GRAV_BATTLE_PRIVATE_CHECK_H_
#define _GRAV_BATTLE_PRIVATE_CHECK_H_

#include "Type.h"
class BattleTrigger;
class CheckTriggerPrivate
{
public:
    virtual bool CheckPrivate(BattleTrigger* trigger, const TriggerParam& param) = 0;
    virtual ~CheckTriggerPrivate(){}
    inline PrivateCheckType GetType(){ return privateCheckType; }
    inline void Init(PrivateCheckType checkType){ privateCheckType = checkType; }
protected:
    CheckTriggerPrivate()
        :privateCheckType(PrivateCheckType::Invalid)
    {}
    PrivateCheckType privateCheckType;
};
class CheckTriggerComPrivate :public CheckTriggerPrivate
{
public:
    bool CheckCommonPrivate(BattleTrigger* trigger, bool & outPass);
};

class CheckTriggerPrivateC1 :public CheckTriggerComPrivate
{
public:
    virtual bool CheckPrivate(BattleTrigger* trigger, const TriggerParam& param);
};
class CheckTriggerPrivateC1List :public CheckTriggerComPrivate
{
public:
    virtual bool CheckPrivate(BattleTrigger* trigger, const TriggerParam& param);
};
class CheckTriggerPrivateNeglect :public CheckTriggerComPrivate
{
public:
    virtual bool CheckPrivate(BattleTrigger* trigger, const TriggerParam& param);
};

#endif
#ifndef _GRAV_BATTLE_TRIGGER_ACTION_H_
#define _GRAV_BATTLE_TRIGGER_ACTION_H_

#include "Type.h"
#include "Enum.h"
#include "TriggerMeta.h"

class BattleTrigger;
class TriggerAction
{
public:
    TriggerAction();
    virtual ~TriggerAction();
    virtual bool NextExceute(TriggerParam & param);
    virtual void Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4,int reset, TriggerActionCaster caster);
    void InitDelayExec(TriggerActionDelay delayExec){ delayAction = delayExec; }
    inline TriggerActionType GetActionType(){ return actionType; }
protected:
    virtual bool NextExceuteImpl(TriggerParam & param) = 0;
    BattleTrigger * pParentTrigger;
    TriggerActionType actionType;
    int eCountMeta;
    int resetMeta;
    int eCount;
    TriggerActionDelay delayAction;
};
class DoEffectAction : public TriggerAction
{
public:
    DoEffectAction();
    void Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster);
protected:
    bool NextExceuteImpl(TriggerParam & param);
    int effectID;
    TriggerActionCaster casterMeta;
};

class RemoveSrcBuffAction : public TriggerAction
{
public:
    RemoveSrcBuffAction();
protected:
    bool NextExceuteImpl(TriggerParam & param);
};
class RemoveTriggerAction : public TriggerAction
{
public:
    RemoveTriggerAction();
protected:
    bool NextExceuteImpl(TriggerParam & param);
};
class ExeEffectAffixAction : public TriggerAction
{
public:
    ExeEffectAffixAction(){}
    void Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1,int param2,int param3,int param4, int reset, TriggerActionCaster caster);
protected:
    bool NextExceuteImpl(TriggerParam & param);
    int affixID;
    int affixGrow;
    int affixBase;
};
class Old_ExeEffectBuffChanceAction : public TriggerAction
{
public:
    Old_ExeEffectBuffChanceAction(){}
    void Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster);
protected:
    bool NextExceuteImpl(TriggerParam & param);
    int buffIndex;
    int extraChanceBase;
    int extraChanceGrow;
};


class ExeEffectBuffChanceAction : public TriggerAction
{
public:
    ExeEffectBuffChanceAction(){}
    void Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster);
protected:
    bool NextExceuteImpl(TriggerParam & param);
    double extraChanceBase;
    double extraChanceBase2;
    double extraChanceGrow;
    double extraChanceGrow2;
};

class ExecSpEffectChanceAction : public TriggerAction
{
public:
    ExecSpEffectChanceAction(){}
    void Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster);
protected:
    bool NextExceuteImpl(TriggerParam & param);
    double extraChanceBase;
    double extraChanceBase2;
    double extraChanceGrow;
    double extraChanceGrow2;
};

class ChangeFuncParamsAction : public TriggerAction
{
public:
    ChangeFuncParamsAction(){}
    void Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster);
protected:
    bool NextExceuteImpl(TriggerParam & param);
    int order;
    double extraChanceBase;
    double extraChanceBase2;
    double extraChanceGrow;
    double extraChanceGrow2;
};

class ChangeSkillRedoRateAction : public TriggerAction
{
public:
    ChangeSkillRedoRateAction(){}
    void Init(BattleTrigger* pParent, TriggerActionType aType, int eCnt, int param, int param1, int param2,int param3,int param4, int reset, TriggerActionCaster caster);
protected:
    bool NextExceuteImpl(TriggerParam & param);
    double extraChanceBase;
    double extraChanceBase2;
    double extraChanceGrow;
    double extraChanceGrow2;
};



#endif /* _GRAV_BATTLE_TRIGGER_H_ */

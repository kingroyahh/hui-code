#ifndef _GRAV_TRIGGER_META_H_
#define _GRAV_TRIGGER_META_H_

#include "Enum.h"
#include "MetaSet.h"
#include "Type.h"
#include "Singleton.h"
#include "rapidjson/document.h"

using namespace rapidjson;

enum ETriggerType
{
    EPublic = 0,
    EPrivate = 1,
};
struct TriggerMeta
{
    enum {
        TriggerChanceCnt = 2,
    };
    int id;
    ETriggerType TriggerType;
    EEventType Event;
    int EventParam1[MAX_TRIGGER_EVENT_PARAM_COUNT];
    int EventParam2[MAX_TRIGGER_EVENT_PARAM_COUNT];
    int Priority;
    int TriggerChance[TriggerChanceCnt];
    int TriggerTimes;
    int ConditionId[MAX_TRIGGER_CONDID_COUNT];
    int ActionEcount[MAX_TRIGGER_ACTION_COUNT];
    TriggerActionType ActionType[MAX_TRIGGER_ACTION_COUNT];
    int ActionParam[MAX_TRIGGER_ACTION_COUNT];
    int ActionParam1[MAX_TRIGGER_ACTION_COUNT];
    int ActionParam2[MAX_TRIGGER_ACTION_COUNT];
    int ActionParam3[MAX_TRIGGER_ACTION_COUNT];
    int ActionParam4[MAX_TRIGGER_ACTION_COUNT];
    int ActionReset[MAX_TRIGGER_ACTION_COUNT];
    TriggerActionCaster ActionCaster[MAX_TRIGGER_ACTION_COUNT];
    TriggerActionDelay ActionDelay;
};

class TriggerMetaSet : public BaseMetaSet<TriggerMeta>, public Singleton < TriggerMetaSet >
{
protected:
    void AssignPath() override;
    TriggerMeta* ParseJObject(Value::Object &jobj) override;
};

#endif

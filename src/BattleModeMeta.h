#ifndef _GRAV_BATTLEMODE_META_H_
#define _GRAV_BATTLEMODE_META_H_

#include "Enum.h"
#include "Type.h"
#include "MetaSet.h"
#include "Singleton.h"

#include "rapidjson/document.h"

using namespace rapidjson;

enum ESelectTarget
{
    BM_Sequence = 0,
    BM_ERelevant = 1,
    BM_ERandom = 2,
};
enum ESwitchToAuto
{
    BM_Denied = 0,
    BM_Reserved = 1,
    BM_ManuallyFirst = 2,
};
enum EBattleType
{
    BM_Pve = 0,
    BM_Pvp = 1,
};
enum class EBMCombo
{
    True = 0,
    False = 1,
};
struct BattleModeMeta
{
    int id;
    ESelectTarget TargetType_Attacker;
    ESelectTarget TargetType_Defender;
    ESwitchToAuto SwitchAutoBattle;
    EBattleType BattleType;
    EBMCombo ComboRule_Attacker;
    EBMCombo ComboRule_Defender;
    int RoundAffix[MAX_ROUND_COUNT];
    int RoundAffixPower[MAX_ROUND_COUNT];
};

class BattleModeMetaSet : public BaseMetaSet<BattleModeMeta>, public Singleton < BattleModeMetaSet >
{
protected:
    void AssignPath() override;
    BattleModeMeta* ParseJObject(Value::Object &jobj) override;
};

#endif

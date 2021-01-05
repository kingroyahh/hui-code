#ifndef _GRAV_TYPE_H_
#define _GRAV_TYPE_H_
#include <map>
#include <vector>
#include <string>
#include "Enum.h"
#include "EnumMacro.h"
#include <memory>
#include "BattleReplay.pb.h"
using namespace std;
using namespace battle;
#define MAX_AREA_COUNT 2
#define MAX_COLUMN_SIZE 3
#define MAX_ROW_SIZE_NEW 2
#define MAX_ROW_SIZE 3
#define FRONT_MIDDLE 1
#define BACK_MIDDLE 4

#define MAX_ACTOR_COUNT 12
#define MAX_ROUND_COUNT 20
#define MAX_REPLAY_CMD_COUNT 250
#define MAX_EFFECT_CMD_COUNT 60

#define BATTLE_POS_MAX 6
#define DOUBLEHIT_TYPE_COUNT 4

#define MAX_MOD_COUNT 50
#define MAX_TARGET_FILTER_CNT 4
#define MAX_DMG_TARGET_COUNT 6
#define MAX_DMG_PARAM 32
#define MAX_BUFF_CACHE_ATTR_COUNT 32
#define MAX_EFFECT_COUNT 5

#define MAX_AFFIX_ATTR 10

#define DAMAGE_REDUCTION_ARG 700
#define DAMAGE_ADDITION_ARG 701
#define DAMAGE_ARG_1 702
#define DAMAGE_ARG_2 703
#define STRIKE_DAMAGE_ARG 704
#define BLOCK_RATE_ARG 705
#define DOUBLE_HIT_DAMAGE_RATE 757
#define DOUBLE_HIT_DAMAGE_RATE_COUNT 4
#define DOUBLE_HIT_TIME_RADIUS 759
#define DOUBLE_HIT_PROBALITY 765
#define DOUBLE_HIT_TIME_RADIUS_COUNT 4
#define DOUBLE_HIT_SCORE_RANGE_COUNT 5
#define DOUBLE_HIT_PROBALITY_COUNT 4

#define ATTACK_TRIGGER_INTERVAL_MYSIDE 731
#define ATTACK_TRIGGER_INTERVAL_OPPOSIDE 734

#define HEAL_STRIKE_DAMAGE_ARG 773

#define MAX_TRIGGER_EVENT_PARAM_COUNT 32
#define MAX_TRIGGER_CONDID_COUNT 32
#define MAX_TRIGGER_ACTION_COUNT 32
#define MAX_FIGHT_COND_COUNT 32

#define MAX_MONSTER_GROUP 3
#define MAX_BUFF_GROUP_CNT 8
#define LEGEND_SKILL_BUFF_ID 101

#define MAX_CACHE_EVENT_DATA 10

#define MAX_CACHE_SKILL_DATA 10
#define MAX_CACHE_AFFIX_DATA 20
#define MAX_TRIGGER_STACK_LENGTH 10

#define HOT_GROUP_ID 14
#define DOT_GROUP_ID 12
#define MAX_SHIELD_INFO 16

#define RANDOM_MAX 2147483647

class BattleField;
class BattleArea;
class Actor;
class ActorSkill;
class ActorSkillSub;
class ActorModifier;
class ActorModifierSub;
class Attr;
class ModSub;
class EventData;

typedef BattleField* BattleFieldPtr;

typedef BattleArea* BattleAreaPtr;

typedef Actor* ActorPtr;

typedef ActorSkill* ActorSkillPtr;

typedef ActorSkillSub* ActorSkillSubPtr;

typedef ActorModifier* ActorModifierPtr;

typedef ActorModifierSub* ActorModifierSubPtr;

typedef Attr* AttrPtr;

typedef ModSub* ModSubPtr;

typedef EventData* EventDataPtr;


DECLARE_ENUM(EEventType)
Invalid = 0,
CauseDamage = 1,
SufferDamage = 2,
CauseHeal = 3,
SufferHeal = 4,
CauseSkill = 5,
SufferSkill = 6,
SomeActorDead = 7,
GainBuff = 8,
BuffOver = 9,
RoundBegin = 10,
Dodge = 11,
Miss = 12,
ComingCauseDamage = 13,
ComingSufferDamage = 14,
ComingCauseHeal = 15,
ComingSufferHeal = 16,
BigRoundStart = 20,
SmallRoundEnd = 21,
NearDeath = 22,
RoundSmallBegin = 23,
RoundSmallEnd = 24,
BeforeEffectAddBuff =25,
BeforeEffectAddSpEffect = 26,
BeforeSkill = 27,
AfterSkill = 28,
AfterCauseSubEffect = 29,
AfterSufferSubEffect = 30,
BeforeCauseSubEffect = 31,
BeforeSufferSubEffect = 32,
Count = 33,
END_ENUM()

DECLARE_ENUM(TriggerActionType)
Invalid = 0,
DoEffect = 1,
ModifyResult = 2,
RemoveBuff = 3,
RemoveTrigger = 4,
ExecEffectAffix = 5,
Old_ExecEffectBuffChance = 6, //abandon arithmetic
ChangeFuncParams = 8,
ExecEffectBuffChance = 11,
ExecSpEffectChance = 12,
SkillRedoRate = 13,
END_ENUM()

DECLARE_ENUM(TriggerActionCaster)
Invalid = 0,
C1 = 1,
C2 = 2,
BuffCaster = 3,
BuffOwner = 4,
END_ENUM()

DECLARE_ENUM(TriggerActionDelay)
Default = 0,
BigRoundStart = 1,
SmallRoundEnd = 2,
END_ENUM()

//little struct
struct SkillData
{
    bool isSet;
    int skillId;
    int skillLv;
};

struct AffixData
{
    bool isSet;
    int affixId;
    int affixPower;
    map <ActorAttrType, int> modMaps;
};

typedef struct _ShieldInfo
{
    int buffID;
    int absorbValue;
    int isOver;
}ShieldInfo;

struct ActorList
{
    enum ActorListCount
    {
        Size = 6,
    };
    ActorList()
    {
        Reset();
    }
    ActorList(const ActorList & other)
    {
        memcpy(list, other.list, sizeof(list));
    }
    void operator = (const ActorList& other)
    {
        memcpy(list, other.list, sizeof(list));
    }
    void Reset()
    {
        memset(&list, 0, sizeof(list));
    }
    int GetSize()
    {
        int cnt = 0;
        for (int i = 0; i < Size; i++)
        {
            if (list[i] != nullptr)
            {
                cnt++;
            }
        }
        return cnt;
    }
    void Remove(Actor* pActor)
    {
        if (pActor == nullptr)
        {
            return;
        }
        for (int i = 0; i < Size; i++)
        {
            if (list[i] == pActor)
            {
                list[i] = nullptr;
                break;
            }
        }
    }
    void RemoveByValidIndex(int index)
    {
        int validIndex = 0;
        for (int i = 0; i < Size; i++)
        {
            if (list[i] != nullptr)
            {
                if (validIndex++ == index)
                {
                    list[i] = nullptr;
                    break;
                }
            }
        }
    }
    void Merge(ActorList& other)
    {
        for (int i = 0; i < Size; i++)
        {
            if (other.list[i] != nullptr)
            {
                AppendUnique(other.list[i]);
            }
        }
    }
    void AppendUnique(Actor* pActor)
    {
        if (pActor == nullptr)
        {
            return;
        }
        for (int i = 0; i < Size; i++)
        {
            if (list[i] == nullptr)
            {
                list[i] = pActor;
                break;
            }
            else if (list[i] == pActor)
            {
                break;
            }
        }
    }
    Actor *list[Size];
};
struct TriggerParam
{
    ActorList c1;
    ActorList c2;
    int r1; //当前回合数//
    int r2; //伤害类型，2暴击，4格挡//
    int r3; //过量伤害/治疗//
    int r4; // 实际伤害/治疗//
    int r5; //SubEffectId
    int r6; //EffectId
    int r7; //SkillId
    int r8; //BuffId
    EEventType evtType;
    SkillType skillType;
    ActorModifierSub* pBuff;
    bool normalBuffOver;
    int level;
    int intParam;
    int intParam2;
    double doubleParams[MAX_DMG_PARAM];
    bool hasResult;
    int resultInt;
    double resultDouble;
    double resultDouble2;
    class EffectSub* pEffect;
    class TempoExtraAffixPerformer* affixPerformer;
    TriggerParam()
        :r1(0), r2(0), r3(0), r4(0),r5(0),r6(0),r7(0),r8(0), evtType(EEventType::Invalid), skillType(Invalid), pBuff(nullptr), pEffect(nullptr), affixPerformer(nullptr), normalBuffOver(false),level(0),resultInt(0),resultDouble(0),hasResult(false),intParam(0),intParam2(0){}
};

//给Trigger追踪用的一个结构体
struct TriggerTrace
{
    int subEffectId; //SubEffectId
    int effectId; //EffectId
    int skillId; //SkillId
    int buffId; //BuffId
    TriggerTrace():subEffectId(0), effectId(0), skillId(0), buffId(0){}
};

struct SkillInfo
{
    int skillId;
    SkillType skillType;
    int DoubleHitRank;
    int ComAtkIndex;
    float doubleHitDamageRate;
    SkillInfo()
        :skillType(Invalid), DoubleHitRank(0), ComAtkIndex(0), doubleHitDamageRate(0){}
};
struct DelayDoAction
{
    ActorList c1;
    ActorList c2;
    int effectID;
    int triggerID;
    int levelID;
    Actor * effectCaster;
    Actor * bufferCaster;
    Actor * bufferOwner;
    DelayDoAction()
        :effectID(0), triggerID(0), levelID(0), effectCaster(nullptr), bufferCaster(nullptr), bufferOwner(nullptr){}
};

DECLARE_ENUM(PrivateCheckType)
Invalid = 0,
CheckC1 = 1,
CheckC1List = 2,
Neglect = 3,
END_ENUM()

DECLARE_ENUM(BuffAffixApply)
Attach = 0,
CauseAtk = 1,
SufferAtk = 2,
CauseSkill = 3,
SufferSkill = 4,
END_ENUM()

DECLARE_ENUM(TargetFilter1) //目标选择过滤器 1
None = 0,
FrontRowRel = 1,
BackRowRel = 2,
All = 3,
AllDead = 4,
Self = 5,
MainTarget = 6,
C1 = 7,
C2 = 8,
BuffCaster = 9,
BuffOwner = 10,
OppoPos = 11,
BackOne = 12,
END_ENUM()

DECLARE_ENUM(TargetFilter2) //目标选择过滤器 2
Current = 0,
HpMost = 1,
HpLest = 2,
XpMost = 3,
XpLest = 4,
OtherFirst = 5,
Random = 6,
XpLestOtherFirst = 7
END_ENUM()

DECLARE_ENUM(TargetFilter3) //目标选择过滤器 3
Current = 0,
TargetRow = 1,
TargetColumn = 2,
TargetAround = 3,
TargetLR = 4,
TargetBehind = 5,
TargetFront = 6,
TargetAndLR = 7,
END_ENUM()

DECLARE_ENUM(BuffVanishType)
Invalid = 0,
MaxLayerLimit = 1,
LastZeroRound = 2,
Expired = 3,
RmByDispel = 4,
RmByType = 5,
RmByTrigger = 6,
Immune = 7,
ShieldOver = 8,
END_ENUM()

using TempAttrInfo = vector<pair<ActorAttrType, int>>;
using TempAffixPowerInfo = vector<pair<int, int>>;
using RoundAttrInfo = vector<pair<ActorAttrType, int>>;
using ActorAttrMap = vector<pair<int, RoundAttrInfo>>;

struct FillDamageParam
{
    ActorPtr caster;
    ActorPtr target;
    pb::EffectType showEffectType;
    int damage;
    int subEffectId;
    int buffId;
    int resilience;
    int bloodDrink;
    int blockTherapy;
    int strikeTherapy;
    int s_damageTherapy;
    int s_strikeTherapy;
    int s_blockTherapy;
    ShieldInfo* pShieldInfo;
    FillDamageParam(Actor *pCaster, Actor* pTarget, pb::EffectType eType, int subEffID, int damageValue) 
        :caster(pCaster),
        target(pTarget),
        showEffectType(eType),
        subEffectId(subEffID),
        damage(damageValue),
        buffId(0),
        resilience(0),
        bloodDrink(0),
        blockTherapy(0),
        strikeTherapy(0),
        s_damageTherapy(0),
        s_blockTherapy(0),
        s_strikeTherapy(0),
        pShieldInfo(nullptr)
    {}
};
struct FillXpChangeParam
{
    ActorPtr caster;
    ActorPtr target;
    int xpChange;
    int subEffectId;
    int originBuffID;
    bool isShow;
    pb::XpRecoveryType recovery;
    int slainActorID;
    FillXpChangeParam(Actor *pCaster, Actor* pTarget, int subEffID, int xpValue) :
        caster(pCaster), target(pTarget), subEffectId(subEffID), xpChange(xpValue), originBuffID(0), isShow(false), recovery(pb::Normal), slainActorID(0)
    {}
};
struct FillRecoveryParam
{
    ActorPtr caster;
    ActorPtr target;
    pb::EffectType showEffectType;
    int recovery;
    bool isShow;
    int subEffectId;
    int originBuffID;

    FillRecoveryParam(Actor *pCaster, Actor* pTarget, pb::EffectType effectType, int subEffID, int recoveryValue)
        :caster(pCaster), target(pTarget), showEffectType(effectType), subEffectId(subEffID), recovery(recoveryValue), originBuffID(0),isShow(true)
    {}
};
#endif

#ifndef _GRAV_BATTLE_FIELD_H_
#define _GRAV_BATTLE_FIELD_H_
#include "Type.h"
#include "Battle.h"
#include "Random.h"
#include "BattleReplay.pb.h"
#include "BattleParams.pb.h"
#include "BattleCommand.pb.h"
#include "BattleStatistics.pb.h"
#include "BattleAnalysis.pb.h"
#include "ModTrigger.h"
#include "TriggerMeta.h"

using namespace pb;
using namespace battle;
class SkillMeta;
class BattleModeMeta;
class BattleTriggerManager;
class BattleField
{
public:
    BattleStatistics statistics;
    BattleAnalysis analysis;
    bool bAnalysis =false;
    
    class Random random;
    
    //trigger group
    //GetDamageTrigger getDamageTrigger;
    //SendDamageTrigger sendDamageTrigger;
    //DeadSelfTrigger deadSelfTrigger;
    //DeadSomeoneTrigger deadSomeoneTrigger;
    //WhenCastSkillTrigger whenCastSkillTigger;
    //WhenCastTypeSkillTrigger whenCastTypeSkillTrigger;
    //SpecialEffectTrigger specialEffectTrigger;
    //GetSpecialEffectTrigger getSpecialEffectTrigger;
    //BattleStartTrigger battleStartTrigger;
    //SpecialRoundStartTrigger specialRoundStartTrigger;
    //SpecialRoundEndTrigger specialRoundEndTrigger;
    //WhenDodgeTrigger whenDodgeTrigger;
    //WhenStrikeTrigger whenStrikeTrigger;
    //WhenBlockTrigger whenBlockTrigger;
    //DamageHitTrigger damageHitTrigger;
    //GetDamageHitTrigger getDamageHitTrigger;
    //KillSomeoneTrigger killSomeoneTrigger;
    //WhenHealTrigger whenHealTrigger;
    //DeadEnemyTrigger deadEnemyTrigger;
    //DeadFriendTrigger deadFriendTrigger;

public:
    inline BattleTriggerManager* GetTriggerMgr(){ return pTriggerMgr; }
    bool PreFireEvent(TriggerParam & param);
    void FireEvent(TriggerParam & param);
protected:
    BattleTriggerManager* pTriggerMgr;
public:
    void InitBattle(BattleParams& params, int oppIndex);
    void DoBattle(BattleParams& params, int oppIndex);
    void VerifyBattle(BattleParams& params, int oppIndex);
    ReplayCmd* InitOneReplayCmd();

    BattleAreaPtr GetEnemyArea(int areaIndex);
    BattleAreaPtr GetOwnArea(int areaIndex);
    //void RemoveModByCaster(ActorPtr casterIn);

    void AddActorInfoCmd(ActorPtr actor);
    void AddSkillMainCmd(int casterId, int skillId, int mainTargetId);
    void AddBuffMainCmd();
    void GetResult();

    void Reset();
    void SetAnalysis(bool analysis);
    void SetFailCode(BattleFailCode failCode);
    bool InitCombatAttr;
    bool SkillTestMode;
    bool InfiniteSkill;
    bool comAtkDuration;
    bool invalidUserCmd;
    bool inited;
    int curComAtkIndex;
    int maxRoundCount;
    int roundCount;
    int activeRoundCount;
    int firstArea;
    int firstTargetArea;
    int winner;
    int forceWinner;
    int curOrder;
    int curAttackTime;
    int lastAttackTime;
    int lastAttackArea;
    float doubleHitRate;
    int failCode;
    float AtkMyInterval;
    float AtkOppoInterval;
    float doubleHitDamageRate[DOUBLE_HIT_DAMAGE_RATE_COUNT];
    int doubleHitTimeRadius[DOUBLE_HIT_TIME_RADIUS_COUNT];
    int doubleHitTypeInfo[DOUBLEHIT_TYPE_COUNT];
    int doubleHitProbalility[DOUBLE_HIT_PROBALITY_COUNT];
    UserCommand *curUserRecord;
    BattleRecord *curBattleRecord;
    RoundRecord *curRoundRecord;
    BattleRoundState roundState;
    BattleReplay *curReplay;
    RoundCmd *curRoundCmd;
    ReplayCmd *curReplayCmd;
    SkillCmd *curSkillCmd;
    const SkillMeta* LastComSkill;
    char *replayListBuf;
    const BattleModeMeta* CurBattleMode;
    BattleReplayList replayList;
    bool BeginRound(int atkArea);
    void CastSkillByPos(int areaIndex, int posIndex, int tarPosIndex, bool skill);
    bool EndRound(int atkArea);
    bool CheckRoundOver(int atkArea);
    bool CheckWinner();

    void AppendDelayAction(const DelayDoAction & doAction, bool bigRound);
    void OnSomeOneKilled(ActorPtr target,ActorPtr caster);
    int GetCurrentDoubleIndex();
    void SetDoubleHitIndex(int index);
    void CleanReplayBuf();
    bool IsBigRoundDelayActExecuting(){ return bigRoundDelayExecuting; }
    bool IsSmallRoundDelayActExecuting(){ return smallRoundDelayExecuting; }
    int GetCurDelayActTriggerID(){ return curDelayTriggerID; }
    int GetBattleType();
    BattleField();
    virtual ~BattleField();
private:
    BattleAreaPtr battleArea[MAX_AREA_COUNT];
    void InitStaticArgs();
    void InitBattleMode(int battleMode);
    
    void AddActor(int actorId, const pb::InitActorInfo &actorInfo);
    void ExecuteSideAttack(int atkArea, int oppoArea, bool ignoreSkill, bool resetAction);
    void RunBattleToEnd(int curRound, bool firstSide, bool enterAtkPeriod, bool checkSideOver);
    bool VerifyCondition(int atkArea, int atkPos, bool skill, int triggerTime,int roundArea);
    void OnStart();
    void OnRound();
    bool IsOver();
    void InitRoundCmd();
    void AddActorInfoToCmd(ActorInfo *actorInfo, ActorPtr actor);
    void MakeEndReplay();
    void MakeStatistics();
    int CalcAttackTime(int atkArea);
    int CalSkillRandomInterval();
    vector<DelayDoAction> bigRoundDelayAction;
    vector<DelayDoAction> smallRoundDelayAction;
    bool bigRoundDelayExecuting;
    bool smallRoundDelayExecuting;
    int curDelayTriggerID;

};
#endif

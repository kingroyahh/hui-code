#include "BattleField.h"
#include "BattleArea.h"
#include "ValueMeta.h"
#include "Actor.h"
#include "ActorSkill.h"
#include "ActorModifier.h"
#include "Type.h"
#include "Log.h"
#include "BattleReplay.pb.h"
#include "BattleParams.pb.h"
#include "BattleStatistics.pb.h"
#include "BattleAnalysis.pb.h"
#include "EventData.h"
#include <stdio.h>
#include <stdlib.h>
#include "Random.h"
#include <ctime>
#include "Calculator.h"
#include "BattleModeMeta.h"
#include "SkillMeta.h"
#include "EffectUtil.h"
#include "TriggerManager.h"
#include "ActorSkillSub.h"
BattleField::BattleField()
    :curUserRecord(NULL_PTR),
    curBattleRecord(NULL_PTR),
    curRoundRecord(NULL_PTR),
    curReplay(NULL_PTR),
    curRoundCmd(NULL_PTR),
    curReplayCmd(NULL_PTR),
    curSkillCmd(NULL_PTR),
    replayListBuf(NULL),
    LastComSkill(NULL),
    CurBattleMode(NULL),
    failCode(0),
    curOrder(0),
    curAttackTime(0),
    lastAttackTime(0),
    lastAttackArea(-1),
    doubleHitRate(0),
    curComAtkIndex(0),
    AtkMyInterval(2000),
    AtkOppoInterval(2000),
    InitCombatAttr(false),
    InfiniteSkill(false),
    SkillTestMode(false),
    comAtkDuration(false),
    invalidUserCmd(false),
    inited(false),
    pTriggerMgr(NULL),
    roundState((BattleRoundState)0),
    bigRoundDelayExecuting(false),
    smallRoundDelayExecuting(false),
    curDelayTriggerID(0)
{
    TRACE("BattleField::BattleField");
    // init args
    InitStaticArgs();
    
    //area init
    for (int i = 0; i < 2; ++i)
    {
        battleArea[i] = NULL;
    }
    firstArea = 0;
    firstTargetArea = 1;
    random.SetRandomSeed((unsigned)time(NULL));
    memset(doubleHitTypeInfo,0,sizeof(doubleHitTypeInfo));
    curOrder = 0;
    pTriggerMgr = new BattleTriggerManager();
}


void BattleField::CleanReplayBuf()
{
    TRACE("BattleField::CleanReplayBuf");
    if (replayListBuf != NULL)
    {
        delete[] replayListBuf;
        replayListBuf = NULL;
    }
}

BattleField::~BattleField()
{
    TRACE("BattleField::~BattleField");
    if (bAnalysis)
    {
        analysis.Clear();
    }
    CleanReplayBuf();
}

void BattleField::AddActorInfoCmd(ActorPtr actor)
{
    TRACE("BattleField::AddActorInfoCmd");
    ActorInfo* actorInfo = curReplay->add_init_cells();
    AddActorInfoToCmd(actorInfo, actor);
}

void BattleField::GetResult()
{

}

void BattleField::SetAnalysis(bool analysis)
{
    bAnalysis = analysis;
}


void BattleField::InitBattle(BattleParams& params, int oppIndex)
{
    TRACE("BattleField::InitBattle");

    maxRoundCount = params.max_round();
    forceWinner = 1;
    winner = -1;

    random.SetRandomSeed(params.randomseed());
    //outside set

    firstArea = params.first_attack();
    if (firstArea == 0)
    {
        firstTargetArea = 1;
    }
    else if (firstArea == 1)
    {
        firstTargetArea = 0;
    }
    else
    {
        error("first_attack [%d]", firstArea);
    }
    
    roundCount = 0;
    activeRoundCount = 0;

    InitBattleMode(params.battle_type());

    for (int i = 0; i < 2; ++i)
    {
        battleArea[i] = NULL;
        battleArea[i] = new BattleArea(i, this);
    }

    const pb::InitBattleAreaInfo& ownArea = params.own_area();

    if (ownArea.actor_infos_size() <= 0)
    {
        error("no own area actor cant fight !!!\n");
        return;
    }

    if (params.opp_area(oppIndex).actor_infos_size() <= 0)
    {
        error("no opp area actor cant fight !!!\n");
        return;
    }

    info("actor own count : %d\n", ownArea.actor_infos_size());

    for (int i = 0; i < ownArea.actor_infos_size(); ++i)
    {
        const pb::InitActorInfo &actorInfo = ownArea.actor_infos(i);
        int heroId = actorInfo.area_index() * 10 + actorInfo.pos_index() + 1;
        info("Add Actor : heroid:%d, areaIndex:%d, posIndex:%d, metaid:%d\n", heroId, actorInfo.area_index(), actorInfo.pos_index(), actorInfo.meta_id());
        AddActor(heroId, actorInfo);
    }
    
    info("actor opp count : %d\n", params.opp_area(oppIndex).actor_infos_size());

    for (int i = 0; i < params.opp_area(oppIndex).actor_infos_size(); ++i)
    {
        pb::InitActorInfo actorInfo = params.opp_area(oppIndex).actor_infos(i);
        int heroId = actorInfo.area_index() * 10 + actorInfo.pos_index() + 1;
        info("Add Actor : heroid:%d, areaIndex:%d, posIndex:%d, metaid:%d\n", heroId, actorInfo.area_index(), actorInfo.pos_index(), actorInfo.meta_id());
        AddActor(heroId, actorInfo);
    }
    curReplay->set_winner(winner);
    curReplay->set_fail_code(failCode);
    curReplay->set_first_attack(firstArea);
    curReplay->set_max_round(maxRoundCount);
    curReplay->set_stage_id(params.stage_id());
    for (int i = 0; i < params.userinfo_size(); ++i)
    {
        auto uOld = params.userinfo(i);
        auto userInfo = curReplay->add_userinfo();
        userInfo->set_guildid(uOld.guildid());
        userInfo->set_name(uOld.name());
        userInfo->set_guildname(uOld.guildname());
        userInfo->set_power(uOld.power());
        userInfo->set_viplevel(uOld.viplevel());
        userInfo->set_head(uOld.head());
        userInfo->set_headframe(uOld.headframe());
    }
    inited = true;
}


void BattleField::DoBattle(BattleParams& params, int oppIndex)
{
    TRACE("BattleField::DoBattle");
    InitBattle(params, oppIndex);
    OnStart();
    RunBattleToEnd(1, true, false, false);
    CheckWinner();
    MakeEndReplay();
    MakeStatistics();
}
bool BattleField::VerifyCondition(int atkArea, int atkPos,bool skill, int triggerTime, int roundArea)
{
    TRACE("BattleField::VerifyCondition atkArea[%d] atkPos[%d] skill[%d] triggerTime[%d] roundArea[%d]", atkArea, atkPos, skill, triggerTime, roundArea);
    if (comAtkDuration && skill)
    {
        error("BattleField::VerifyCondition [comAtkDuration && skill] atkArea[%d] atkPos[%d] skill[%d] triggerTime[%d] roundArea[%d]", atkArea, atkPos, skill, triggerTime, roundArea);
        return false;
    }
    if (!comAtkDuration && !skill)
    {
        comAtkDuration = true;
    }
    if (atkArea != 0 && atkArea != 1)
    {
        error("BattleField::VerifyCondition [atkArea != 0 && atkArea != 1 ] atkArea[%d] atkPos[%d] skill[%d] triggerTime[%d] roundArea[%d]", atkArea, atkPos, skill, triggerTime, roundArea);
        return false;
    }
    if (triggerTime < 0)
    {
        error("BattleField::VerifyCondition [triggerTime[%d] < 0] atkArea[%d] atkPos[%d] skill[%d] triggerTime[%d] roundArea[%d]", triggerTime,atkArea, atkPos, skill, triggerTime, roundArea);
        return false;
    }
    if (roundArea != atkArea)
    {
        error("BattleField::VerifyCondition [roundArea != atkArea] atkArea[%d] atkPos[%d] skill[%d] triggerTime[%d] roundArea[%d]", atkArea, atkPos, skill, triggerTime, roundArea);
        return false;
    }
    BattleAreaPtr atkBArea = battleArea[atkArea];
    ActorPtr atkActor = atkBArea->GetActorByPos(atkPos);
    if (atkActor == NULL_PTR)
    {
        error("BattleField::VerifyCondition [atkActor == NULL_PTR] atkArea[%d] atkPos[%d] skill[%d] triggerTime[%d] roundArea[%d]", atkArea, atkPos, skill, triggerTime, roundArea);
        return false;
    }
    if (skill)
    {
        if (!atkActor->AllowSkill())
        {
            error("BattleField::VerifyCondition [AllowSkill is flase] atkArea[%d] atkPos[%d] skill[%d] triggerTime[%d] roundArea[%d]", atkArea, atkPos, skill, triggerTime, roundArea);
            return false;
        }
    }
    else
    {
        if (!atkActor->AllowComAtk())
        {
            error("BattleField::VerifyCondition [AllowComAtk is flase] atkArea[%d] atkPos[%d] skill[%d] triggerTime[%d] roundArea[%d]", atkArea, atkPos, skill, triggerTime, roundArea);
            return false;
        }
    }
    return true;
}

void BattleField::ExecuteSideAttack(int atkArea, int targetArea,bool ignoreSkill,bool resetAction)
{
    TRACE("BattleField::ExecuteSideAttack atkArea[%d] targetArea[%d] ignoreSkill[%d] resetAction[%d]", atkArea, targetArea, ignoreSkill, resetAction);
    BattleAreaPtr atkBArea = battleArea[atkArea];

    if (!ignoreSkill)
    {
        bool checkSkillAvailable = true;
        while (checkSkillAvailable)
        {
            bool allSkillOver = true;
            for (int i = 0; i < BATTLE_POS_MAX; i++)
            {
                ActorPtr atkActor = atkBArea->GetActorByPos(i);
                if (atkActor == NULL_PTR)
                {
                    continue;
                }

                if(atkActor->AllowSkill())
                {
                    if (CheckWinner())
                    {
                        return;
                    }
                    CastSkillByPos(atkArea, i, 0, true);
                    if (CheckWinner())
                    {
                        return;
                    }
                    allSkillOver = false;
                    break;
                }
            }
            if (allSkillOver)
            {
                checkSkillAvailable = false;
            }
        }
    }

    lastAttackTime = 0;
    curAttackTime = 0;
    int atkCnt = 0;
    if (resetAction)
    {
        atkBArea->ResetAction();
    }
    for (int i = 0; i < BATTLE_POS_MAX; i++)
    {
        ActorPtr atkActor = atkBArea->GetActorByPos(i);
        if (atkActor == NULL_PTR)
        {
            continue;
        }
        if (!atkActor->AllowComAtk())
        {
            continue;
        }
        lastAttackTime = curAttackTime;
        /*float atkInterval = (atkArea == 0) ? AtkMyInterval : AtkOppoInterval;
        curAttackTime = atkCnt * atkInterval;*/
        curAttackTime = CalcAttackTime(atkArea) + lastAttackTime;
        if (CheckWinner())
        {
            break;
        }
        CastSkillByPos(atkArea, i, 0,false);
        if (CheckWinner())
        {
            break;
        }
        atkCnt++;
    }
}

void BattleField::RunBattleToEnd(int curRound, bool firstSide, bool enterAtkPeriod,bool checkSideOver)
{
    TRACE("BattleField::RunBattleToEnd curRound[%d] firstSide[%d] enterAtkPeriod[%d] checkSideOver[%d]", curRound, firstSide, enterAtkPeriod, checkSideOver);
    roundCount = curRound;
    int localRound = firstSide ? 1 : 2;
    activeRoundCount = (roundCount - 1) * 2 + localRound;

    if (CheckWinner())
    {
        return;
    }

    if (roundCount > maxRoundCount)
    {
        return;
    }

//    bool firstSidePlaying = (firstArea == 0) ? true : false;
//
    int firstIndex = firstSide?0:1;
    for (int index = firstIndex ; roundCount <= maxRoundCount && !CheckWinner(); index++)
    {
        int atkArea = 0;
        int targetArea = 1;
        if (index == firstIndex && checkSideOver)
        {
            if (firstSide)
            {
                atkArea = firstArea;
                targetArea = firstTargetArea;
            }
            else
            {
                atkArea = firstTargetArea;
                targetArea = firstArea;
            }

            ExecuteSideAttack(atkArea, targetArea, enterAtkPeriod, false);
            if (CheckRoundOver(atkArea))
            {
                if (!EndRound(atkArea))
                {
                    invalidUserCmd = true;
                    failCode = Error_CheckRoundEnd;
                    error("BattleField::RunBattleToEnd EndRound(checkSideOver) fail,atkArea[%d] roundCount[%d]", atkArea, roundCount);
                    break;
                }
                if (CheckWinner())
                {
                    return;
                }
            }
            if (CheckWinner())
            {
                return;
            }
        }
        else
        {
            if (firstSide)
            {
                atkArea = firstArea;
                targetArea = firstTargetArea;
            }
            else
            {
                atkArea = firstTargetArea;
                targetArea = firstArea;
            }
            
            if (index != 0 && index%2 == 0)
            {
                roundCount++;
                if (roundCount > maxRoundCount)
                {
                    break;
                }
            }
            int localRound = firstSide ? 1 : 2;
            activeRoundCount = (roundCount - 1) * 2 + localRound;
            TRACE("BattleField::RunBattleToEnd curRound[%d] activeRoundCount[%d]", roundCount, activeRoundCount);
            InitRoundCmd();
            if (!BeginRound(atkArea))
            {
                invalidUserCmd = true;
                failCode = Error_CheckRoundStart;
                error("BattleField::RunBattleToEnd BeginRound fail,atkArea[%d] roundCount[%d]", atkArea, roundCount);
                break;
            }
            if (CheckWinner())
            {
                return;
            }
            
            ExecuteSideAttack(atkArea, targetArea, false, true);
            
            if (CheckRoundOver(atkArea))
            {
                if (!EndRound(atkArea))
                {
                    invalidUserCmd = true;
                    failCode = Error_CheckRoundEnd;
                    error("BattleField::RunBattleToEnd EndRound fail,atkArea[%d] roundCount[%d]", atkArea, roundCount);
                    break;
                }
                if (CheckWinner())
                {
                    return;
                }
            }
            
            if (CheckWinner())
            {
                return;
            }
        }
        firstSide = !firstSide;
    }
}
void BattleField::VerifyBattle(BattleParams& params, int oppIndex)
{
    TRACE("BattleField::VerifyBattle");
    InitBattle(params, oppIndex);
    OnStart();
    comAtkDuration = false;
    invalidUserCmd = false;
    bool firstSidePlaying = (firstArea == 0) ? true : false;
    roundCount = 0;
    activeRoundCount = 0;
    bool checkSideOver = false;

    if (params.commands_size() > 0)
    {
        const BattleRecord& bRecord = params.commands(0).battlerecord(oppIndex);
        for (int i = 0; i < bRecord.roundrecord_size() && !invalidUserCmd; i++)
        {
            int atkArea = 0;
            int targetArea = 1;
            int roundMod = i % 2;
            if (roundMod == 0)
            {
                atkArea = firstArea;
                targetArea = firstTargetArea;
                roundCount++;
                if (roundCount > maxRoundCount)
                {
                    break;
                }
            }
            else
            {
                atkArea = firstTargetArea;
                targetArea = firstArea;
            }
            firstSidePlaying = (atkArea == 0) ? true : false;
            int localRound = firstSidePlaying ? 1 : 2;
            activeRoundCount = (roundCount - 1) * 2 + localRound;
            TRACE("BattleField::VerifyBattle BattleRecord[%d] curRound[%d] activeRoundCount[%d]", i, roundCount, activeRoundCount);
            InitRoundCmd();
            checkSideOver = true;
            comAtkDuration = false;
            BattleAreaPtr atkBArea = battleArea[atkArea];
            atkBArea->ResetAction();
            if (!BeginRound(atkArea))
            {
                error("BattleField::VerifyBattle BeginRound fail,atkArea[%d] roundCount[%d]", atkArea, roundCount);
                invalidUserCmd = true;
                failCode = Error_CheckRoundStart;
                break;
            }
            if (CheckWinner())
            {
                break;
            }
            
            const RoundRecord& rRecord = bRecord.roundrecord(i);
            
            for (int j = 0; j < rRecord.attackrecord_size(); j++)
            {
                const AttackRecord& aRecord = rRecord.attackrecord(j);
                int curAtkArea = aRecord.atk_area_index();
                int curAtkPos = aRecord.atk_pos_index();
                bool curSkill = aRecord.skill();
                int curTriggerTime = aRecord.attacktime();
                int targetPos = aRecord.target_pos_index();
                if (!VerifyCondition(curAtkArea, curAtkPos, curSkill, curTriggerTime, atkArea))
                {
                    error("VerifyCondition is false");
                    invalidUserCmd = true;
                    failCode = Error_CheckAtkValid;
                    break;
                }
                lastAttackTime = curAttackTime;
                curAttackTime = curTriggerTime;
                CastSkillByPos(curAtkArea, curAtkPos, targetPos, curSkill);
                if (CheckWinner())
                {
                    break;
                }
            }
            
            //judge player's common Atk count...todu...
            if (CheckRoundOver(atkArea))
            {
                if (!EndRound(atkArea))
                {
                    error("BattleField::VerifyBattle EndRound fail,atkArea[%d] roundCount[%d]", atkArea, roundCount);
                    invalidUserCmd = true;
                    failCode = Error_CheckRoundEnd;
                    break;
                }
                if (CheckWinner())
                {
                    break;
                }
                checkSideOver = false;
                firstSidePlaying = !firstSidePlaying;
            }
            if(firstArea != 0)
            {
                firstSidePlaying = !firstSidePlaying;
            }
        }
    }
    if (!invalidUserCmd)
    {
        if (roundCount == 0)
        {
            TRACE("BattleField::VerifyBattle roundCount == 0,force set to 1");
            roundCount = 1;
        }
        RunBattleToEnd(roundCount, firstSidePlaying, comAtkDuration, checkSideOver);
    }
    CheckWinner();
    MakeEndReplay();
    MakeStatistics();
}
void BattleField::OnStart()
{
    TRACE("BattleField::OnStart");
    //event
    roundCount = 1;
    activeRoundCount = 1;
    /*BattleStartEventData bsData;
    battleStartTrigger.Trigger(&bsData);*/
}

void BattleField::CastSkillByPos(int areaIndex, int posIndex, int tarPosIndex, bool skill)
{
    info("cast skill by area [%d] pos [%d] skill[%d] tarPos [%d]", areaIndex,posIndex,skill,tarPosIndex);
    curReplayCmd = NULL_PTR;

    ActorPtr act;
    if (areaIndex == 0)
    {
        act = battleArea[0]->GetActorByPos(posIndex);
    }
    else
    {
        act = battleArea[1]->GetActorByPos(posIndex);
    }
    
    if (act == NULL_PTR)
    {
        error("CastSkill Actor is null area[%d],posIndex[%d]", areaIndex, posIndex);
        return;
    }

    SkillType skillType = skill ? XpSkillType : CommonSkillType;
    act->CastSkillByType(skillType, tarPosIndex);
}

bool BattleField::IsOver()
{
    return winner != -1;
}

bool BattleField::CheckWinner()
{
    if (invalidUserCmd)
    {
        winner = 1;
        return true;
    }
    if (winner != -1)
    {
        return true;
    }
    for (int i = 0; i < 2; ++i)
    {
        if (!battleArea[i]->HasAlive() )
        {
            if (i == 0)
            {
                winner = 1;
            }
            else
            {
                winner = 0;
            }
            return true;
        }
    }
    return false;
}

void BattleField::AddActor(int actorId, const pb::InitActorInfo &actorInfo)
{
    TRACE("BattleField::AddActor actorID[%d]", actorId);
    // actor init
    Actor *actor = new Actor(this);
    actor->actorId = actorId;
    actor->actionCount = 0;
    actor->InitActor(actorInfo);
    
    if (actor->actorSkill != NULL_PTR)
    {
        const pb::InitSkillInfo &defaultSkill = actorInfo.default_skill();
        if (defaultSkill.skill_id() != 0)
        {
            actor->actorSkill->InitDefaultSkill(defaultSkill.skill_id(), defaultSkill.skill_lv());
        }
        
        const pb::InitSkillInfo &xpSkill = actorInfo.xp_skill();
        if (xpSkill.skill_id() != 0)
        {
            actor->actorSkill->InitXpSkill(xpSkill.skill_id(), xpSkill.skill_lv());
        }
        
        const pb::InitSkillInfo &combineSkill = actorInfo.combine_skill();
        if (combineSkill.skill_id() != 0)
        {
            actor->actorSkill->InitCombineSkill(combineSkill.skill_id(), combineSkill.skill_lv());
        }
        
        const pb::InitSkillInfo &legendSkill = actorInfo.legend_skill();
        if (legendSkill.skill_id() != 0)
        {
            actor->actorSkill->InitLegendSkill(legendSkill.skill_id(), legendSkill.skill_lv());
        }
    }
    
    //init pasive skill
    for (int i = 0; i < actorInfo.buff_infos_size(); ++i)
    {
        if (actorInfo.buff_infos(i).buff_id() != 0)
        {
            actor->actorMod->AddMod(actor, actorInfo.buff_infos(i).buff_id(), actorInfo.buff_infos(i).buff_lv(), -1);
        }
    }
    
    if(bAnalysis)
    {
        actor->analysisActorInfo = analysis.add_actor_infos();
        actor->analysisActorInfo->set_area_index(actor->areaIndex);
        actor->analysisActorInfo->set_pos_index(actor->posIndex);
        actor->analysisActorInfo->set_hp_init(actor->actorDatas.hp);
        actor->analysisActorInfo->set_hp(0);
        actor->analysisActorInfo->set_damage(0);
        actor->analysisActorInfo->set_times_damage(0);
        actor->analysisActorInfo->set_times_damaged(0);
        actor->analysisActorInfo->set_heal(0);
        actor->analysisActorInfo->set_damage_default(0);
        actor->analysisActorInfo->set_times_default(0);
        actor->analysisActorInfo->set_damage_legend(0);
        actor->analysisActorInfo->set_times_legend(0);
        actor->analysisActorInfo->set_damage_xp(0);
        actor->analysisActorInfo->set_times_xp(0);
        actor->analysisActorInfo->set_damage_combine(0);
        actor->analysisActorInfo->set_times_combine(0);
        actor->analysisActorInfo->set_times_strike(0);
        actor->analysisActorInfo->set_times_dodge(0);
        actor->analysisActorInfo->set_times_block(0);
        actor->analysisActorInfo->set_times_lifesteal(0);
        actor->analysisActorInfo->set_times_kill(0);
        actor->analysisActorInfo->set_dead_round(0);
    }
    
    TRACE("init actor : %d is done!, actorhp : %d\n", actorId, actor->actorDatas.hpMax);

    //add actor in area
    if (battleArea[actor->areaIndex])
    {
        battleArea[actor->areaIndex]->AddActor(actor);
    }
}

BattleAreaPtr BattleField::GetEnemyArea(int areaIndex)
{
    if (areaIndex == 0)
    {
        return battleArea[1];
    }
    else if (areaIndex == 1)
    {
        return battleArea[0];
    }
    return nullptr;
}

BattleAreaPtr BattleField::GetOwnArea(int areaIndex)
{
    return battleArea[areaIndex];
}

void BattleField::AddSkillMainCmd(int casterId, int skillId, int mainTargetId)
{
    TRACE("BattleField::AddSkillMainCmd : casterId: %d, skillId: %d maintargetId: %d\n", casterId, skillId, mainTargetId);

    if (curSkillCmd != NULL_PTR)
    {
        curSkillCmd->set_caster_id(casterId);
        curSkillCmd->set_skill_meta_id(skillId);
        curSkillCmd->set_target_id(mainTargetId);
    }
}

void BattleField::MakeEndReplay()
{
    TRACE("BattleField::MakeEndReplay winner[%d] failCode[%d] firstArea[%d] curRound[%d]",winner,failCode,firstArea,roundCount);
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (battleArea[0]->actors[i] != NULL)
        {
            ActorPtr actor = battleArea[0]->actors[i];
            ActorInfo* actorInfo = curReplay->add_over_cells();
            AddActorInfoToCmd(actorInfo, actor);
        }
        if (battleArea[1]->actors[i] != NULL)
        {
            ActorPtr actor = battleArea[1]->actors[i];
            ActorInfo* actorInfo = curReplay->add_over_cells();
            AddActorInfoToCmd(actorInfo, actor);
        }
    }
    curReplay->set_winner(winner);
    curReplay->set_fail_code(failCode);
    curReplay->set_first_attack(firstArea);
}

void BattleField::MakeStatistics()
{
    statistics.set_winner(winner);
    statistics.set_round_count(roundCount);
    statistics.set_fail_code(failCode);
    double area0Hpmax = 0;
    double area0Hp = 0;
    StAreaInfo *areaInfo0 = statistics.add_areainfo();
    areaInfo0->set_areaindex(0);
    areaInfo0->set_damage((int)battleArea[0]->damageAll);
    
    double area1Hpmax = 0;
    double area1Hp = 0;
    StAreaInfo *areaInfo1 = statistics.add_areainfo();
    areaInfo1->set_areaindex(1);
    areaInfo1->set_damage((int)battleArea[1]->damageAll);
    
    for (int i = 0; i < BATTLE_POS_MAX; ++i)
    {
        if (battleArea[0]->actors[i] != NULL)
        {
            ActorPtr actor = battleArea[0]->actors[i];
            StActorInfo *actorInfo0 = areaInfo0->add_actorinfo();
            actorInfo0->set_actorid(actor->actorId);
            actorInfo0->set_hpnum(actor->actorDatas.hp);
            actorInfo0->set_xpnum(actor->actorDatas.xp);
            actorInfo0->set_hppercent(actor->actorDatas.GetHpPercent());
            actorInfo0->set_deltahp(actor->actorDatas.GetDeltaHp());
            area0Hp += actor->actorDatas.hp;
            area0Hpmax += actor->actorDatas.hpMax;
        }
        if (battleArea[1]->actors[i] != NULL)
        {
            ActorPtr actor = battleArea[1]->actors[i];
            StActorInfo *actorInfo1 = areaInfo1->add_actorinfo();
            actorInfo1->set_actorid(actor->actorId);
            actorInfo1->set_hpnum(actor->actorDatas.hp);
            actorInfo1->set_xpnum(actor->actorDatas.xp);
            actorInfo1->set_hppercent(actor->actorDatas.GetHpPercent());
            actorInfo1->set_deltahp(actor->actorDatas.GetDeltaHp());
            area1Hp += actor->actorDatas.hp;
            area1Hpmax += actor->actorDatas.hpMax;
        }
    }
    areaInfo0->set_hppercent((double)area0Hp/(double)area0Hpmax);
    areaInfo1->set_hppercent((double)area1Hp/(double)area1Hpmax);

    ::pb::StDoubleHitInfo* doubleHit = statistics.mutable_doublehit();
    doubleHit->set_fair(doubleHitTypeInfo[0]);
    doubleHit->set_good(doubleHitTypeInfo[1]);
    doubleHit->set_fine(doubleHitTypeInfo[2]);
    doubleHit->set_excellent(doubleHitTypeInfo[3]);
}

void BattleField::Reset()
{
    TRACE("BattleField::Reset");
    for (int i = 0; i < MAX_AREA_COUNT; ++i)
    {
        if (battleArea[i] != 0)
        {
            delete battleArea[i];
        }
        battleArea[i] = NULL_PTR;
    }
    if (curUserRecord != NULL_PTR)
    {
        delete curUserRecord;
        curUserRecord = NULL_PTR;
    }

    curBattleRecord = NULL;
    curRoundRecord = NULL;
    curReplay = NULL;
    curRoundCmd = NULL;
    curReplayCmd = NULL;
    curSkillCmd = NULL;

    statistics.Clear();
//    error("pb clear done\n");
    /*getDamageTrigger.ClearAll();
    sendDamageTrigger.ClearAll();
    deadSelfTrigger.ClearAll();
    deadSomeoneTrigger.ClearAll();
    whenCastSkillTigger.ClearAll();
    whenCastTypeSkillTrigger.ClearAll();
    specialEffectTrigger.ClearAll();
    getSpecialEffectTrigger.ClearAll();
    battleStartTrigger.ClearAll();
    specialRoundEndTrigger.ClearAll();
    specialRoundStartTrigger.ClearAll();
    whenDodgeTrigger.ClearAll();
    whenStrikeTrigger.ClearAll();
    whenBlockTrigger.ClearAll();
    damageHitTrigger.ClearAll();
    getDamageHitTrigger.ClearAll();
    killSomeoneTrigger.ClearAll();
    whenHealTrigger.ClearAll();
    deadEnemyTrigger.ClearAll();
    deadFriendTrigger.ClearAll();*/
    if (pTriggerMgr != NULL)
    {
        delete pTriggerMgr;
        pTriggerMgr = NULL;
    }
    comAtkDuration = false;
    invalidUserCmd = false;
    winner = -1;
    curOrder = 0;
    curAttackTime = 0;
    lastAttackTime = 0;
    lastAttackArea = -1;
    doubleHitRate = 0;
    roundCount = 0;
    curComAtkIndex = 0;
    activeRoundCount = 0;
    replayList.Clear();
    roundState = RS_Default;
}

//初始化RoundCmd Protobuf
void BattleField::InitRoundCmd()
{
    curRoundCmd = curReplay->add_round_cmds();
}

ReplayCmd* BattleField::InitOneReplayCmd()
{
    curReplayCmd = curRoundCmd->add_replaylist();
    curReplayCmd->set_triggertime(0);
    curSkillCmd = curReplayCmd->mutable_skill_cmd();
    return curReplayCmd;
}

void BattleField::AddActorInfoToCmd(ActorInfo *actorInfo, ActorPtr actor)
{
    TRACE("BattleField::AddActorInfoToCmd");
    int id = actor->actorId;
    actorInfo->set_actor_id(id);
    int tempId = actor->templateId;
    actorInfo->set_meta_id(tempId);
    int posIndex = actor->posIndex;
    actorInfo->set_pos_index(posIndex);
    int areaIndex = actor->areaIndex;
    actorInfo->set_area_index(areaIndex);
    int hp = actor->actorDatas.hp;
    actorInfo->set_hp(hp);
    int xp = actor->actorDatas.xp;
    actorInfo->set_xp(xp);
    actorInfo->set_activeround(actor->activeRound);
    string name = actor->name;
    actorInfo->set_name(name);
    int hpMax = actor->actorDatas.hpMax;
    actorInfo->set_hpmax(hpMax);
    int heroId = actor->actorId;
    actorInfo->set_hero_id(heroId);
    actorInfo->set_xpmax(actor->actorDatas.xpMax);
    actorInfo->set_fashionid(actor->fashionId);
    actorInfo->set_level(actor->level);
    actorInfo->set_awake(actor->awake);
    actorInfo->set_advance(actor->advance);
    actorInfo->set_halo(actor->halo);
}

void BattleField::InitStaticArgs()
{
    TRACE("BattleField::InitStaticArgs");
    const ValueMeta *atkMyInterval = ValueMetaSet::instance()->Get(ATTACK_TRIGGER_INTERVAL_MYSIDE);
    if (atkMyInterval == NULL)
    {
        error("BattleField::InitStaticArgs() value meta cant read! : %d", ATTACK_TRIGGER_INTERVAL_MYSIDE);
        return;
    }
    AtkMyInterval = atkMyInterval->NumValue;
    const ValueMeta *atkOppoInterval = ValueMetaSet::instance()->Get(ATTACK_TRIGGER_INTERVAL_OPPOSIDE);
    if (atkOppoInterval == NULL)
    {
        error("BattleField::InitStaticArgs() value meta cant read! : %d", ATTACK_TRIGGER_INTERVAL_OPPOSIDE);
        return;
    }
    AtkOppoInterval = atkOppoInterval->NumValue;

    memset(doubleHitDamageRate, 0, sizeof(doubleHitDamageRate));
    const ValueMeta *metaDamageRate = ValueMetaSet::instance()->Get(DOUBLE_HIT_DAMAGE_RATE);
    if (metaDamageRate == NULL_PTR)
    {
        error("BattleField::InitStaticArgs() value meta cant read! : %d", DOUBLE_HIT_DAMAGE_RATE);
        return;
    }
    for (int i = 0; i < DOUBLE_HIT_DAMAGE_RATE_COUNT; i++)
    {
        doubleHitDamageRate[i] = metaDamageRate->FloatValueArry[i];
    }

    memset(doubleHitTimeRadius, 0, sizeof(doubleHitTimeRadius));
    const ValueMeta *metaTimeRadius = ValueMetaSet::instance()->Get(DOUBLE_HIT_TIME_RADIUS);
    if (metaTimeRadius == NULL_PTR)
    {
        error("BattleField::InitStaticArgs() value meta cant read! : %d", DOUBLE_HIT_TIME_RADIUS);
        return;
    }
    for (int i = 0; i < DOUBLE_HIT_TIME_RADIUS_COUNT; i++)
    {
        doubleHitTimeRadius[i] = metaTimeRadius->NumValueArry[i];
    }

    memset(doubleHitProbalility, 0, sizeof(doubleHitProbalility));
    const ValueMeta *metaDoubleHitProba = ValueMetaSet::instance()->Get(DOUBLE_HIT_PROBALITY);
    if (metaDoubleHitProba == NULL_PTR)
    {
        error("BattleField::InitStaticArgs() value meta cant read! : %d", DOUBLE_HIT_PROBALITY);
        return;
    }
    for (int i = 0; i < DOUBLE_HIT_PROBALITY_COUNT; i++)
    {
        doubleHitProbalility[i] = metaDoubleHitProba->NumValueArry[i];
    }
}

void BattleField::InitBattleMode(int battleMode)
{
    TRACE("BattleField::InitBattleMode battleMode[%d]", battleMode);
    CurBattleMode = BattleModeMetaSet::instance()->Get(battleMode);
    if (CurBattleMode == NULL)
    {
        error("CurBattleMode is null: %d", battleMode);
        return;
    }
}

int BattleField::GetBattleType()
{
    return (int)CurBattleMode->BattleType;
}

bool BattleField::BeginRound(int atkArea)
{
    TRACE("BattleField::BeginRound atkArea[%d]", atkArea);
    if (atkArea != 0 && atkArea != 1)
    {
        error("BeginRound,atkArea value error[%d]", atkArea);
        return false;
    }
    if (lastAttackArea == -1)
    {
        if (atkArea != firstArea)
        {
            error("BeginRound,the first atkArea[%d] mismatch the origin area [%d]", atkArea,firstArea);
            return false;
        }
    }
    if (lastAttackArea == atkArea)
    {
        error("BeginRound,the same atkArea has been existed[%d]", atkArea);
        return false;
    }
    if (roundState != RS_Default)
    {
        error("BeginRound,AtkArea[%d] RoundState [%d] is not equal to RS_Default", atkArea, roundState);
        return false;
    }
    curComAtkIndex = 0;
    lastAttackArea = atkArea;
    LastComSkill = NULL;
    doubleHitRate = 0;
    roundState = RS_Started;
    if (pTriggerMgr != NULL)
    {
        TriggerParam param;
        param.evtType = EEventType::RoundSmallBegin;
        param.r1 = roundCount;
        param.intParam = atkArea;
        FireEvent(param);
        
        if(atkArea == firstArea){
            pTriggerMgr->ResetTimesLimitPerRound();
            bigRoundDelayExecuting = true;
            
            for (auto & delayAct : bigRoundDelayAction)
            {
                curDelayTriggerID = delayAct.triggerID;
                auto * effect = new EffectSub(delayAct.effectCaster, delayAct.effectID, delayAct.levelID);
                effect->GetEffectTarget(0, &delayAct.c1, &delayAct.c2, delayAct.bufferCaster, delayAct.bufferOwner);
                effect->ExecuteEffect(SkillInfo());
                delete effect;
            }
            bigRoundDelayAction.clear();
            bigRoundDelayExecuting = false;
            
            TriggerParam param;
            param.evtType = EEventType::RoundBegin;
            param.r1 = roundCount;
            FireEvent(param);
        }
    }
    auto affixArray = CurBattleMode->RoundAffix;
    if(roundCount < MAX_ROUND_COUNT){
        int affixId = affixArray[roundCount-1];
        if(affixId>0){
            int power = CurBattleMode->RoundAffixPower[roundCount-1];
            if(power<=0){
                error("RoundAffix > 0 but power <=0  @ liuqingsong ");
                power = 1;
            }
            battleArea[0]->AddRoundAffix(affixId, power);
            battleArea[1]->AddRoundAffix(affixId, power);
        }
    }
    
    battleArea[0]->OnBuffTrigger(atkArea);
    battleArea[1]->OnBuffTrigger(atkArea);
    battleArea[0]->ModRoundCount();
    battleArea[1]->ModRoundCount();
    battleArea[atkArea]->RoundXpRecovery();
    roundState = RS_Attacking;
    return true;
}

bool BattleField::EndRound(int atkArea)
{
    TRACE("BattleField::EndRound atkArea[%d]", atkArea);
    if (atkArea != 0 && atkArea != 1)
    {
        error("BeginRound,atkArea value error[%d]", atkArea);
        return false;
    }
    if (lastAttackArea != atkArea)
    {
        error("EndRound,the begin atkArea does not match current end AtkArea[%d]", atkArea);
        return false;
    }
    if (roundState != RS_Attacking)
    {
        error("BeginRound,AtkArea[%d] RoundState [%d] is not equal to RS_Attacking", atkArea, roundState);
        return false;
    }
    roundState = RS_Ended;
    smallRoundDelayExecuting = true;
    for (auto & delayAct : smallRoundDelayAction)
    {
        curDelayTriggerID = delayAct.triggerID;
        auto * effect = new EffectSub(delayAct.effectCaster, delayAct.effectID, delayAct.levelID);
        effect->GetEffectTarget(0, &delayAct.c1, &delayAct.c2, delayAct.bufferCaster, delayAct.bufferOwner);
        effect->ExecuteEffect(SkillInfo());
        delete effect;
    }
    smallRoundDelayAction.clear();
    smallRoundDelayExecuting = false;
    
    auto affixArray = CurBattleMode->RoundAffix;
    if(roundCount < MAX_ROUND_COUNT){
        int affixId = affixArray[roundCount];
        if(affixId>0){
            battleArea[0]->ClearRoundAffix();
            battleArea[1]->ClearRoundAffix();
        }
    }
    battleArea[0]->OnBuffTrigger(atkArea);
    battleArea[1]->OnBuffTrigger(atkArea);
    battleArea[0]->ModRoundCount();
    battleArea[1]->ModRoundCount();
    battleArea[atkArea]->CheckDeleteMods();
    roundState = RS_Default;
    
    TriggerParam param;
    param.evtType = EEventType::RoundSmallEnd;
    param.r1 = roundCount;
    param.intParam = atkArea;
    FireEvent(param);
    return true;
}

bool BattleField::CheckRoundOver(int atkArea)
{
    return battleArea[atkArea]->CheckRoundOver();
}

void BattleField::SetFailCode(BattleFailCode eFailCode)
{
    failCode = (int)eFailCode;
}

void BattleField::OnSomeOneKilled(ActorPtr target, ActorPtr caster)
{
    BattleAreaPtr friendArea = NULL;
    BattleAreaPtr enemyArea = NULL;
    if (target->areaIndex == 0)
    {
        friendArea = battleArea[0];
        enemyArea = battleArea[1];
    }
    else
    {
        friendArea = battleArea[1];
        enemyArea = battleArea[0];
    }
    for (int i = 0; i < BATTLE_POS_MAX; i++)
    {
        ActorPtr actor = friendArea->actors[i];
        if (actor != NULL && actor->IsAlive())
        {
            int friendDeadXpRecovery = actor->GetAttrByType(FriendDeadXpRecovery) * actor->GetAttrByType(XpGainRate);
            if (friendDeadXpRecovery > 0)
            {
                if (actor->ChangeXp(friendDeadXpRecovery))
                {
                    auto param = FillXpChangeParam(actor, actor, 0, friendDeadXpRecovery);
                    param.recovery = XpRecoveryType::AllySlain;
                    param.slainActorID = target->actorId;
                    FillXpChangeCmd(param);
                }
            }
        }
    }
    for (int i = 0; i < BATTLE_POS_MAX; i++)
    {
        ActorPtr actor = enemyArea->actors[i];
        if (actor != NULL && actor->IsAlive())
        {
            int enemyDeadXpRecovery = actor->GetAttrByType(EnemyDeadXpRecovery) * actor->GetAttrByType(XpGainRate);
            if (enemyDeadXpRecovery > 0)
            {
                if (actor->ChangeXp(enemyDeadXpRecovery))
                {
                    auto param = FillXpChangeParam(actor, actor, 0, enemyDeadXpRecovery);
                    param.recovery = XpRecoveryType::EnemySlain;
                    param.slainActorID = target->actorId;
                    FillXpChangeCmd(param);
                }
            }
        }
    }
}

int BattleField::CalSkillRandomInterval()
{
    TRACE("BattleField::CalSkillRandomInterval");
    if (LastComSkill == NULL)
    {
        //error("LastComSkill == NULL");
        return 0;
    }
    int rate = random.RandForWidth(100);
    int targetMark = 0;
    int accuMark = 0;
    for (int i = 0; i < DOUBLE_HIT_PROBALITY_COUNT; i++)
    {
        accuMark += doubleHitProbalility[DOUBLE_HIT_PROBALITY_COUNT - 1 - i];
        if (rate <= accuMark)
        {
            targetMark = i;
            break;
        }
    }
    int doubleHitTime = (LastComSkill->StartComboTime + LastComSkill->EndComboTime)* 0.5f + LastComSkill->Atk123Time;
    if (targetMark > 0)
    {
        for (int i = 0; i < targetMark; i++)
        {
            doubleHitTime -= doubleHitTimeRadius[i];
        }
        doubleHitTime -= 10;
    }
    return doubleHitTime;
}
int BattleField::CalcAttackTime(int atkArea)
{
    TRACE("BattleField::CalcAttackTime");
    //lastAttackTime
    
    if (CurBattleMode == nullptr)
    {
        return 0;
    }

    if (atkArea == 0)
    {
        if (CurBattleMode->ComboRule_Attacker == EBMCombo::True)
        {
            return CalSkillRandomInterval();
        }
        else
        { 
            return AtkMyInterval;
        }
    }
    else
    {
        if (CurBattleMode->ComboRule_Defender == EBMCombo::True)
        {
            return CalSkillRandomInterval();
        }
        else
        {
            return AtkOppoInterval;
        }
    }
}

bool BattleField::PreFireEvent(TriggerParam & param){
    if (pTriggerMgr != NULL)
    {
        param.r1 = roundCount;
        return pTriggerMgr->PreFireEvent(param);
    }
    return false;
}

void BattleField::FireEvent(TriggerParam & param)
{
    if (pTriggerMgr != NULL)
    {
        param.r1 = roundCount;
        pTriggerMgr->FireEvent(param);
    }
}

void BattleField::AppendDelayAction(const DelayDoAction & doAction, bool bigRound)
{
    if (bigRound)
    {
        bigRoundDelayAction.push_back(doAction);
    } 
    else
    {
        smallRoundDelayAction.push_back(doAction);
    }
}

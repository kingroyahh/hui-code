#include "Log.h"
#include "Battle.h"
#include "Define.h"
#include "Profiler.h"
#include "Battle.h"
#include "BattleField.h"
#include "CombatAttrMeta.h"
#include "BattleParams.pb.h"
#include "BattleTest.pb.h"
#include "BattleAnalysis.pb.h"
#include "ValueMeta.h"
#include "SkillMeta.h"
#include "AffixMeta.h"
#include "EffectMeta.h"
#include "ModifierMeta.h"
#include "SubEffectsMeta.h"
#include <stdio.h>
#include "NativeCalc.h"
#ifdef __cplusplus
extern "C"
{
#endif
#ifdef _MSC_VER
    __declspec(dllexport) void* CreateBattle()
#else
    GRAV_API void* CreateBattle()
#endif
    {
        BattleField *battle = new BattleField();
        void *ptr = battle;
        return ptr;
    }

    int GRAV_API InitBattle(void* ptr, char* params, int paramsLength, char** data)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return -1;
        }
        TRACE("GRAV->PLAYBATTLE\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        //mark_start();
        
        BattleParams par;
        par.ParseFromArray(params, paramsLength);
        if (par.opp_area_size() <= 0)
        {
            error("no opp area cant fight !!!\n");
            return -1;
        }
        info("opp size count : %d\n", par.opp_area_size());
        for (int i = 0; i < par.opp_area_size(); ++i)
        {
            if (i > 0)
            {
                battle->Reset();
            }
            battle->curReplay = battle->replayList.add_battle_replays();
            battle->InitBattle(par, i);
            //TODO: CHECK WINNER IF DO NEXT
            if (battle->winner != 0)
            {
                break;
            }
        }
        /*mark_end();
        info("Battle time consumed in milliseconds:%ld, in microseconds:%ld\n",
            elapsed_milli(), elapsed_micro());*/

        int size = battle->replayList.ByteSize();
        battle->CleanReplayBuf();
        battle->replayListBuf = (char*)malloc(sizeof(char)*size);
        battle->replayList.SerializeToArray(battle->replayListBuf, size);
        *data = battle->replayListBuf;
        battle->replayList.Clear();
        return size;
    }

    int GRAV_API VerifyBattle(void* ptr, char* params, int paramsLength, char** data)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return -1;
        }
        TRACE("GRAV->PLAYBATTLE\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        mark_start();
        BattleParams par;
        par.ParseFromArray(params, paramsLength);
        if (par.commands_size() == 0)
        {
            error("no user command !!!\n");
            return -1;
        }
        if (par.opp_area_size() <= 0)
        {
            error("no opp area cant fight !!!\n");
            return -1;
        }
        info("opp size count : %d\n", par.opp_area_size());
        for (int i = 0; i < par.opp_area_size(); ++i)
        {
            if (i > 0)
            {
                battle->Reset();
            }
            battle->curReplay = battle->replayList.add_battle_replays();
            battle->VerifyBattle(par, i);
            //TODO: CHECK WINNER IF DO NEXT
            if (battle->winner != 0)
            {
                break;
            }
        }
        mark_end();
        info("Battle time consumed in milliseconds:%ld, in microseconds:%ld\n",
            elapsed_milli(), elapsed_micro());
        int size = battle->replayList.ByteSize();
        battle->CleanReplayBuf();
        battle->replayListBuf = (char*)malloc(sizeof(char)*size);
        battle->replayList.SerializeToArray(battle->replayListBuf, size);
        *data = battle->replayListBuf;
        battle->replayList.Clear();
        return size;
    }

    int GRAV_API PlayBattle(void* ptr, char* params, int paramLength, char** data)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return -1;
        }
        TRACE("GRAV->PLAYBATTLE\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        mark_start();
        BattleParams par;
        par.ParseFromArray(params, paramLength);

        if (par.opp_area_size() <= 0)
        {
            error("no opp area cant fight !!!\n");
            return -1;
        }

        info("opp size count : %d\n", par.opp_area_size());

        for (int i = 0; i < par.opp_area_size(); ++i)
        {

            if(i > 0)
            {
                battle->Reset();
            }

            battle->curReplay = battle->replayList.add_battle_replays();
    
            battle->DoBattle(par, i);
            //TODO: CHECK WINNER IF DO NEXT
            if (battle->winner != 0)
            {
                break;
            }
        }
        
        mark_end();
        info("Battle time consumed in milliseconds:%ld, in microseconds:%ld\n",
            elapsed_milli(), elapsed_micro());
        
        int size = battle->replayList.ByteSize();
        battle->CleanReplayBuf();
        battle->replayListBuf = (char*)malloc(sizeof(char)*size);
        battle->replayList.SerializeToArray(battle->replayListBuf, size);
        *data = battle->replayListBuf;
        battle->replayList.Clear();
        return size;
    }
    
    int GRAV_API SetBattleAnalysis(void* ptr, bool analysis)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return 0;
        }
        
        info("GRAV->SET ANALYSIS %s\n",analysis==false?"FALSE":"TRUE");
        
        BattleField *battle = static_cast<BattleField *>(ptr);
        battle->SetAnalysis(analysis);
        return 0;
    }
    
    int GRAV_API GetStatistics(void* ptr, char** statis)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return 0;
        }
        
        info("GRAV->GETSTATISTICS\n");
        
        BattleField *battle = static_cast<BattleField *>(ptr);
        
        int staSize = battle->statistics.ByteSize();
        battle->CleanReplayBuf();
        battle->replayListBuf = (char*)malloc(sizeof(char)*staSize);
        battle->statistics.SerializeToArray(battle->replayListBuf, staSize);
        *statis = battle->replayListBuf;
        return staSize;
    }
    
    int GRAV_API GetAnalysis(void* ptr, char** analysis)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return 0;
        }
        
        info("GRAV->GETANALYSIS\n");
        
        BattleField *battle = static_cast<BattleField *>(ptr);

        int anaSize = battle->analysis.ByteSize();
        battle->CleanReplayBuf();
        battle->replayListBuf = (char*)malloc(sizeof(char)*anaSize);
        battle->analysis.SerializeToArray(battle->replayListBuf, anaSize);
        *analysis = battle->replayListBuf;
        info("GRAV->GETANALYSIS    size[%d]",anaSize);
        return anaSize;
    }

    int GRAV_API DestroyBattle(void* ptr)
    {
        if (ptr == NULL_PTR)
        {
            return 0;
        }
        BattleField *battle = static_cast<BattleField *>(ptr);
        battle->Reset();
        delete battle;
        return 1;
    }
    
    int GRAV_API SetXpSkill(void* ptr, int posIndex)
    {
        /*if (ptr == NULL_PTR)
        {
            return 0;
        }
        BattleField *battle = static_cast<BattleField *>(ptr);
        battle->SetXpStart(posIndex);*/
        return 1;
    }
    int GRAV_API GetRecord(void* ptr, char** data)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return 0;
        }
        info("GRAV->GETCURREPLAY\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        int staSize = 0;
        if (battle->curUserRecord != NULL_PTR)
        {
            staSize = battle->curUserRecord->ByteSize();
            battle->CleanReplayBuf();
            battle->replayListBuf = (char*)malloc(sizeof(char)*staSize);
            battle->curUserRecord->SerializeToArray(battle->replayListBuf, staSize);
            *data = battle->replayListBuf;
        }
        return staSize;
    }
    int GRAV_API AppendBattleRecord(void* ptr)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return 0;
        }
        info("GRAV->GETCURREPLAY\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        if (battle->curReplayCmd != NULL_PTR)
        {
            error("curReplayCmd is not null!");
            return 0;
        }
        if (battle->curUserRecord == NULL_PTR)
        {
            battle->curUserRecord = new UserCommand();
        }
        battle->curBattleRecord = battle->curUserRecord->add_battlerecord();
        return 1;
    }
    int GRAV_API AppendRoundRecord(void* ptr)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return 0;
        }
        info("GRAV->GETCURREPLAY\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        if (battle->curReplayCmd != NULL_PTR)
        {
            error("curReplayCmd is not null!");
            return 0;
        }
        if (battle->curRoundCmd != NULL_PTR)
        {
            error("curRoundCmd is not null!");
            return 0;
        }
        if (battle->curUserRecord == NULL_PTR)
        {
            error("curUserRecord is null!,forget to call 'AppendBattleRecord' ?");
            return 0;
        }
        if (battle->curBattleRecord == NULL_PTR)
        {
            error("curBattleRecord is null!,forget to call 'AppendBattleRecord' ?");
            return 0;
        }
        battle->curRoundRecord = battle->curBattleRecord->add_roundrecord();
        return 1;
    }
    int GRAV_API BeginRound(void* ptr, int areaIndex, char** data)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return -1;
        }
        info("GRAV->BeginRound\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        if (battle->curReplayCmd != NULL_PTR)
        {
            error("BeginRound curReplayCmd is not null!");
            return -1;
        }
        if (battle->curRoundCmd != NULL_PTR)
        {
            error("BeginRound curRoundCmd is not null!");
            return -1;
        }
        if (battle->curUserRecord == NULL_PTR)
        {
            error("curUserRecord is null!,forget to call 'AppendBattleRecord' ?");
            return -1;
        }
        if (battle->curBattleRecord == NULL_PTR)
        {
            error("curBattleRecord is null!,forget to call 'AppendBattleRecord' ?");
            return -1;
        }
        if (battle->curRoundRecord == NULL_PTR)
        {
            error("curRoundRecord is null!,forget to call 'AppendRoundRecord' ?");
            return -1;
        }
        RoundCmd roundcmd;
        battle->curRoundCmd = &roundcmd;
        if (battle->firstArea == areaIndex)
        {
            battle->roundCount++;
        }
        battle->activeRoundCount++;
        if (!battle->BeginRound(areaIndex))
        {
            return -1;
        }
        int staSize = 0;
        if (battle->curRoundCmd != NULL_PTR)
        {
            staSize = battle->curRoundCmd->ByteSize();
            battle->CleanReplayBuf();
            battle->replayListBuf = (char*)malloc(sizeof(char)*staSize);
            battle->curRoundCmd->SerializeToArray(battle->replayListBuf, staSize);
            *data = battle->replayListBuf;
        }
        battle->curRoundCmd = NULL_PTR;
        return staSize;
    }
    
    int GRAV_API CastSkillByPos(void* ptr, int attackTime, int areaIndex, int posIndex, int tarAreaIndex, int tarPosIndex, bool skill, char** statis)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return -1;
        }
        info("GRAV->GETCURREPLAY\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        if (battle->curReplayCmd != NULL_PTR)
        {
            error("curReplayCmd is not null!");
            return -1;
        }
        if (battle->curRoundCmd != NULL_PTR)
        {
            error("curRoundCmd is not null!");
            return -1;
        }
        if (battle->curUserRecord == NULL_PTR)
        {
            error("curUserRecord is null!,forget to call 'AppendBattleRecord' ?");
            return -1;
        }
        if (battle->curBattleRecord == NULL_PTR)
        {
            error("curBattleRecord is null!,forget to call 'AppendBattleRecord' ?");
            return -1;
        }
        if (battle->curRoundRecord == NULL_PTR)
        {
            error("curRoundRecord is null!,forget to call 'AppendRoundRecord' ?");
            return -1;
        }
        AttackRecord *atkRecord = battle->curRoundRecord->add_attackrecord();
        atkRecord->set_skill(skill);
        atkRecord->set_atk_area_index(areaIndex);
        atkRecord->set_atk_pos_index(posIndex);
        atkRecord->set_attacktime(attackTime);
        atkRecord->set_target_area_index(0);
        atkRecord->set_target_pos_index(tarPosIndex);

        battle->lastAttackTime = battle->curAttackTime;
        battle->curAttackTime = attackTime;
        battle->curOrder = 0;
        RoundCmd roundcmd;
        battle->curRoundCmd = &roundcmd;
        battle->CastSkillByPos(areaIndex, posIndex, tarPosIndex, skill);
        int staSize = 0;
        if (battle->curRoundCmd != NULL_PTR)
        {
            staSize = battle->curRoundCmd->ByteSize();
            battle->CleanReplayBuf();
            battle->replayListBuf = (char*)malloc(sizeof(char)*staSize);
            battle->curRoundCmd->SerializeToArray(battle->replayListBuf, staSize);
            *statis = battle->replayListBuf;
        }
        battle->curRoundCmd = NULL_PTR;
        battle->curReplayCmd = NULL_PTR;
        battle->curSkillCmd = NULL_PTR;
        return staSize;
    }
    int GRAV_API EndRound(void* ptr, int areaIndex, char** data)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return -1;
        }
        info("GRAV->EndRound\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        if (battle->curReplayCmd != NULL_PTR)
        {
            error("BeginRound curReplayCmd is not null!");
            return -1;
        }
        if (battle->curRoundCmd != NULL_PTR)
        {
            error("BeginRound curRoundCmd is not null!");
            return -1;
        }
        if (battle->curUserRecord == NULL_PTR)
        {
            error("curUserRecord is null!,forget to call 'AppendBattleRecord' ?");
            return -1;
        }
        if (battle->curBattleRecord == NULL_PTR)
        {
            error("curBattleRecord is null!,forget to call 'AppendBattleRecord' ?");
            return -1;
        }
        if (battle->curRoundRecord == NULL_PTR)
        {
            error("curRoundRecord is null!,forget to call 'AppendRoundRecord' ?");
            return -1;
        }
        RoundCmd roundcmd;
        battle->curRoundCmd = &roundcmd;
        if (!battle->EndRound(areaIndex))
        {
            return -1;
        }
        int staSize = 0;
        if (battle->curRoundCmd != NULL_PTR)
        {
            staSize = battle->curRoundCmd->ByteSize();
            battle->CleanReplayBuf();
            battle->replayListBuf = (char*)malloc(sizeof(char)*staSize);
            battle->curRoundCmd->SerializeToArray(battle->replayListBuf, staSize);
            *data = battle->replayListBuf;
        }
        battle->curRoundCmd = NULL_PTR;
        return staSize;
    }
    int GRAV_API SetRandomSeed(void* ptr, int seedValue)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return 0;
        }
        
        info("GRAV->GETCURREPLAY\n");
        
        BattleField *battle = static_cast<BattleField *>(ptr);
        
        battle->random.SetRandomSeed(seedValue);
        return 0;
    }

    int GRAV_API SetInfinitSkill(void* ptr, bool enable)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return 0;
        }
        info("GRAV->SetInfinitSkill\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        battle->InfiniteSkill = enable;
        return 1;
    }

    int GRAV_API SetSkillTestMode(void* ptr, bool skillTest)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return 0;
        }
        info("GRAV->SetSkillTestMode\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        battle->SkillTestMode = skillTest;
        return 1;
    }
    int GRAV_API SetInitCombatAttr(void* ptr, bool initCombatAttr)
    {
        if (ptr == NULL_PTR)
        {
            error("PTR is null!");
            return 0;
        }
        info("GRAV->SetInitCombatAttr\n");
        BattleField *battle = static_cast<BattleField *>(ptr);
        battle->InitCombatAttr = initCombatAttr;
        return 1;
    }
    
#ifdef __cplusplus
}
#endif

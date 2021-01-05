#ifndef _GRAVE_BATTLE_H_
#define _GRAVE_BATTLE_H_

#include "Define.h"

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef _MSC_VER
    __declspec(dllexport) void* CreateBattle();
#else
    GRAV_API void* CreateBattle();
#endif
    int GRAV_API SetBattleAnalysis(void* battle,bool bAnalysis);
    int GRAV_API GetStatistics(void* battle, char** statis);
    int GRAV_API SetInfinitSkill(void* battle, bool enable);
    int GRAV_API SetSkillTestMode(void* battle, bool skillTest);
    int GRAV_API SetInitCombatAttr(void* battle, bool initCombatAttr);
    int GRAV_API InitBattle(void* battle, char* params, int paramsLength, char** data);
    int GRAV_API VerifyBattle(void* battle, char* params, int paramsLength, char** data);
    int GRAV_API PlayBattle(void* battle, char* params, int paramsLength, char** data);
    int GRAV_API DestroyBattle(void* battle);

    int GRAV_API GetRecord(void* ptr, char** data);
    int GRAV_API AppendBattleRecord(void* ptr);
    int GRAV_API AppendRoundRecord(void* ptr);
    int GRAV_API BeginRound(void* ptr, int areaIndex,char** data);//deal with buff relatives..,,consider verify info later...
    int GRAV_API CastSkillByPos(void* ptr, int attackTime,int areaIndex, int posIndex, int tarAreaIndex, int tarPosIndex, bool skill, char** statis);
    int GRAV_API EndRound(void* ptr, int areaIndex,char** data);
    int GRAV_API SetRandomSeed(void* ptr, int seedValue);
#ifdef __cplusplus
}
#endif

#endif

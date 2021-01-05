#ifndef _GRAV_META_H_
#define _GRAV_META_H_

#include "Define.h"

typedef enum
{
    TSkill = 0,
    TValue = 1,
    TAffix = 2,
    TEffect = 3,
    TModifier = 4,
    TSubEffect = 5,
    TCombatAttr = 6,
    TBattleMode = 7,
    TActor = 8,
    TTrigger = 9,
    TTriggerCondition = 10,
} MetaType;

#ifdef __cplusplus
extern "C"
{
#endif

void GRAV_API SetMetaRootPath(char *);

#ifdef _MSC_VER
__declspec(dllexport) const char* GetMetaRootPath();
#else
const char* GRAV_API GetMetaRootPath();
#endif

void GRAV_API CloseAllSets();
void GRAV_API LoadDBAll(int);
void GRAV_API CalcPrepare();
void GRAV_API SetDBConfig(int, const char *, const char *);

#ifdef __cplusplus
}
#endif

#endif

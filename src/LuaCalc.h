//
//  LuaCalc.hpp
//  grav
//
//  Created by Andrew Li on 7/26/16.
//  Copyright © 2016 Babeltime. All rights reserved.
//

#ifndef _GRAV_LUA_CALC_H_
#define _GRAV_LUA_CALC_H_

#ifndef GV_COMBAT_TYPE
#define GV_COMBAT_TYPE "CombatType"
#endif

#ifndef GV_BLOCK_RATE_ARG
#define GV_BLOCK_RATE_ARG "BlockRateArg"
#endif

#ifndef GV_STRIKE_RATE_ARG
#define GV_STRIKE_RATE_ARG "StrikeRateArg"
#endif

#ifndef GV_DAMAGE_ADDITION_ARG
#define GV_DAMAGE_ADDITION_ARG "DamageAdditionArg"
#endif

#ifndef GV_DAMAGE_REDUCTION_ARG
#define GV_DAMAGE_REDUCTION_ARG "DamageReductionArg"
#endif

#ifndef GV_DAMAGE_FIX_1
#define GV_DAMAGE_FIX_1 "DamageFix1"
#endif

#ifndef GV_DAMAGE_FIX_2
#define GV_DAMAGE_FIX_2 "DamageFix2"
#endif

#ifndef GF_CALC_LIFE_STEAL
#define GF_CALC_LIFE_STEAL "CalculateLifeSteal"
#endif

#ifndef GF_CALC_HEAL
#define GF_CALC_HEAL "CalculateHeal"
#endif

#ifndef GF_CALC_HIT_CHANCE
#define GF_CALC_HIT_CHANCE "CalculateHitChance"
#endif

#ifndef GF_CALC_STRIKE_DAMAGE
#define GF_CALC_STRIKE_DAMAGE "CalculateStrikeDamage"
#endif

#ifndef GF_CALC_STRIKE_CHANCE
#define GF_CALC_STRIKE_CHANCE "CalculateStrikeChance"
#endif

#ifndef GF_CALC_BLOCK_DAMAGE
#define GF_CALC_BLOCK_DAMAGE "CalculateBlockDamage"
#endif

#ifndef GF_CALC_BLOCK_CHANCE
#define GF_CALC_BLOCK_CHANCE "CalculateBlockChance"
#endif

#ifndef GF_CALC_DAMAGE
#define GF_CALC_DAMAGE "CalculateDamage"
#endif

#ifndef GF_LUA_LOG
#define GF_LUA_LOG "log"
#endif

#ifndef GF_CALC_ATTR_CALC_PATH
#define GF_CALC_ATTR_CALC_PATH "AttrCalc.lua"
#endif

#ifndef GF_CALC_DAMAGE_CALC_PATH
#define GF_CALC_DAMAGE_CALC_PATH "DamageCalc.lua"
#endif

#ifndef GF_CALC_ENUM_PATH
#define GF_CALC_ENUM_PATH "Enum.lua"
#endif

#include "BaseCalc.h"
#include "Define.h"
#include "Singleton.h"
#include "BattleModeMeta.h"

#ifdef GRAV_LUA_CALC
extern "C"
{
    #include "luajit/lua.h"
    #include "luajit/lualib.h"
    #include "luajit/lauxlib.h"
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    
    void GRAV_API SetLuaRootPath(char*);
    
#ifdef __cplusplus
}
#endif

extern char* lua_root_path;
extern int lua_root_len;

class LuaCalc :public Singleton<LuaCalc>
{
private:
    
    
#ifdef GRAV_LUA_CALC
    lua_State* L;
#endif
    
    void lua_pushactor(ActorPtr);
    
    float lua_callfunc(const char*, int);
    
public:
    LuaCalc();
    ~LuaCalc();
    
    
public:
    void SetCombatType(EBattleType bType);
    
    void SetBlockRateArg(float) ;
    
    void SetStrikeRateArg(float) ;
    
    void SetDamageAdditionArg(float);
    
    void SetDamageReductionArg(float) ;
    
    void SetDamageFix1(float) ;
    
    void SetDamageFix2(float) ;
    
    float CalculateLifeSteal(float, ActorPtr) ;
    
    float CalculateHeal(ActorPtr, ActorPtr, float, float) ;
    
    float CalculateHitChance(DamageType, ActorPtr, ActorPtr) ;
    
    float CalculateStrikeDamage(float, ActorPtr, ActorPtr) ;
    float CalculateStrikeChance(DamageType, ActorPtr, ActorPtr) ;
    
    float CalculateBlockDamage(float damage) ;
    float CalculateBlockChance(DamageType, ActorPtr, ActorPtr) ;
    
    float CalculateDamage(DamageType, ActorPtr, ActorPtr,float, float, float) ;
    
    float CalculateShield(ActorAttrType, ActorPtr, float, float) ;

};

#endif /* _GRAV_LUA_CALC_H_ */

//
//  LuaCalc.cpp
//  grav
//
//  Created by Andrew Li on 7/26/16.
//  Copyright © 2016 Babeltime. All rights reserved.
//
#include "LuaCalc.h"
#include "Log.h"
#include "Actor.h"
#include "CombatAttrMeta.h"

using namespace std;

#if GRAV_LUA_CALC
static int lua_log(lua_State* L)
{
    int nargs = lua_gettop(L);
    if (nargs != 2)
    {
        return 1;
    }
    int level = lua_tonumber(L, 1);
    const char* msg = lua_tostring(L, 2);
    
    if (msg)
    {
        switch (level)
        {
        case 0:
            verb(msg);
            break;
        case 1:
            debug(msg);
        case 2:
            info(msg);
        case 3:
            warning(msg);
        case 4:
            error(msg);
        default:
            break;
        }
    }
    
    return 0;
}
#endif

void LuaCalc::lua_pushactor(ActorPtr actor)
{
#if GRAV_LUA_CALC
    lua_newtable(L);
    map<int, CombatAttrMeta*>::iterator it;
    for (it = CombatAttrMetaSet::instance()->Begin(); it != CombatAttrMetaSet::instance()->End(); ++it)
    {
        lua_pushnumber(L, it->first);
        lua_pushnumber(L, actor->GetAttrByType((ActorAttrType)it->first));
        lua_rawset(L, -3);
    }
#endif
}

float LuaCalc::lua_callfunc(const char *func, int narg)
{
#if GRAV_LUA_CALC
    if (lua_pcall(L, narg, 1, 0))
    {
        error("error running lua func \"%s\" %s\n", func, lua_tostring(L, -1));
    }
    
    if (!lua_isnumber(L, -1))
    {
        error("lua func \"%s\" must return a number!\n", func);
    }
    
    float v = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    return v;
#else
    return 0;
#endif
}

LuaCalc::LuaCalc()
{
#if GRAV_LUA_CALC
    L = lua_open();
    
    if (!L)
    {
        error("open lua failed");
    }
    else
    {
        luaL_openlibs(L);
        
        lua_register(L, GF_LUA_LOG, lua_log);
        
        //TODO: do files
        
        int key_len = (int)strlen(GF_CALC_ENUM_PATH);
        int path_len = lua_root_len + key_len;
        char* tmp = new char[path_len+1];
        strcpy(tmp, lua_root_path);
        strcat(tmp, GF_CALC_ENUM_PATH);
        const char* enumPath = (const char*)tmp;
        
        if(luaL_dofile(L, enumPath))
        {
            error("lua do file error: %s\n", enumPath);
        }
        
        key_len = (int)strlen(GF_CALC_ATTR_CALC_PATH);
        path_len = lua_root_len + key_len;
        tmp = new char[path_len+1];
        strcpy(tmp, lua_root_path);
        strcat(tmp, GF_CALC_ATTR_CALC_PATH);
        const char* attrCalcPath = (const char*)tmp;
        
        if(luaL_dofile(L, attrCalcPath))
        {
            error("lua do file error: %s\n", attrCalcPath);
        }
        
        key_len = (int)strlen(GF_CALC_DAMAGE_CALC_PATH);
        path_len = lua_root_len + key_len;
        tmp = new char[path_len+1];
        strcpy(tmp, lua_root_path);
        strcat(tmp, GF_CALC_DAMAGE_CALC_PATH);
        const char* dmgCalcPath = (const char*)tmp;
        
        if(luaL_dofile(L, dmgCalcPath))
        {
            error("lua do file error: %s\n", dmgCalcPath);
        }
        
    }
#endif
}

LuaCalc::~LuaCalc()
{
#if GRAV_LUA_CALC
    if (L)
    {
        lua_close(L);
    }
#endif
}

void LuaCalc::SetCombatType(EBattleType bType)
{
#if GRAV_LUA_CALC
    lua_pushnumber(L, combatType);
    lua_setglobal(L, GV_COMBAT_TYPE);
#endif
}

void LuaCalc::SetBlockRateArg(float val)
{
#if GRAV_LUA_CALC
    lua_pushnumber(L, blockRateArg);
    lua_setglobal(L, GV_BLOCK_RATE_ARG);
#endif
}

void LuaCalc::SetStrikeRateArg(float val)
{
#if GRAV_LUA_CALC
    lua_pushnumber(L, strikeRateArg);
    lua_setglobal(L, GV_STRIKE_RATE_ARG);
#endif
}

void LuaCalc::SetDamageAdditionArg(float val)
{
#if GRAV_LUA_CALC
    lua_pushnumber(L, damageAdditionArg);
    lua_setglobal(L, GV_DAMAGE_ADDITION_ARG);
#endif
}

void LuaCalc::SetDamageReductionArg(float val)
{
#if GRAV_LUA_CALC
    lua_pushnumber(L, damageReductionArg);
    lua_setglobal(L, GV_DAMAGE_REDUCTION_ARG);
#endif
}

void LuaCalc::SetDamageFix1(float val)
{
#if GRAV_LUA_CALC
    lua_pushnumber(L, damageFix1);
    lua_setglobal(L, GV_DAMAGE_FIX_1);
#endif
}

void LuaCalc::SetDamageFix2(float val)
{
#if GRAV_LUA_CALC
    lua_pushnumber(L, damageFix2);
    lua_setglobal(L, GV_DAMAGE_FIX_2);
#endif
}

float LuaCalc::CalculateLifeSteal(float damage, ActorPtr caster)
{
#if GRAV_LUA_CALC
    if (!L)
    {
        error("Lua state not inited!");
        return 0;
    }
    lua_getglobal(L, GF_CALC_LIFE_STEAL);
    lua_pushnumber(L, damage);
    lua_pushactor(caster);
    
    return lua_callfunc(GF_CALC_LIFE_STEAL, 2);
#else
    return 0;
#endif
}


float LuaCalc::CalculateHeal(ActorPtr caster, ActorPtr target, float skillFactor, float skillDamage)
{
#if GRAV_LUA_CALC
    if (!L)
    {
        error("Lua state not inited!");
        return 0;
    }
    lua_getglobal(L, GF_CALC_HEAL);
    lua_pushactor(caster);
    lua_pushactor(target);
    lua_pushnumber(L, skillFactor);
    lua_pushnumber(L, skillDamage);
    
    return lua_callfunc(GF_CALC_HEAL, 4);
#else
    return 0;
#endif
}

float LuaCalc::CalculateHitChance(DamageType dType, ActorPtr caster, ActorPtr target)
{
#if GRAV_LUA_CALC
    if (!L)
    {
        error("Lua state not inited!");
        return 0;
    }
    lua_getglobal(L, GF_CALC_HIT_CHANCE);
    lua_pushnumber(L, dType);
    lua_pushactor(caster);
    lua_pushactor(target);
    
    return lua_callfunc(GF_CALC_HIT_CHANCE, 3);
#else
    return 0;
#endif
}

float LuaCalc::CalculateStrikeDamage(float damage, ActorPtr caster, ActorPtr target)
{
#if GRAV_LUA_CALC
    if (!L)
    {
        error("Lua state not inited!");
        return 0;
    }
    lua_getglobal(L, GF_CALC_STRIKE_DAMAGE);
    lua_pushnumber(L, damage);
    lua_pushactor(caster);
    lua_pushactor(target);
    
    return lua_callfunc(GF_CALC_STRIKE_DAMAGE, 3);
#else
    return 0;
#endif
}

float LuaCalc::CalculateStrikeChance(DamageType dType, ActorPtr caster, ActorPtr target)
{
#if GRAV_LUA_CALC
    if (!L)
    {
        error("Lua state not inited!");
        return 0;
    }
    lua_getglobal(L, GF_CALC_STRIKE_CHANCE);
    lua_pushnumber(L, dType);
    lua_pushactor(caster);
    lua_pushactor(target);
    
    return lua_callfunc(GF_CALC_STRIKE_CHANCE, 3);
#else
    return 0;
#endif
}

float LuaCalc::CalculateBlockDamage(float damage)
{
#if GRAV_LUA_CALC
    if (!L)
    {
        error("Lua state not inited!");
        return 0;
    }
    lua_getglobal(L, GF_CALC_BLOCK_DAMAGE);
    lua_pushnumber(L, damage);
    
    return lua_callfunc(GF_CALC_BLOCK_DAMAGE, 3);
#else
    return 0;
#endif
}

float LuaCalc::CalculateBlockChance(DamageType dType, ActorPtr caster, ActorPtr target)
{
#if GRAV_LUA_CALC
    if (!L)
    {
        error("Lua state not inited!");
        return 0;
    }
    lua_getglobal(L, GF_CALC_BLOCK_CHANCE);
    lua_pushnumber(L, dType);
    lua_pushactor(caster);
    lua_pushactor(target);
    
    return lua_callfunc(GF_CALC_BLOCK_CHANCE, 3);
#else
    return 0;
#endif
}

float LuaCalc::CalculateDamage(DamageType dType, ActorPtr caster, ActorPtr target,
                              float skillFactor, float skillDamage, float skillFinalDamage)
{
#if GRAV_LUA_CALC
    if (!L)
    {
        error("Lua state not inited!");
        return 0;
    }
    lua_getglobal(L, GF_CALC_DAMAGE);
    lua_pushnumber(L, dType);
    lua_pushactor(caster);
    lua_pushactor(target);
    lua_pushnumber(L, skillFactor);
    lua_pushnumber(L, skillDamage);
    lua_pushnumber(L, skillFinalDamage);
    
    return lua_callfunc(GF_CALC_DAMAGE, 6);
#else
    return 0;
#endif
}

//TODO : LUA
float LuaCalc::CalculateShield(ActorAttrType attrType, ActorPtr actor, float skillFactor, float skillValue)
{
    float shieldValue = 0;
    return shieldValue;
}

char* lua_root_path = nullptr;
int lua_root_len = 0;

#ifdef __cplusplus
extern "C"
{
#endif
    
    void GRAV_API SetLuaRootPath(char* path)
    {
        int len = (int)strlen(path);
        lua_root_path = new char[len + 1];
        strcpy(lua_root_path, path);
        lua_root_len = len;
    }
    
#ifdef __cplusplus
}
#endif

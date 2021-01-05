#include "Meta.h"
#include "Define.h"

#include "SkillMeta.h"
#include "ValueMeta.h"
#include "AffixMeta.h"
#include "EffectMeta.h"
#include "ModifierMeta.h"
#include "SubEffectsMeta.h"
#include "CombatAttrMeta.h"
#include "NativeCalc.h"
#include "ActorMeta.h"
#include "TriggerMeta.h"
#include "FightCondMeta.h"
string meta_root_path("");

#ifdef __cplusplus
extern "C"
{
#endif

void GRAV_API SetMetaRootPath(char* path)
{
    TRACE("Meta::SetMetaRootPath [%s]", path);
    meta_root_path.clear();
    meta_root_path.append(path);

#ifndef GRAV_UNITY_PLUGIN
    ValueMetaSet::instance()->LoadAll();
    CombatAttrMetaSet::instance()->LoadAll();
    NativeCalc::instance()->LoadAll();
    SkillMetaSet::instance()->LoadAll();
    AffixMetaSet::instance()->LoadAll();
    EffectMetaSet::instance()->LoadAll();
    ModifierMetaSet::instance()->LoadAll();
    SubEffectsMetaSet::instance()->LoadAll();
    BattleModeMetaSet::instance()->LoadAll();
    ActorMetaSet::instance()->LoadAll();
    TriggerMetaSet::instance()->LoadAll();
    FightCondMetaSet::instance()->LoadAll();
#endif
}

#ifdef _MSC_VER
__declspec(dllexport) const char* GetMetaRootPath()
#else
const char* GRAV_API GetMetaRootPath()
#endif
{
    return meta_root_path.c_str();
}

void GRAV_API CloseAllSets()
{
    SkillMetaSet::instance()->Close();
    ValueMetaSet::instance()->Close();
    AffixMetaSet::instance()->Close();
    EffectMetaSet::instance()->Close();
    ModifierMetaSet::instance()->Close();
    SubEffectsMetaSet::instance()->Close();
    CombatAttrMetaSet::instance()->Close();
    BattleModeMetaSet::instance()->Close();
    ActorMetaSet::instance()->Close();
    TriggerMetaSet::instance()->Close();
    FightCondMetaSet::instance()->Close();
}

//ugly function.... -_^!
void GRAV_API SetDBConfig(int type, const char *dbfname, const char *dbtname)
{
    switch (type)
    {
        case TSkill:
            SkillMetaSet::instance()->Config(dbfname, dbtname);
            break;
        case TValue:
            ValueMetaSet::instance()->Config(dbfname, dbtname);
            break;
        case TAffix:
            AffixMetaSet::instance()->Config(dbfname, dbtname);
            break;
        case TEffect:
            EffectMetaSet::instance()->Config(dbfname, dbtname);
            break;
        case TModifier:
            ModifierMetaSet::instance()->Config(dbfname, dbtname);
            break;
        case TSubEffect:
            SubEffectsMetaSet::instance()->Config(dbfname, dbtname);
            break;
        case TCombatAttr:
            CombatAttrMetaSet::instance()->Config(dbfname, dbtname);
            break;
        case TBattleMode:
            BattleModeMetaSet::instance()->Config(dbfname, dbtname);
            break;
        case TActor:
            ActorMetaSet::instance()->Config(dbfname, dbtname);
            break;
        case  TTrigger:
            TriggerMetaSet::instance()->Config(dbfname, dbtname);
            break;
        case TTriggerCondition:
            FightCondMetaSet::instance()->Config(dbfname, dbtname);
            break;
        default:
            break;
    }
}
    
void GRAV_API LoadDBAll(int type)
{
    switch (type)
    {
        case TSkill:
            SkillMetaSet::instance()->LoadAll();
            break;
        case TValue:
            ValueMetaSet::instance()->LoadAll();
            break;
        case TAffix:
            AffixMetaSet::instance()->LoadAll();
            break;
        case TEffect:
            EffectMetaSet::instance()->LoadAll();
            break;
        case TModifier:
            ModifierMetaSet::instance()->LoadAll();
            break;
        case TSubEffect:
            SubEffectsMetaSet::instance()->LoadAll();
            break;
        case TCombatAttr:
            CombatAttrMetaSet::instance()->LoadAll();
            break;
        case TBattleMode:
            BattleModeMetaSet::instance()->LoadAll();
            break;
        case TActor:
            ActorMetaSet::instance()->LoadAll();
            break;
        default:
            info("LoadDBAll error [%d]", type);
            break;
    }
}

void GRAV_API CalcPrepare()
{
    NativeCalc::instance()->LoadAll();
}

#ifdef __cplusplus
}
#endif

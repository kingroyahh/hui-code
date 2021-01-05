#include "AnalysisUtil.h"
#include "Enum.h"
#include "Actor.h"
#include "ActorDatas.h"
#include "Type.h"
#include "BattleField.h"
#include "BattleAnalysis.pb.h"
#include "Log.h"
//
//void setCurrAnalysisField(ActorPtr caster,AnalysisField aField)
//{
//    BattleField* bfp =caster->battleField;
//    
//    if(!bfp->bAnalysis)
//    {
//        return;
//    }
////    info("setCurrAnalysisField actorId:%d  curr:%d\n",caster->actorId,aField);
//    caster->analysisActorInfo->set_lastfield(aField);
//}
//void setCurrAnalysisValue(ActorPtr actor,ActorPtr target,int val)
//{
//    BattleField* bfp =actor->battleField;
//    
//    if(!bfp->bAnalysis)
//    {
//        return;
//    }
//
//    AnalysisField aField = (AnalysisField)actor->analysisActorInfo->lastfield();
////    info("analysis actorId:%d  curr:%d val:%d\n",actor->actorId,aField,val);
//    switch (aField) {
//        case skill_default:
//        {
//            analysis(actor, times_default, 1);
//            analysis(actor, damage_default, val);
//            analysis(actor, times_damage, 1);
//            analysis(target, times_damaged, 1);
//            analysis(actor,damage,val);
//            break;
//        }
//        case skill_legend:
//        {
//            analysis(actor, times_legend, 1);
//            analysis(actor, damage_legend, val);
//            analysis(actor, times_damage, 1);
//            analysis(target, times_damaged, 1);
//            analysis(actor,damage,val);
//            break;
//        }
//        case skill_xp:
//        {
//            analysis(actor, times_xp, 1);
//            analysis(actor, damage_xp, val);
//            analysis(actor, times_damage, 1);
//            analysis(target, times_damaged, 1);
//            analysis(actor,damage,val);
//            break;
//        }
//        case skill_combine:
//        {
//            analysis(actor, times_combine, 1);
//            analysis(actor, damage_combine, val);
//            analysis(actor, times_damage, 1);
//            analysis(target, times_damaged, 1);
//            analysis(actor,damage,val);
//            break;
//        }
//        case DEFAULT:
//            break;
//        default:
//            break;
//    }
//}

//void analysisEffect(ActorPtr caster,ActorPtr target,ShowEffectType effectType,int val)
//{
//    BattleField* bfp =caster->battleField;
//    
//    if(!bfp->bAnalysis)
//    {
//        return;
//    }
//    switch (effectType) {
////        case ShowDodge:
////        {
////            analysis(target, times_dodge, 1);
////            break;
////        }
//        case ShowBlock:
//        {
//            analysis(target, times_block, 1);
//            break;
//        }
//        case ShowStrike:
//        {
//            analysis(caster, times_strike, 1);
//            break;
//        }
//        case ShowLifeSteal:
//        {
//            analysis(caster, times_lifesteal, 1);
//            break;
//        }
//        default:
//        {
//            break;
//        }
//    }
//    
//    caster->analysisActorInfo->set_hp(caster->actorDatas.hp);
//    target->analysisActorInfo->set_hp(caster->actorDatas.hp);
//    
//    if(!target->actorDatas.IsAlive())
//    {
//        analysis(caster, times_kill, 1);
//        analysis(target, dead_round, target->battleField->roundCount);
//        info("%d dead,round:%d\n",target->actorId,target->battleField->roundCount);
//    }
//}
//
//void analysis(ActorPtr actor,AnalysisField aField,int val)
//{
//    BattleField* bfp =actor->battleField;
//    
//    if(!bfp->bAnalysis)
//    {
//        return;
//    }
//    
////    info("analysis actorId:%d field:%d val:%d",actor->actorId,aField,val);
//    
//    switch (aField) {
//        case damage:
//        {
//            int damage = actor->analysisActorInfo->damage();
//            damage+=val;
//            actor->analysisActorInfo->set_damage(damage);
//            break;
//        }
//        case times_damage:
//        {
//            int td = actor->analysisActorInfo->times_damage();
//            td+=val;
//            actor->analysisActorInfo->set_times_damage(td);
//            
//            break;
//        }
//        case times_damaged:
//        {
//            int td = actor->analysisActorInfo->times_damaged();
//            td+=val;
//            actor->analysisActorInfo->set_times_damaged(td);
//            break;
//        }
//        case heal:
//        {
//            int heal = actor->analysisActorInfo->heal();
//            heal+=val;
//            actor->analysisActorInfo->set_heal(heal);
//            break;
//        }
//        case damage_default:
//        {
//            int damage_default = actor->analysisActorInfo->damage_default();
//            damage_default+=val;
//            actor->analysisActorInfo->set_damage_default(damage_default);
//            break;
//        }
//        case times_default:
//        {
//            int times_default = actor->analysisActorInfo->times_default();
//            times_default+=val;
//            actor->analysisActorInfo->set_times_default(times_default);
//            break;
//        }
//        case damage_legend:
//        {
//            int damage_legend = actor->analysisActorInfo->damage_legend();
//            damage_legend+=val;
//            actor->analysisActorInfo->set_damage_legend(damage_legend);
//            break;
//        }
//        case times_legend:
//        {
//            int times_legend = actor->analysisActorInfo->times_legend();
//            times_legend+=val;
//            actor->analysisActorInfo->set_times_legend(times_legend);
//            break;
//        }
//        case damage_xp:
//        {
//            int damage_xp = actor->analysisActorInfo->damage_xp();
//            damage_xp+=val;
//            actor->analysisActorInfo->set_damage_xp(damage_xp);
//            break;
//        }
//        case times_xp:
//        {
//            int times_xp = actor->analysisActorInfo->times_xp();
//            times_xp+=val;
//            actor->analysisActorInfo->set_times_xp(times_xp);
//            break;
//        }
//        case damage_combine:
//        {
//            int damage_combine = actor->analysisActorInfo->damage_combine();
//            damage_combine+=val;
//            actor->analysisActorInfo->set_damage_combine(damage_combine);
//            break;
//        }
//        case times_combine:
//        {
//            int times_combine = actor->analysisActorInfo->times_combine();
//            times_combine+=val;
//            actor->analysisActorInfo->set_times_combine(times_combine);
//            break;
//        }
//        case times_strike:
//        {
//            int times_strike = actor->analysisActorInfo->times_strike();
//            times_strike+=val;
//            actor->analysisActorInfo->set_times_strike(times_strike);
//            break;
//        }
//        case times_dodge:
//        {
//            int times_dodge = actor->analysisActorInfo->times_dodge();
//            times_dodge+=val;
//            actor->analysisActorInfo->set_times_dodge(times_dodge);
//            break;
//        }
//        case times_block:
//        {
//            int times_block = actor->analysisActorInfo->times_block();
//            times_block+=val;
//            actor->analysisActorInfo->set_times_block(times_block);
//            break;
//        }
//        case times_lifesteal:
//        {
//            int times_lifesteal = actor->analysisActorInfo->times_lifesteal();
//            times_lifesteal+=val;
//            actor->analysisActorInfo->set_times_lifesteal(times_lifesteal);
//            break;
//        }
//        case times_kill:
//        {
//            int times_kill = actor->analysisActorInfo->times_kill();
//            times_kill+=val;
//            actor->analysisActorInfo->set_times_kill(times_kill);
//            break;
//        }
//        case dead_round:
//        {
//            actor->analysisActorInfo->set_dead_round(val);
//            break;
//        }
//        case hp:
//        {
//            actor->analysisActorInfo->set_hp(val);
//            break;
//        }
//        default:
//            break;
//    }
//}
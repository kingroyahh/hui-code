#ifndef _GRAV_EFFECT_UTIL_H_
#define _GRAV_EFFECT_UTIL_H_
#include "Enum.h"
#include "Type.h"
#include "Actor.h"
#include "EffectMeta.h"
#include "SubEffectsMeta.h"
#include "BattleReplay.pb.h"
#include "BattleField.h"
using namespace pb;
class EffectSub;
using FilterTopMostCompareFunc = Actor* (*)(Actor *curActor, Actor* lastActor, int &topValue, int &topMostCnt);
using FilterTopMostSelectFunc = bool(*)(Actor *curActor,int topValue);
void makeEffectByType(ActorPtr caster, ActorPtr target, const SubEffectsMeta* subMeta, float tarDmgParam, int skillLv, int buffId, SkillType skillType,TriggerTrace traceData);
void CheckDeadEvt(ActorPtr caster, ActorPtr target, EffectType eType);
EffectCmd* CreateSubEffect(ActorPtr caster);
void FillDamageCmd(const FillDamageParam & param);
void FillRecoveryCmd(const FillRecoveryParam& param);
void FillXpChangeCmd(const FillXpChangeParam & param);
void FillDodgeCmd(ActorPtr caster, ActorPtr target, int subEffectId, int originBuffId);
void FillBuffAttachCmd(ActorPtr caster, ActorPtr target, int originBuffId, int attachBuffID, int layerCnt, bool immune);
void FillBuffDetachCmd(ActorPtr caster, ActorPtr target, BuffVanishType vanishType, int subEffecID,int originBuffID, int detachBuffID);

void SelectEffectTargets(EffectSub* pEffectSub, int targetPos, ActorList* c1, ActorList *c2, Actor* buffCaster, Actor *buffOwner);

void SelectEffectTargetsFilter1(const int filter1[], BattleArea * pArea, EffectSub* pEffectSub, Actor* pCaster, Actor* pMainTarget, ActorList* c1, ActorList *c2, Actor* buffCaster, Actor *buffOwner, ActorList &actorList);
void SelectEffectTargetsFilter2(const int filter2[], int targetCnt, Actor* pCaster,ActorList &actorList);
void SelectEffectTargetsFilter3(const int filter3[], BattleArea * pArea, ActorList &actorList);

struct SelectActorDataValue
{
    enum EnumType
    {
        Hp = 0,
        Xp = 1,
    };
};

template<SelectActorDataValue::EnumType>
struct GetActorValue
{
    static int Get(Actor* pActor);
};
template<>
struct GetActorValue<SelectActorDataValue::EnumType::Hp>
{
    static inline int Get(Actor* pActor);
    /*{
        return pActor->actorDatas.hp;
    }*/
};
int GetActorValue<SelectActorDataValue::EnumType::Hp>::Get(Actor* pActor)
{
    return (pActor->actorDatas.hp * 1.0f / pActor->actorDatas.hpMax)* 10000;
}

//linux compiler doesnot work
//template<>
//static inline int GetActorValue<SelectActorDataValue::EnumType::Xp>::Get(Actor* pActor)
//{
//    return pActor->actorDatas.xp;
//};

template <>
struct GetActorValue<SelectActorDataValue::EnumType::Xp>
{
    static inline int Get(Actor* pActor)
    {
        return pActor->actorDatas.xp;
    }
};

struct GeneralOperator
{
    enum EnumType
    {
        Less = 0,
        Greater = 1,
    };
};
struct CompareValue
{
    template<GeneralOperator::EnumType comp>
    static bool Compare(int curValue, int LastValue);
};
template<>
inline bool CompareValue::Compare<GeneralOperator::EnumType::Less>(int curValue, int LastValue)
{
    return curValue < LastValue;
}
template<>
inline bool CompareValue::Compare<GeneralOperator::EnumType::Greater>(int curValue, int LastValue)
{
    return curValue > LastValue;
}

template<SelectActorDataValue::EnumType dataTy,GeneralOperator::EnumType opTy>
Actor* RemoveTopMostActors(ActorList &filterList)
{
    int topMostValue = 0;
    int topMostCnt = 0;
    Actor *filterActor = nullptr;
    for (auto & actor : filterList.list)
    {
        if (actor != nullptr)
        {
            if (filterActor == nullptr)
            {
                filterActor = actor;
                topMostValue = GetActorValue<dataTy>::Get(actor);
                topMostCnt = 1;
            }
            else
            {
                auto curValue = GetActorValue<dataTy>::Get(actor);
                if (CompareValue::Compare<opTy>(curValue, topMostValue))
                {
                    topMostCnt = 1;
                    topMostValue = curValue;
                }
                else if (curValue == topMostValue)
                {
                    topMostCnt++;
                }
            }
        }
    }
    if (topMostCnt == 0)
    {
        return nullptr;
    }
    int selectIndex = 0;
    if (topMostCnt > 1)
    {
        if (filterActor != nullptr)
        {
            selectIndex = filterActor->battleField->random.RandForWidthFromZero(topMostCnt);
        }
    }
    int curIndex = 0;
    for (int i = 0; i < (int)(ActorList::Size); i++)
    {
        auto actor = filterList.list[i];
        if (actor != nullptr)
        {
            auto curValue = GetActorValue<dataTy>::Get(actor);
            if (curValue == topMostValue)
            {
                if (curIndex++ == selectIndex)
                {
                    filterList.list[i] = nullptr;
                    return actor;
                }
            }
        }
    }
    return nullptr;
}
template<SelectActorDataValue::EnumType dataTy, GeneralOperator::EnumType opTy, GeneralOperator::EnumType opTyInv>
void SelectTopMostActors(ActorList & filterList, int requiredCnt,ActorList & actorList)
{
    auto filterActorSize = filterList.GetSize();
    if (requiredCnt > 0 && requiredCnt <= 6)
    {
        if (requiredCnt > filterActorSize)
        {
            requiredCnt = filterActorSize;
        }
        auto excessCnt = filterActorSize - requiredCnt;
        if (excessCnt > 0 && excessCnt < requiredCnt)
        {
            for (int i = 0; i < excessCnt; i++)
            {
                RemoveTopMostActors<dataTy, opTyInv>(filterList);
            }
            actorList = filterList;
        }
        else
        {
            for (int i = 0; i < requiredCnt; i++)
            {
                auto pActor = RemoveTopMostActors<dataTy, opTy>(filterList);
                if (pActor != nullptr)
                {
                    actorList.AppendUnique(pActor);
                }
            }
        }
    }
}
#endif

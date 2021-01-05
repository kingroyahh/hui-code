#ifndef _GRAV_ACTOR_H_
#define _GRAV_ACTOR_H_
#include <map>
#include "ActorAttrs.h"
#include "NewActorAttrs.h"
#include "ActorDatas.h"
#include "BattleParams.pb.h"
#include "Type.h"
#include "Enum.h"
#include "BattleAnalysis.pb.h"
#include "ActorMeta.h"
using namespace battle;
class Actor
{
public:
    int actorId;//逻辑计算的ID
    int templateId;//actor表中的ID
    int heroId;//服务器用的ID
    const ActorMeta* metaData;
    string name;
    int fashionId;
    int level;
    int awake;
    int advance;
    int halo;
    
    int areaIndex;
    int posIndex;
    int actionCount;
    int activeRound;
    ActorActionState actionState;
    
    bool IsImmuneThisTime;

    BattleFieldPtr battleField;
    int battleType; 

    NewActorAttrs actorAttrs;
    //ActorAttrs actorAttrs;
    ActorDatas actorDatas;

    ActorModifierPtr actorMod;
    ActorSkillPtr actorSkill;
    pb::BattleAnalysisActorInfo *analysisActorInfo;
    
    void InitActor(const pb::InitActorInfo &actorInfo);
    bool IsAlive();
    void ReAlive();
    bool ChangeXp(int param);
    void ChangeHp(double param);
    ENationality GetNationality();
    double GetAttrByType(ActorAttrType dType);
    
    /*void AddCacheSkill(int skillId, int skillLv);
    void CheckCacheSkill();*/
    
    //void AddCacheAffix(int affixId);
    //void ApplyCacheAffix();
    void RemoveCacheAffix();
    
    bool AllowSkill();
    bool AllowComAtk();
    void CastSkillByType(SkillType sType, int targePos);
    
    void NotifyDeadEvtRaised();
    bool DeadEvtRaised();
    void CheckDeleteMods();
    Actor(BattleFieldPtr bf);
    virtual ~Actor();
private:
    bool deadEvtRaised;
    //SkillData cacheSkill[MAX_CACHE_SKILL_DATA];
    //AffixData cacheAffix[MAX_CACHE_AFFIX_DATA];
    //void ClearCacheAffix();
};


#endif

#ifndef _GRAV_BATTLE_AREA_H_
#define _GRAV_BATTLE_AREA_H_
#include "Type.h"
#include "BattleModeMeta.h"
using namespace battle;
class BattleArea
{
public:
    BattleFieldPtr bf;
    int playerId;
    int areaIndex;
    int curPos;
    float damageAll;
    RoundAttrInfo actorAffixs[BATTLE_POS_MAX];
    
    ActorPtr actors[BATTLE_POS_MAX];
    bool AddActor(ActorPtr actor);
    bool HasAlive();
    int AliveCount();
    bool CheckRoundOver();
    void OnBuffTrigger(int triggerArea);
    void AddRoundAffix(int affixId,int power);
    void ClearRoundAffix();
    void OnBuffCount();
    void RoundXpRecovery();
    bool ActorIsExist(int skillActorID);

    ActorPtr GetActorById(int tempId);
    ActorPtr GetActorByPos(int posIndex);
    ActorPtr GetActorByCAR(int column, int row);

    ActorPtr GetLowestHpTarget();
    ActorPtr GetHighestXpTarget();
    ActorPtr GetLowestXpTarget();

    ActorPtr GetNewMainTarget(int casterPos,int targetPos);
    //以下这些均是历史遗留代码，现在基本上不用了
    void GetRandomTargets(int maxSize, ActorPtr *targetList);
    void GetOtherFirstTargets(int maxSize, ActorPtr *targetList,int myArea,int posIndex);
    void GetAllTargets(ActorPtr *targetList);
    void GetCrossTargets(int posIndex, ActorPtr *targetList);
    void GetOneColumnTargets(int posIndex, ActorPtr *targetList);
    void GetRowTargets(int row, ActorPtr *targetList);
    void GetSelfRlTargets(int posIndex, ActorPtr *targetList);
    void GetTargetAndRlTargets(int posIndex, ActorPtr *targetList);
    void GetDeadTargets(ActorPtr *targetList);
    void GetSelfRow(int posIndex, ActorPtr *targetList);
    void GetSameSide(int casterPos,ActorPtr *targetList);
    bool FrontRowIsHaveAlive();
    void ModRoundCount();
    void ResetAction();
    void CheckDeleteMods();
    BattleArea(int areaIndex, BattleFieldPtr bfIn);
    virtual ~BattleArea();
private:
    ActorPtr GetNextMainTarget(int casterPos,int targetPos, ESelectTarget etarget);
};

#endif

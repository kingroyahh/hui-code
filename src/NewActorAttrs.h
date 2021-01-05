#ifndef _GRAV_ACTOR_NEWATTRS_H_
#define _GRAV_ACTOR_NEWATTRS_H_
#include "Type.h"
#include "BattleParams.pb.h"

struct NewAttr
{
    ActorAttrType attrID;
    int nextInstID;
    bool dirty;
    double resultValue;
    std::vector<std::pair<int, double>> valueVec;
    int AddMod(double modValue);
    void RemoveMod(int toIndex);
    double GetRealValue();
    ~NewAttr()
    {

    }
};

typedef std::vector<std::pair<ActorAttrType, double>> NewAttrSnapShot;
class NewActorAttrs
{
public:
    typedef NewAttr* NewAttrPtr;
    void InitAttr(const pb::AttrInfo & attrInfo);
    double GetAttrByType(ActorAttrType attrType);
    int AddAttrMod(ActorAttrType aType, double modValue);
    void RemoveAttrMod(ActorAttrType aType, int modIndex);
    void CopyToActorData(int attrIDs[MAX_BUFF_CACHE_ATTR_COUNT], NewAttrSnapShot & snapShot);
    void SetSnapShot(NewAttrSnapShot * pSnapShot);
    bool IsComboAffect(); //is the commbo affect the attr
    NewActorAttrs(BattleFieldPtr bf);
    virtual ~NewActorAttrs();
private:
    NewAttrPtr GetAttr(int aType);
    NewAttrPtr attrArray[ActorAttrCount];
    BattleFieldPtr battleField;
    NewAttrSnapShot * snapShot;
};
#endif

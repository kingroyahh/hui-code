#ifndef _GRAV_ACTOR_ATTRS_H_
#define _GRAV_ACTOR_ATTRS_H_
#include <map>
#include "Type.h"
#include "BattleParams.pb.h"

using namespace std;

class ActorAttrData
{
public:
    void CopyToActorData(int attrIDs[MAX_BUFF_CACHE_ATTR_COUNT], ActorAttrData & attrData);
    void UpdateFromActorData(ActorAttrData & attrData);
    typedef map <ActorAttrType, AttrPtr> InnerAttrData;
    InnerAttrData attrMap;
public:
    ActorAttrData();
    virtual~ActorAttrData();
    void Clear();
};
class ActorAttrs
{
public:
    ActorAttrData attrData;
    void InitAttr(const pb::AttrInfo & attrInfo);
    double GetAttrByType(ActorAttrType attrType);
    int AddAttrMod(ActorAttrType aType, double modValue);
    bool RemoveAttrMod(ActorAttrType aType, int modIndex);
    ActorAttrs(BattleFieldPtr bf);
    virtual ~ActorAttrs();
private:
    AttrPtr GetAttr(ActorAttrType aType);
    BattleFieldPtr battleField;
};
class Attr
{
public:
    double baseValue;
    int modIndex;
    map<int, ModSubPtr> mods;

    int AddMod(double modValue);
    bool RemoveMod(int toIndex);
    double GetRealValue();
    Attr* Clone();
    Attr();
    virtual ~Attr();
    void Clear();
private:

};



class ModSub
{
public:

    int index;
    double modValue;

    ModSub();
    virtual ~ModSub();

private:

};

#endif
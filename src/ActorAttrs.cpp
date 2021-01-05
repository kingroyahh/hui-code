#include <map>
#include "ActorAttrs.h"
#include "CombatAttrMeta.h"
#include "BattleParams.pb.h"
#include "Enum.h"
#include "Type.h"
#include "Log.h"
#include "BattleField.h"
#include <unordered_map>
using namespace std;

unordered_map<int, Attr*> umap;
ActorAttrs::ActorAttrs(BattleFieldPtr bf)
{
    battleField = bf;
    TRACE("ActorAttrs::ActorAttrs");
    int attrCount = 0;
    map<int, CombatAttrMeta*>::iterator it;
    for (it = CombatAttrMetaSet::instance()->set.begin(); it != CombatAttrMetaSet::instance()->set.end(); it++)
    {
        if (it->second != NULL_PTR)
        {
            AttrPtr attr = new Attr();
            if (battleField->InitCombatAttr)
            {
                attr->baseValue = it->second->atValue;
            }
            else
            {
                attr->baseValue = 0;
            }
            attrData.attrMap.insert(pair<ActorAttrType, AttrPtr>((ActorAttrType)it->second->id, attr));
            attrCount++;
        }
    }
}

ActorAttrs::~ActorAttrs()
{
}

void ActorAttrs::InitAttr(const pb::AttrInfo & attrInfo)
{
    TRACE("ActorAttrs::InitAttr");
    if (attrInfo.attr_id() != 0)
    {
        AttrPtr attrPtr = GetAttr((ActorAttrType)attrInfo.attr_id());
        if (attrPtr != NULL_PTR)
        {
            attrPtr->AddMod(attrInfo.attr_value());
        }
    }
}

double ActorAttrs::GetAttrByType(ActorAttrType attrType)
{
    map<ActorAttrType, AttrPtr>::iterator attrIt;
    attrIt = attrData.attrMap.find(attrType);
    if (attrIt == attrData.attrMap.end())
    {
        error("not have this attribute: %d\n", attrType);
        return 0;
    }

    return attrIt->second->GetRealValue();
}

int ActorAttrs::AddAttrMod(ActorAttrType aType, double modValue)
{
    AttrPtr atPtr = GetAttr(aType);
    if (atPtr != NULL)
    {
        return atPtr->AddMod(modValue);
    }
    return -1;
}

AttrPtr ActorAttrs::GetAttr(ActorAttrType aType)
{
    map<ActorAttrType, AttrPtr>::iterator attrIt;
    attrIt = attrData.attrMap.find(aType);
    if (attrIt == attrData.attrMap.end())
    {
        error("not have this attribute: %d\n", aType);
        return NULL;
    }
    return attrIt->second;
}

bool ActorAttrs::RemoveAttrMod(ActorAttrType aType, int modIndex)
{
    TRACE("ActorAttrs::RemoveAttrMod ActorAttrType[%d] modIndex[%d]", aType, modIndex);
    map<ActorAttrType, AttrPtr>::iterator attrIt;
    attrIt = attrData.attrMap.find(aType);
    if (attrIt == attrData.attrMap.end())
    {
        error("not have this attribute: %d\n", aType);
        return false;
    }
    return attrIt->second->RemoveMod(modIndex);
}


Attr::Attr()
{
    baseValue = 0;
    modIndex = 0;
    mods.clear();
}

Attr* Attr::Clone()
{
    TRACE("Attr::Clone");
    Attr* ret = new Attr();
    ret->baseValue = baseValue;
    ret->modIndex = modIndex;
    map<int, ModSubPtr>::iterator it = mods.begin();
    for (; it != mods.end();it++)
    {
        int key = it->first;
        ModSubPtr srcSub = it->second;
        ModSubPtr cloneMod = new ModSub;
        cloneMod->modValue = srcSub->modValue;
        cloneMod->index = srcSub->index;
        ret->mods[key] = cloneMod;
    }
    return ret;
}

Attr::~Attr()
{
    Clear();
}

double Attr::GetRealValue()
{
    double modValue = 0;
    map<int, ModSubPtr>::iterator modIt;
    for (modIt = mods.begin(); modIt != mods.end(); modIt++)
    {
        modValue += modIt->second->modValue;
    }
    return baseValue + modValue;
}

int Attr::AddMod(double modValue)
{
    TRACE("Attr::AddMod");
    ModSubPtr modPtr = new ModSub();
    modPtr->modValue = modValue;
    modIndex++;
    mods.insert(pair<int, ModSubPtr>(modIndex, modPtr));
    return modIndex;
}

bool Attr::RemoveMod(int toIndex)
{
    TRACE("Attr::RemoveMod toIndex[%d]", toIndex);
    map<int, ModSubPtr>::iterator modIt;
    modIt = mods.find(toIndex);
    if (modIt == mods.end())
    {
        return false;
    }
    delete modIt->second;
    mods.erase(modIt);
    return true;
}

void Attr::Clear()
{
    map<int, ModSubPtr>::iterator modIt;
    for (modIt = mods.begin(); modIt != mods.end(); modIt++)
    {
        delete modIt->second;
    }
    mods.clear();
}



ModSub::ModSub()
{
}

ModSub::~ModSub()
{
}

ActorAttrData::ActorAttrData()
{

}

ActorAttrData::~ActorAttrData()
{
    Clear();
}

void ActorAttrData::Clear()
{
    TRACE("ActorAttrData::Clear");
    InnerAttrData::iterator attrIt;
    for (attrIt = attrMap.begin(); attrIt != attrMap.end(); attrIt++)
    {
        delete attrIt->second;
    }
    attrMap.clear();
}

void ActorAttrData::CopyToActorData(int attrIDs[MAX_BUFF_CACHE_ATTR_COUNT], ActorAttrData & attrData)
{
    TRACE("ActorAttrData::CopyToActorData");
    attrData.Clear();
    if (attrIDs == NULL)
    {
        InnerAttrData::iterator it = attrMap.begin();
        for (; it != attrMap.end();++it)
        {
            attrData.attrMap[(ActorAttrType)it->first] = it->second->Clone();
        }
    }
    else
    {
        for (int i = 0; i < MAX_BUFF_CACHE_ATTR_COUNT; i++)
        {
            if (attrIDs[i] > 0)
            {
                InnerAttrData::iterator it = attrMap.find((ActorAttrType)attrIDs[i]);
                if (it == attrMap.end())
                {
                    error("not have this attribute: %d\n", attrIDs[i]);
                    continue;
                }
                attrData.attrMap[(ActorAttrType)attrIDs[i]] = it->second->Clone();
            }
        }
    }
}

void ActorAttrData::UpdateFromActorData(ActorAttrData & attrData)
{
    TRACE("ActorAttrData::UpdateFromActorData");
    InnerAttrData::iterator it = attrData.attrMap.begin();
    for (; it != attrData.attrMap.end(); ++it)
    {
        int key = it->first;
        AttrPtr newAttr = it->second;
        InnerAttrData::iterator updateIt = attrMap.find((ActorAttrType)key);
        if (updateIt == attrMap.end())
        {
            error("ActorAttrs::UpdateFromActorData,attrMap does not has key %d", key);
            continue;
        }
        delete updateIt->second;
        attrMap[(ActorAttrType)key] = newAttr->Clone();
    }
}

#include "NewActorAttrs.h"
#include "CombatAttrMeta.h"
#include "BattleField.h"
int NewAttr::AddMod(double modValue)
{
    ++nextInstID;
    dirty = true;
    valueVec.push_back({ nextInstID, modValue });
    return nextInstID;
}

void NewAttr::RemoveMod(int instID)
{
    for (auto it = valueVec.rbegin(); it != valueVec.rend(); ++it)
    {
        if (it->first == instID)
        {
            valueVec.erase(--it.base());
            break;
        }
    }
    dirty = true;
}

double NewAttr::GetRealValue()
{
    if (dirty)
    {
        resultValue = 0;
        for (auto pair : valueVec)
        {
            resultValue += pair.second;
        }
        dirty = false;
    }
    return resultValue;
}

void NewActorAttrs::InitAttr(const pb::AttrInfo & attrInfo)
{
    if (attrInfo.attr_id() != 0)
    {
        auto attr = GetAttr(attrInfo.attr_id());
        if (attr != nullptr)
        {
            attr->AddMod(attrInfo.attr_value());
        }
    }
}

NewActorAttrs::NewActorAttrs(BattleFieldPtr bf)
    :attrArray{}
    , snapShot(nullptr)
    , battleField{bf}
{
    std::map<int, CombatAttrMeta*>::iterator it;
    for (it = CombatAttrMetaSet::instance()->set.begin(); it != CombatAttrMetaSet::instance()->set.end(); it++)
    {
        if (it->second != NULL_PTR)
        {
            auto id = it->second->id;
            attrArray[id] = new NewAttr{};
            attrArray[id]->attrID = (ActorAttrType)id;
            if (battleField->InitCombatAttr)
            {
                attrArray[id]->AddMod(it->second->atValue);
            }
        }
    }
}

NewActorAttrs::~NewActorAttrs()
{
    for (auto newAttr : attrArray)
    {
        if (newAttr != nullptr)
        {
            delete newAttr;
        }
    }
}

double NewActorAttrs::GetAttrByType(ActorAttrType attrType)
{
    if (snapShot != nullptr)
    {
        for (auto pair : *snapShot)
        {
            if (pair.first == attrType)
            {
                return pair.second;
            }
        }
    }
    auto attr = GetAttr(attrType);
    if (attr != nullptr)
    {
        return attr->GetRealValue();
    }
    return 0;
}

int NewActorAttrs::AddAttrMod(ActorAttrType aType, double modValue)
{
    auto attr = GetAttr(aType);
    if (attr != nullptr)
    {
        return attr->AddMod(modValue);
    }
    return -1;
}

void NewActorAttrs::RemoveAttrMod(ActorAttrType aType, int modIndex)
{
    auto attr = GetAttr(aType);
    if (attr != nullptr)
    {
        return attr->RemoveMod(modIndex);
    }
}

NewActorAttrs::NewAttrPtr NewActorAttrs::GetAttr(int aType)
{
    if (aType >= 0 && aType < ActorAttrCount)
    {
        return attrArray[aType];
    }
    return nullptr;
}

void NewActorAttrs::CopyToActorData(int attrIDs[MAX_BUFF_CACHE_ATTR_COUNT], NewAttrSnapShot & snapShot)
{
    bool isDoubleHitAffect = false;
    for (int i = 0; i < MAX_BUFF_CACHE_ATTR_COUNT; i++)
    {
        
        auto attrID = attrIDs[i];
        if (attrID == -1)
        {
            isDoubleHitAffect = true;
            break;
        }
        if (attrID <= 0)
        {
            break;
        }
        auto attr = GetAttr(attrID);
        if (attr != nullptr)
        {
            snapShot.push_back({ (ActorAttrType)attrID, attr->GetRealValue() });
        }
    }
    snapShot.push_back({ ActorAttrType::DoubleHitAffect, 1});
    if(!isDoubleHitAffect)
    {
        snapShot.push_back({ ActorAttrType::DoubleHitRateForceZero, 1});
    }
}

void NewActorAttrs::SetSnapShot(NewAttrSnapShot * pSnapShot)
{
    snapShot = pSnapShot;
}

bool NewActorAttrs::IsComboAffect()
{
    return GetAttrByType(ActorAttrType::DoubleHitAffect) == 1;
}

#ifndef _GRAV_COMBAT_ATTR_META_H_
#define _GRAV_COMBAT_ATTR_META_H_

#ifdef GRAV_UNITY_PLUGIN
#define COMBAT_ATTR_KEY   "Combatattr/trunk.db"
#else
#define COMBAT_ATTR_KEY   "Combatattr.json"
#endif

#include "Enum.h"
#include "Type.h"
#include "MetaSet.h"
#include "Singleton.h"

#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

struct CombatAttrMeta
{
    int id;
    float atValue;
    
};

class CombatAttrMetaSet : public BaseMetaSet<CombatAttrMeta>, public Singleton < CombatAttrMetaSet >
{
protected:
    void AssignPath() override;
    CombatAttrMeta* ParseJObject(Value::Object &jobj) override;
};

#endif
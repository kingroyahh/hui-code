#ifndef _GRAV_SUBEFFECTS_META_H_
#define _GRAV_SUBEFFECTS_META_H_

#include "Enum.h"
#include "MetaSet.h"
#include "Type.h"
#include "Singleton.h"

#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

struct SubEffectsMeta
{
    int id;
    FuncType funcType;
    int DoRage;
    int DoEvent;
    double funcParams[MAX_DMG_PARAM];
};

class SubEffectsMetaSet : public BaseMetaSet<SubEffectsMeta>, public Singleton<SubEffectsMetaSet>
{
protected:
    void AssignPath() override;
    SubEffectsMeta* ParseJObject(Value::Object &jobj) override;
};

#endif

#ifndef _GRAV_AFFIX_META_H_
#define _GRAV_AFFIX_META_H_

#include "Enum.h"
#include "MetaSet.h"
#include "Type.h"
#include "Singleton.h"

#include "rapidjson/document.h"

using namespace rapidjson;

struct AttrMeta
{
    int attrId;
    float attrValue;
    int attrPower;
};

struct AffixMeta
{
    int id;
    AttrMeta attrList[MAX_AFFIX_ATTR];
};

class AffixMetaSet : public BaseMetaSet<AffixMeta>, public Singleton < AffixMetaSet >
{
protected:
    void AssignPath() override;
    AffixMeta* ParseJObject(Value::Object &jobj) override;
};

#endif
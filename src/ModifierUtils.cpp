#include "ModifierUtils.h"
#include "AffixMeta.h"
#include "Type.h"
#include "Actor.h"
#include "Log.h"
#include <list>

void ApplyAffix(ActorPtr actor, int affixId, int affixPower, TempAttrInfo & attrInfo)
{
    TRACE("ApplyAffix actor[%d] affixID[%d]", actor->actorId, affixId);
    if(affixId == 0)
    {
        return;
    }
    const AffixMeta *affixMeta = AffixMetaSet::instance()->Get(affixId);
    if (affixMeta == NULL)
    {
        error("affix not found : %d\n", affixId);
        return;
    }

    for (int i = 0; i < MAX_AFFIX_ATTR; ++i)
    {
        if (affixMeta->attrList[i].attrId != 0)
        {
            ActorAttrType attrType = (ActorAttrType)affixMeta->attrList[i].attrId;
            float attrValue = affixMeta->attrList[i].attrValue;
            if(affixMeta->attrList[i].attrPower == 1)
            {
                attrValue = attrValue * affixPower;
            }
            int modIndex = actor->actorAttrs.AddAttrMod(attrType, attrValue);
            attrInfo.push_back(make_pair(attrType, modIndex));
        }
    }
}

void RemoveAffix(ActorPtr actor, const TempAttrInfo & attrInfo)
{
    TRACE("RemoveAffix");
    TempAttrInfo::const_reverse_iterator it = attrInfo.rbegin();
    for (; it != attrInfo.rend(); ++it)
    {
        actor->actorAttrs.RemoveAttrMod((*it).first, (*it).second);
    }
}
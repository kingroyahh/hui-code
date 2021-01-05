#ifndef _GRAV_MODIFIER_UTILS_H_
#define _GRAV_MODIFIER_UTILS_H_
#include <map>
#include "Enum.h"
#include "Type.h"

using namespace std;
void ApplyAffix(ActorPtr actor, int affixId, int affixPower, TempAttrInfo & attrInfo);
void RemoveAffix(ActorPtr actor, const TempAttrInfo & attrInfo);
#endif // !_GRAV_MODIFIER_UTILS_H_

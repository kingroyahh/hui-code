#ifndef _GRAV_ACTOR_META_H_
#define _GRAV_ACTOR_META_H_

#include "Enum.h"
#include "MetaSet.h"
#include "Type.h"
#include "Singleton.h"

#include "rapidjson/document.h"

using namespace rapidjson;

enum ENationality
{
    EN_None = 0,
    EN_Player = 1,
    EN_Nation1 = 2,
    EN_Nation2 = 3,
    EN_Nation3 = 4,
};
struct ActorMeta
{
    int id;
    ENationality Nationality;
    int SkillActorId;
};

class ActorMetaSet : public BaseMetaSet<ActorMeta>, public Singleton < ActorMetaSet >
{
protected:
    void AssignPath() override;
    ActorMeta* ParseJObject(Value::Object &jobj) override;
};

#endif
#ifndef _GRAV_VALUE_META_H_
#define _GRAV_VALUE_META_H_

#include "Enum.h"
#include "MetaSet.h"
#include "Type.h"
#include "Singleton.h"

#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

struct ValueMeta
{
    int id;
    float floatValue;
    float *FloatValueArry;
    int NumValue;
    int *NumValueArry;

    virtual ~ValueMeta()
    {
        if (FloatValueArry != NULL)
        {
            delete []FloatValueArry;
            FloatValueArry = NULL;
        }
        if (NumValueArry != NULL)
        {
            delete[]NumValueArry;
            NumValueArry = NULL;
        }
    }
};

class ValueMetaSet : public BaseMetaSet<ValueMeta>, public Singleton<ValueMetaSet>
{
protected:
    void AssignPath() override;
    ValueMeta* ParseJObject(Value::Object &jobj) override;
};
#endif

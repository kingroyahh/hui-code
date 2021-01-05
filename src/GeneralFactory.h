#ifndef _GRAV_GENERAL_FACTORY_H_
#define _GRAV_GENERAL_FACTORY_H_
#include <list>
using namespace std;
template<typename KeyType,typename InstBaseType>
class GeneralPoolFactoryMgr
{
public:
    GeneralPoolFactoryMgr(){}
    virtual ~GeneralPoolFactoryMgr()
    {
        OnDestroy();
    }
    
    template<typename InstType>
    void RegisterFactory(KeyType keyType)
    {
        typename FactoryTypeMap::iterator it = factoyTypeMap.find(keyType);
        if (it == factoyTypeMap.end())
        {
            factoyTypeMap[keyType] = new PoolFactoryBase<InstType>();
        }
    }
    InstBaseType* CreateInst(KeyType keyType)
    {
        typename FactoryTypeMap::iterator it = factoyTypeMap.find(keyType);
        if (it != factoyTypeMap.end())
        {
            return it->second->CreateInst();
        }
        return NULL;
    }
    void DestroyInst(KeyType keyType,InstBaseType* pInst)
    {
        typename FactoryTypeMap::iterator it = factoyTypeMap.find(keyType);
        if (it != factoyTypeMap.end())
        {
            it->second->DestroyInst(pInst);
        }
    }
private:
    class IFactoryBase
    {
    public:
        virtual ~IFactoryBase(){}
        virtual InstBaseType* CreateInst() = 0;
        virtual void DestroyInst(InstBaseType* pInst)
        {
            delete pInst;
        }
        virtual void OnDestroy(){}
    };

    template<typename InstType>
    class PoolFactoryBase : public IFactoryBase
    {
    public:
        PoolFactoryBase(){}
        virtual InstBaseType* CreateInst()
        {
            if (instPool.size() > 0)
            {
                InstBaseType* pInst = instPool.back();
                instPool.pop_back();
                return pInst;
            }
            return new InstType;
        }
        virtual void DestroyInst(InstBaseType* pInst)
        {
            instPool.push_back(pInst);
        }
        virtual void OnDestroy()
        {
            typename InstPoolType::iterator it;
            for (it = instPool.begin(); it != instPool.end();++it)
            {
                delete *it;
            }
            instPool.clear();
        }
    private: 
        typedef typename std::list<InstBaseType*> InstPoolType;
        InstPoolType instPool;
    };

    void OnDestroy()
    {
        typename FactoryTypeMap::iterator it;
        for (it = factoyTypeMap.begin(); it != factoyTypeMap.end(); ++it)
        {
            it->second->OnDestroy();
            delete it->second;
        }
        factoyTypeMap.clear();
    }
    typedef typename std::map<KeyType, IFactoryBase*> FactoryTypeMap;
    FactoryTypeMap factoyTypeMap;
};

#endif /* _GRAV_GENERAL_FACTORY_H_ */

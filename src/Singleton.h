#ifndef _GRAV_SINGLETON_H_
#define _GRAV_SINGLETON_H_

#include "Define.h"

template<typename T>
class Singleton
{
private:
    Singleton(Singleton const&) {};
    Singleton& operator=(Singleton const&) {};

protected:
    static T* s_instance;

    Singleton() { s_instance = static_cast<T*>(this); };
    ~Singleton() {};

public:
    static T* instance();
    static void destroy();
};

template<typename T>
T* Singleton<T>::s_instance = NULL_PTR;

template<typename T>
T* Singleton<T>::instance()
{
    if (s_instance == NULL_PTR)
    {
        Singleton<T>::s_instance = new T();
    }
    return s_instance;
}

template<typename T>
void Singleton<T>::destroy()
{
    delete Singleton<T>::s_instance;
    Singleton<T>::s_instance = NULL_PTR;
}

#endif
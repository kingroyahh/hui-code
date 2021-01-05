#pragma once
#include "template.h"


template <class T>
struct A::B
{
    void Func(){}
};
template<class T>
struct A::B<T*>
{
    void Func(){}
};
template<> struct A::B<int*>
{
    void Func(){}
};
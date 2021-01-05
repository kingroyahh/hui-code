#pragma once


struct A
{
    template <class T> struct B;
    template <class T> struct B<T*>;
    //template<> struct B<int*>;
};

#include "templateImpl.hpp"

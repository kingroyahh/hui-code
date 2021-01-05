#ifndef _GRAV_ENUM_MACRO_H
#define _GRAV_ENUM_MACRO_H
#define DECLARE_ENUM(E) \
struct E \
{ \
public: \
E(int value = 0) : _value((__Enum)value) { \
} \
E& operator=(int value) { \
this->_value = (__Enum)value; \
return *this; \
} \
operator int() const { \
return this->_value; \
} \
bool operator ==(E &a) const { \
return _value == a._value;\
}\
\
enum __Enum {

#define END_ENUM() \
}; \
\
public: \
__Enum _value; \
};

#endif

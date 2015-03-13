#ifndef LIGHT_H
#define LIGHT_H

#include "property.h"
#include "linalg.h"

class Light {
 public:
    Color* color;
    virtual bool isPointLight() {return false;}
    virtual bool isDirectLight() {return false;}
};

class Point_Light : public Light {
 public:
    Point_Light(Color* c, Point* p, int f);
    Point* pos;
    int falloff;
    bool isPointLight() {return true;}
    bool isDirectLight() {return false;}
};

class Direct_Light : public Light {
 public:
    Direct_Light(Color* c, Vector* v);
    Vector* dir;
    bool isPointLight() {return false;}
    bool isDirectLight() {return true;}
};


#endif

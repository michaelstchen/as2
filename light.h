#ifndef LIGHT_H
#define LIGHT_H

#include "property.h"
#include "linalg.h"

//*************************************************
// Contains classes for the different lights
//
// The Classes:
//
//     --Light: superclass
//     --Point_Light: a point light with a given falloff
//                    defaults to 0 falloff.
//     --Direct_Light: a directional light.
//     --Ambient_Light: an ambient light.
//
//************************************************


class Light {
 public:
    Point* pos;
    Vector* dir;
    Color* color;
    virtual bool isPointLight() {return false;}
    virtual bool isDirectLight() {return false;}
};

class Point_Light : public Light {
 public:
    Point_Light(Color* c, Point* p, int f);
    int falloff;
    bool isPointLight() {return true;}
    bool isDirectLight() {return false;}
};

class Direct_Light : public Light {
 public:
    Direct_Light(Color* c, Vector* v);
    bool isPointLight() {return false;}
    bool isDirectLight() {return true;}
};

class Ambient_Light : public Light {
 public:
    Ambient_Light(Color* c);
};


#endif

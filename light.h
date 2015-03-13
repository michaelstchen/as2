#ifndef LIGHT_H
#define LIGHT_H

#include "geomobj.h"
#include "property.h"
#include "linalg.h"

class Light {
 public:
    Color color;
    virtual bool isPointLight();
    virtual bool isDirectLight();
};

class Point_Light : public Light {
 public:
    Point pos;
    int falloff;
    bool isPointLight() {return true;}
    bool isDirectLight() {return false;}
};

class Direct_Light : public Light {
 public:
    Vector direction;
    bool isPointLight() {return false;}
    bool isDirectLight() {return true;}
};


#endif

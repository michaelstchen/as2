#ifndef GEOMOBJ_H
#define GEOMOBJ_H

#include "linalg.h"

class Ray {
 public:
    Point p0;
    Vector dir;
    float t_min; float t_max;
};

class Shape {
 public:
    Matrix transform;
    Matrix normT;
    virtual bool intersect(Ray r, Point p) {return false;}
};

class Sphere : public Shape {
 public:
    
};

class Triangle : public Shape {
 public:
    
};

#endif

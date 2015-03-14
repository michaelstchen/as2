#ifndef GEOMOBJ_H
#define GEOMOBJ_H

#include <cstddef>
#include "linalg.h"
#include "property.h"
#include "raytracer.h"
#include "linalg.h"

class World;

class Shape {
 public:
    Shape(World* w, Matrix* t);
    World* world;
    Matrix* transform;
    Matrix* normT;
    virtual bool intersect(Ray* r, Point* p) {return false;}
    virtual Normal* getNormal(Point* p) {return NULL;}
    virtual Color* calcBRDF(Point* p) {return NULL;}
};

class Sphere : public Shape {
 public:
    Sphere(Point* c, float r, World* w, Matrix* t);
    Point* center;
    float radius;
    Point* intersect(Ray* r);
    Normal* getNormal(Point* p);
    Color* calcBRDF(Point* p);
};

class Triangle : public Shape {
 public:
    
};

#endif

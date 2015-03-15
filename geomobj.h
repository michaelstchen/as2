#ifndef GEOMOBJ_H
#define GEOMOBJ_H

#include <cstddef>
#include "linalg.h"
#include "property.h"
#include "ray.h"

class World;

class Shape {
 public:
    Shape(World* w, Matrix* t);
    World* world;
    Matrix* transform;
    Matrix* normT;
    virtual float intersect(Ray* r) {return -1.0;}
    virtual Vector* getNormal(Point* p) {return NULL;}
    virtual Color* calcBRDF(Ray*r, Point* p) {return NULL;}
};

class Sphere : public Shape {
 public:
    Sphere(Point* c, float r, World* w, Matrix* t);
    Point* center;
    float radius;
    float intersect(Ray* r);
    Vector* getNormal(Point* p);
    Color* calcBRDF(Ray* r, Point* p);
};

class Triangle : public Shape {
 public:
    
};

#endif

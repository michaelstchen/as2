#ifndef GEOMOBJ_H
#define GEOMOBJ_H

#include <cstddef>
#include "linalg.h"
#include "property.h"
#include "raytracer.h"

class World;

class Shape {
 public:
    Shape(World* w, Matrix* t);
    World* world;
    Matrix* transform;
    Matrix* normT;
    virtual bool intersect(Ray* r, Point* p);
    virtual Normal* getNormal(Point* p);
    virtual Color* calcBRDF(Point* p);
};

class Sphere : public Shape {
 public:
    Sphere(Point* c, float r, World* w, Matrix* t);
    Point* center;
    float radius;
    bool intersect(Ray* r, Point* p);
    Normal* getNormal(Point* p);
    Color* calcBRDF(Point* p);
};

class Triangle : public Shape {
 public:
    
};

#endif

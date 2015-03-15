#ifndef GEOMOBJ_H
#define GEOMOBJ_H

#include <cstddef>
#include "linalg.h"
#include "property.h"
#include "ray.h"

class World;

class Shape {
 public:
    Shape(World* w, Matrix* t, Material* m);
    World* world;
    Matrix* transform;
    Matrix* normT;
    Material* material;
    virtual float intersect(Ray* r) {return -1.0;}
    virtual Vector* getNormal(Point* p) {return NULL;}
    Color* calcBRDF(Ray* ray, Point* p);
};

class Sphere : public Shape {
 public:
    Sphere(Point* c, float r, World* w, Matrix* t, Material* m);
    Point* center;
    float radius;
    float intersect(Ray* r);
    Vector* getNormal(Point* p);
};

class Triangle : public Shape {
 public:
    
};

#endif

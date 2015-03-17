#ifndef GEOMOBJ_H
#define GEOMOBJ_H

#include <cstddef>
#include "linalg.h"
#include "light.h"
#include "property.h"
#include "ray.h"

//*************************************************
// Contains classes for the shape primitives
//
// The Classes:
//
//     --Shape: superclass. member function
//              for calculating intersection.
//     --Sphere: stores radius and center
//     --Triangle: stores three vertices.
//
//************************************************

class World;

class Shape {
 public:
    Shape(World* w, Matrix* t, Material* m);
    /* the world I belong to. */
    World* world;
    /* transformation for my tangent vectors. */
    Matrix* transform;
    /* transformation for my normal vectors. */
    Matrix* normT;
    /* my material properties (i.e. color) */
    Material* material;

    /* calculates intersection with ray R returning the
     * the ray's t parameter. */
    virtual float intersect(Ray* r) {return -1.0;}
    /* returns normal vector for point on shape.
     * assumes point is located on shape. */
    virtual Vector* getNormal(Point* p) {return NULL;}
    /* calculates the BRDF value at a point on this shape 
     * given the incident ray. */
    Color* calcBRDF(Ray* ray, Point* p);
    /* returns true if the point P is in the shadow of an
     * object with respect to a light L */
    bool inShadow(Point* p, Light* l);
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

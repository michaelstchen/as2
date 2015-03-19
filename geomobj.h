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
    /* the inverse of my transformation matrix */
    Matrix* t_inverse;
    /* my material properties (i.e. color) */
    Material* material;

    /* calculates intersection with ray R returning the
     * the ray's t parameter. */
    virtual float intersect(Ray* r, Point** i_obj) {return -1.0;}
    /* returns normal vector for point on shape.
     * assumes point is located on shape. */
    virtual Vector* getNormal(Point* p) {return NULL;}
    /* calculates the BRDF value at a point P on this shape given
     * the incident vector V, and the normal N (assumend normalized. */
    Color* calcBRDF(Vector* v, Vector* n, Point* p);
    /* returns true if the point P is in the shadow of an
     * object with respect to a light L */
    bool inShadow(Point* p, Light* l);
};

class Sphere : public Shape {
 public:
    Sphere(Point* c, float r, World* w, Matrix* t, Material* m);
    Point* center;
    float radius;
    float intersect(Ray* r, Point** i_obj);
    Vector* getNormal(Point* p);
};

class Triangle : public Shape {
 public:
    Triangle(Point* p0, Point* p1, Point* p2,
             World* w, Matrix* t, Material* m);
    Triangle(Point* p0, Point* p1, Point* p2,
	     Vector* v0, Vector* v1, Vector* v2,
             World* w, Matrix* t, Material* m);
    Point* pa; Point* pb; Point* pc;
    Vector* na; Vector* nb; Vector* nc;
    bool hasNormals;
    float intersect(Ray* r, Point** i_obj);
    Vector* getNormal(Point* p);
};

#endif

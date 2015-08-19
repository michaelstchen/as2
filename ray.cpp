//***************************************************
// 
// Author: Michael Stephen Chen
//
// Description:
//    Definitions for the different types of rays used in
//    our raytracer.
//
//***************************************************



#include "raytracer.h"
#include "ray.h"
#include <cstddef>
#include <cmath>
#include <stdio.h>

Ray::Ray(Point* p, Vector* d) {
    p0 = p;
    dir = d;
}

Point* Ray::findPoint(float t) {
    if (t < t_min || t > t_max) {
        return NULL;
    }
    Vector* td = mult(dir, t);
    Point* pt = add(p0, td);
    delete td;

    return pt;
}

/* Rays with their origin at the viewing window. Used to determine
 * what to draw on screen (as determined intersecting objects in
 * the scene.
 */
EyeRay::EyeRay(Point* p, Vector* d) : Ray(p, d) {
    t_min = 0.00001;
    t_max = INFINITY;
}

/* Origin at intersection of ray and scene object. Used to draw
 * reflections.
 */
ReflectRay::ReflectRay(Point* p, Vector* d) : Ray(p, d) {
    t_min = 0.00001;
    t_max = INFINITY;
}

/* Origin at interesection of ray and scene object. Used to draw
 * shadows.
 */
ShadowRay::ShadowRay(Point* p, Vector* d, float max) : Ray(p, d) {
    t_min = 0.00001;
    t_max = max;
}


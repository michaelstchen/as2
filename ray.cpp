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

EyeRay::EyeRay(Point* p, Vector* d) : Ray(p, d) {
    t_min = 1;
    t_max = INFINITY;
}

ShadowRay::ShadowRay(Point* p, Vector* d) : Ray(p, d) {
    t_min = 0.00001;
    t_max = 1.0;
}


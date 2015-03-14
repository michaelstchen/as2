#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "linalg.h"
#include <cstddef>

class Ray {
 public:
    Ray(Point* p, Vector* d);
    Point* p0;
    Vector* dir;
    float t_min; float t_max;
    Point* findPoint(float t);
};

class EyeRay : public Ray {
 public:
    EyeRay(float min, float max, Point* p, Vector* d);
};


#endif

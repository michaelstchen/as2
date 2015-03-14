#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "linalg.h"

class Ray {
 public:
    Ray(Point* p0, Vector* dir);
    Point* p0;
    Vector* dir;
    float t_min; float t_max;
};


#endif

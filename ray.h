#ifndef RAY_H
#define RAY_H

#include "linalg.h"
#include <cstddef>

//*************************************************
// Contains various ray classes. They mainly differ
// in their limits for the parameter t.
//
// The Classes:
//
//     --Ray: the general Ray, serves as a superclass
//     --EyeRay: your typical eye-ray with 1<=t<INFINITY
//     --ShadowRay: helps in determining shadows
//                  epsilon<=t<INFINITY.
//
//************************************************


class Ray {
 public:
    Ray(Point* p, Vector* d);
    /* initial statrting point.*/
    Point* p0;
    /* the ray's direction.*/
    Vector* dir;
    /* the min and max values for the parameter t.*/
    float t_min; float t_max;
    /* p(t) = p0 + t*dir */
    Point* findPoint(float t);
};

class EyeRay : public Ray {
 public:
    EyeRay(Point* p, Vector* d);
};

/*
class ShadowRay: public Ray {
 public:
    ShadowRay(Point* p, Vector* d);
}
*/

#endif

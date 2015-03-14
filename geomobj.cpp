#include "geomobj.h"
#include "ray.h"
#include "linalg.h"
#include <cmath>
#include <cstddef>
#include <stdio.h>

Shape::Shape(World* w, Matrix* t) {
    world = w;
    transform = t;
    Matrix* t_invert = t->invert();
    normT = t_invert->transpose();
    delete t_invert;
}

Sphere::Sphere(Point* c, float r, World* w, Matrix* t) : Shape(w, t) {
    center = c;
    radius = r;
}

/* the intersection of a ray and sphere is given by
 *     (d.d)t^2 + 2(d.(e-c))t + (e-c).(e-c) - r^2 = 0
 * where the ray is parameterized as p(t) = e + td
 * and (c = sphere's center) and (r = sphere's radius). 
 * This function returns NULL if there is no intersection. */
Point* Sphere::intersect(Ray* r) {
    (r->dir)->normalize();
    float d_d = dot(r->dir, r->dir);
    Vector* e_minus_c = newVector(center, r->p0);
    float d_ec = dot(r->dir, e_minus_c);
    float ec_ec = dot(e_minus_c, e_minus_c);

    delete e_minus_c;
  
    float discriminant = d_ec*d_ec - d_d*(ec_ec - radius * radius);

    if (discriminant < 0.0) {
        return NULL;
    }

    float t0 = (-d_ec + sqrt(discriminant)) / d_d;
    float t1 = (-d_ec - sqrt(discriminant)) / d_d;

    float t = fmin(t0, t1);

    return r->findPoint(t);
    
}

/* returns the normal vector at point P assumin P lies on the sphere */
Vector* Sphere::getNormal(Point* p) {
    return newVector(p, center);
}

Color* Sphere::calcBRDF(Ray* r, Point* p) {
    return new Color(1.0, 0.0, 0.0);
}

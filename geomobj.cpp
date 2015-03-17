#include "geomobj.h"
#include "ray.h"
#include "linalg.h"
#include "light.h"
#include "raytracer.h"
#include <vector>
#include <cmath>
#include <cstddef>
#include <stdio.h>

using namespace std;

Shape::Shape(World* w, Matrix* t, Material* m) {
    world = w;
    transform = t;
    material = m;
}

void Shape::calcBRDF(Ray* ray, Point* p, Color* c) {

    Vector* n = getNormal(p);
    Vector* v = mult(ray->dir, 1.0);
    v->normalize();
    n->normalize();

    vector<Light*>::iterator it = world->lightIter();
    for (it; it != world->lightIterEnd(); ++it) {

        if (inShadow(p, *it)) continue;

        Vector* l;
        if ((**it).isPointLight()) {
            l = newVector((**it).pos, p);
        } else if ((**it).isDirectLight()) {
            l = mult((**it).dir, 1.0);
        } else {
            Color* amb = new Color(1.0,1.0,1.0);
            amb->mult(material->ka);
            amb->mult((**it).color);
            c->add(amb);
            delete amb; continue;
        }

        l->normalize();

        Vector* r_temp = mult(n, 2.0 * dot(l, n));
        Vector* r = sub(r_temp, l);
        delete r_temp;

        float l_dot_n = dot(l, n);
        float r_dot_v = dot(r, v);
        delete l; delete r;

        Color* diff = new Color(1.0,1.0,1.0);
        Color* spec = new Color(1.0,1.0,1.0);

        diff->mult(fmax(l_dot_n, 0.0));
        diff->mult(material->kd);
        diff->mult((**it).color);

        spec->mult(pow(fmax(r_dot_v, 0.0), material->ksp));
        spec->mult(material->ks);
        spec->mult((**it).color);

        c->add(diff); c->add(spec);
        delete diff; delete spec;
    }
    delete n;
    delete v;
}

bool Shape::inShadow(Point* p, Light* l) {
    Vector* p_to_l;
    ShadowRay* s;
    
    if (l->isPointLight()) {
        p_to_l = newVector(p, l->pos);
        s = new ShadowRay(p, p_to_l, INFINITY);
    } else if (l->isDirectLight()) {
        p_to_l = mult(l->dir, -1.0);
        s = new ShadowRay(p, p_to_l, INFINITY);
    } else {
        return false;
    }

    vector<Shape*>::iterator it = world->shapeIter();
    for (it; it != world->shapeIterEnd(); ++it) {
        float t = (*it)->intersect(s);
        
        if (t > s->t_min && t < s->t_max) {
            return true;
        }
    }
    return false;
}

Sphere::Sphere(Point* c, float r, World* w, Matrix* t, Material* m) : Shape(w, t, m) {
    center = c;
    radius = r;
}

/* the intersection of a ray and sphere is given by
 *     (d.d)t^2 + 2(d.(e-c))t + (e-c).(e-c) - r^2 = 0
 * where the ray is parameterized as p(t) = e + td
 * and (c = sphere's center) and (r = sphere's radius). 
 * This function returns -1.0 if there is no intersection. */
float Sphere::intersect(Ray* r) {
    (r->dir)->normalize();
    float d_d = dot(r->dir, r->dir);
    Vector* e_minus_c = newVector(center, r->p0);
    float d_ec = dot(r->dir, e_minus_c);
    float ec_ec = dot(e_minus_c, e_minus_c);

    delete e_minus_c;
  
    float discriminant = d_ec*d_ec - d_d*(ec_ec - radius * radius);

    if (discriminant < 0.0) {
        return -1.0;
    }

    float t0 = (-d_ec + sqrt(discriminant)) / d_d;
    float t1 = (-d_ec - sqrt(discriminant)) / d_d;

    return fmin(t0, t1);
    
}

/* returns the normal vector at point P assumin P lies on the sphere */
Vector* Sphere::getNormal(Point* p) {
    return newVector(p, center);
}


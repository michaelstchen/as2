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
    t_inverse = t->invert();
    normT = t_inverse->transpose();
    material = m;
}

Color* Shape::calcBRDF(Vector* v, Vector* n, Point* p) {
    Color* c = new Color(0,0,0);

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
    return c;
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

Sphere::Sphere(Point* c, float r, World* w, Matrix* t, Material* m)
  : Shape(w, t, m) {
    center = c;
    radius = r;
}

/* the intersection of a ray and sphere is given by
 *     (d.d)t^2 + 2(d.(e-c))t + (e-c).(e-c) - r^2 = 0
 * where the ray is parameterized as p(t) = e + td
 * and (c = sphere's center) and (r = sphere's radius). 
 * This function returns -1.0 if there is no intersection. */
float Sphere::intersect(Ray* r) {
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


Triangle::Triangle(Point* p0, Point* p1, Point* p2,
                   World* w, Matrix* t, Material* m) : Shape(w, t, m) {
    pa = p0; pb = p1; pc = p2;
    hasNormals = false;
}

Triangle::Triangle(Point* p0, Point* p1, Point* p2,
		   Vector* v0, Vector* v1, Vector* v2,
                   World* w, Matrix* t, Material* m) : Shape(w, t, m) {
  pa = p0; pb = p1; pc = p2;
  na = v0; nb = v1; nc = v2;
  hasNormals = true;
}

float Triangle::intersect(Ray* r) {
    Vector* a_min_b = newVector(pb, pa);
    Vector* a_min_c = newVector(pc, pa);

    float a = a_min_b->x; float b = a_min_b->y; float c = a_min_b->z;
    float d = a_min_c->x; float e = a_min_c->y; float f = a_min_c->z;
    float g = (r->dir)->x; float h = (r->dir)->y; float i = (r->dir)->z;
    float j = pa->x - (r->p0)->x;
    float k = pa->y - (r->p0)->y;
    float l = pa->z - (r->p0)->z;
    float M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
    delete a_min_b; delete a_min_c;

    float t = -(f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c)) / M;
    if (t < r->t_min || t > r->t_max) return -1.0;

    float gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c)) / M;
    if (gamma < 0 || gamma > 1) return -1.0;

    float beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g)) / M;
    if (beta < 0 || beta > 1 - gamma) return -1.0;

    return t;   
}

Vector* Triangle::getNormal(Point* p) {
  Vector* ret;
  Vector* b_min_a = newVector(pa, pb);
  Vector* c_min_a = newVector(pa, pc);

  ret = cross(c_min_a, b_min_a);
  delete b_min_a; delete c_min_a;
  
  return ret;
}

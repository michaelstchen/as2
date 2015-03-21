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
            Color amb = Color(1.0,1.0,1.0);
            amb.mult(material->ka);
            amb.mult((**it).color);
            c->add(&amb);
            continue;
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

        if ((**it).isPointLight()) {
            float distance = dist((**it).pos, p);
            diff->mult(1.0 / pow(distance, (**it).falloff));
            spec->mult(1.0 / pow(distance, (**it).falloff));
        }

        c->add(diff); c->add(spec);
        delete diff; delete spec;
    }
    return c;
}

bool Shape::inShadow(Point* p, Light* l) {
    Vector* p_to_l = NULL;
    float max;
    
    if (l->isPointLight()) {
        p_to_l = newVector(p, l->pos);
        max = 1.0;
    } else if (l->isDirectLight()) {
        p_to_l = mult(l->dir, -1.0);
        max = INFINITY;
    } else {
        return false;
    }

    ShadowRay s = ShadowRay(p, p_to_l, max);

    vector<Shape*>::iterator it = world->shapeIter();
    for (it; it != world->shapeIterEnd(); ++it) {
        Point* i_obj_t = NULL;
        float t = (*it)->intersect(&s, &i_obj_t);
        
        if (t > s.t_min && t < s.t_max) {
            if (i_obj_t != NULL) delete i_obj_t;
            if (p_to_l != NULL) delete p_to_l;
            return true;
        }
        if (i_obj_t != NULL) delete i_obj_t;
    }
    if (p_to_l != NULL) delete p_to_l;
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
float Sphere::intersect(Ray* r, Point** i_obj) {
    Point rt_p0 = mLeftP(t_inverse, r->p0);
    Vector rt_dir = mLeftV(t_inverse, r->dir);
    Ray rt = Ray(&rt_p0, &rt_dir);
    rt.t_min = r->t_min;
    rt.t_max = r->t_max;

    float d_d = dot(rt.dir, rt.dir);
    Vector* e_minus_c = newVector(center, rt.p0);
    float d_ec = dot(rt.dir, e_minus_c);
    float ec_ec = dot(e_minus_c, e_minus_c);

    delete e_minus_c;
  
    float discriminant = d_ec*d_ec - d_d*(ec_ec - radius * radius);

    if (discriminant < 0.0) {
        return -1.0;
    }

    float t0 = (-d_ec + sqrt(discriminant)) / d_d;
    float t1 = (-d_ec - sqrt(discriminant)) / d_d;

    *i_obj = rt.findPoint(fmin(t0, t1));
    
    return fmin(t0,t1);
    
}

/* returns the normal vector at point P assumin P lies on the sphere */
Vector* Sphere::getNormal(Point* p) {
    return newVector(p, center);
}


Triangle::Triangle(Point* p0, Point* p1, Point* p2,
                   World* w, Matrix* t, Material* m) : Shape(w, t, m) {
    pa = p0; pb = p1; pc = p2;
    hasVertexNorms = false;

    Vector* b_min_a = newVector(pa, pb);
    Vector* c_min_a = newVector(pa, pc);

    const_norm = cross(c_min_a, b_min_a);
    delete b_min_a; delete c_min_a;
    
}

Triangle::Triangle(Point* p0, Point* p1, Point* p2,
		   Vector* v0, Vector* v1, Vector* v2,
                   World* w, Matrix* t, Material* m) : Shape(w, t, m) {
    pa = p0; pb = p1; pc = p2;
    na = v0; nb = v1; nc = v2;
    hasVertexNorms = true;

    Vector* b_min_a = newVector(pa, pb);
    Vector* c_min_a = newVector(pa, pc);

    const_norm = cross(c_min_a, b_min_a);
    delete b_min_a; delete c_min_a;

}

float Triangle::intersect(Ray* r, Point** i_obj) {
    Point rt_p0 = mLeftP(t_inverse, r->p0);
    Vector rt_dir = mLeftV(t_inverse, r->dir);
    Ray rt = Ray(&rt_p0, &rt_dir);
    rt.t_min = r->t_min;
    rt.t_max = r->t_max;

    Vector* a_min_b = newVector(pb, pa);
    Vector* a_min_c = newVector(pc, pa);

    float a = a_min_b->x; float b = a_min_b->y; float c = a_min_b->z;
    float d = a_min_c->x; float e = a_min_c->y; float f = a_min_c->z;
    float g = (rt.dir)->x; float h = (rt.dir)->y; float i = (rt.dir)->z;
    float j = pa->x - (rt.p0)->x;
    float k = pa->y - (rt.p0)->y;
    float l = pa->z - (rt.p0)->z;
    float M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
    delete a_min_b; delete a_min_c;

    float t = -(f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c)) / M;
    if (t < rt.t_min || t > rt.t_max) return -1.0;

    float gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c)) / M;
    if (gamma < 0 || gamma > 1) return -1.0;

    float beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g)) / M;
    if (beta < 0 || beta > 1 - gamma) return -1.0;

    *i_obj = rt.findPoint(t);

    return t;   
}

Vector* Triangle::getNormal(Point* p) {
    if (!hasVertexNorms) return mult(const_norm, 1.0);
    Vector* n = mult(const_norm, -1);
    float n_mag = sqrt(pow(n->x, 2) + pow(n->y, 2) + pow(n->z, 2));

    float alpha; float beta; float gamma;
    getBary(p, &alpha, &beta, &gamma);
    
    Vector* a_contrib = mult(na, alpha);
    Vector* b_contrib = mult(nb, beta);
    Vector* c_contrib = mult(nc, gamma);

    Vector* a_plus_b = add(a_contrib, b_contrib);
    Vector* interpol_n = add(a_plus_b, c_contrib);
    delete a_contrib; delete b_contrib; delete c_contrib; delete a_plus_b;

    return mult(interpol_n, -1);
}

void Triangle::getBary(Point* p, float* a, float* b, float* c) {
    Vector* n = mult(const_norm, -1);
    float n_mag_sq = pow(n->x, 2) + pow(n->y, 2) + pow(n->z, 2);

    Vector* c_min_b = newVector(pb, pc);
    Vector* p_min_b = newVector(pb, p);
    Vector* n_a = cross(c_min_b, p_min_b);
    delete c_min_b; delete p_min_b;

    Vector* a_min_c = newVector(pc, pa);
    Vector* p_min_c = newVector(pc, p);
    Vector* n_b = cross(a_min_c, p_min_c);
    delete a_min_c; delete p_min_c;

    *a = dot(n, n_a) / n_mag_sq;
    *b = dot(n, n_b) / n_mag_sq;
    *c = 1.0 - *a - *b;
    delete n_a; delete n_b; delete n;
}

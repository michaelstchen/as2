#include <vector>
#include <stdio.h>
#include "raytracer.h"
#include "geomobj.h"

/* World class implemetations */
void World::addLight(Light* l) {
    lights.push_back(l);
}

int World::numOfLights() {
    return lights.size();
}

vector<Light*>::iterator World::lightIter() {
    return lights.begin();
}

vector<Light*>::iterator World::lightIterEnd() {
    return lights.end();
}

void World::addShape(Shape* s) {
    shapes.push_back(s);
}

int World::numOfShapes() {
    return shapes.size();
}

vector<Shape*>::iterator World::shapeIter() {
    return shapes.begin();
}

vector<Shape*>::iterator World::shapeIterEnd() {
    return shapes.end();
}


/* ImgPlane class implementations */
ImgPlane::ImgPlane(Point* LL, Point* LR, Point* UL, Point* UR, int w, int h) {
    ll = LL; lr = LR; ul = UL; ur = UR;
    height = h; width = w;
    pixels.resize(height*width, NULL);
}

int ImgPlane::getHeight() {
    return height;
}

int ImgPlane::getWidth() {
    return width;
}

Point* ImgPlane::getPixelPos(int i, int j) {
    float u = (float) i / (float) width + 0.5 / (float) width;
    float v = (float) j / (float) height + 0.5 / (float) height;

    Point* vLL = mult(ll, v);
    Point* vUL = mult(ul, 1.0 - v);
    Point* left = add(vLL, vUL);
    delete vLL; delete vUL;

    Point* vLR = mult(lr, v);
    Point* vUR = mult(ur, 1.0 - v);
    Point* right = add(vLR, vUR);
    delete vLR; delete vUR;

    Point* uLeft = mult(left, 1.0 - u);
    Point* uRight = mult(right, u);
    delete left; delete right;
    Point* ret = add(uLeft, uRight);
    delete uLeft; delete uRight;
    
    return ret;
}

void ImgPlane::setPixelColor(int i, int j, Color* c) {
    pixels[i + j*width]=c;
}

Color* ImgPlane::getPixelColor(int i, int j) {
    return pixels[i + j*width];
}


/* Scene class implemetation. */
Scene::Scene(World* w, ImgPlane* v, Point* c) {
    world = w;
    view = v;
    camera = c;
    depth = 1;
}

Color* Scene::traceRay(Ray* e, int d) {
    Color* c = new Color(0,0,0);
    Shape* s = NULL;
    Point* i_obj = NULL;
    
    float t = -1.0;
    if (d <= 0) return c;

    vector<Shape*>::iterator shape_it = world->shapeIter();
    for (shape_it; shape_it != world->shapeIterEnd(); ++shape_it) {
        Point* i_obj_t = NULL;
        float currT = (**shape_it).intersect(e, &i_obj_t);
        if ( currT >= e->t_min && currT <= e->t_max && ( t<0 || currT<t ) ) {
            t = currT;
            s = (*shape_it);
            if (i_obj != NULL) delete i_obj;
            i_obj = i_obj_t;
        } else {
            if (i_obj_t != NULL) delete i_obj_t;
        }
    }

    if (i_obj != NULL) {
        Vector* n_obj = s->getNormal(i_obj);
        Vector n_world = mLeftV(s->normT, n_obj);
        delete n_obj;

        Point i_world = mLeftP(s->transform, i_obj);

        n_world.normalize(); (e->dir)->normalize();

        Color* brdf = s->calcBRDF(e->dir, &n_world, &i_world);
        c->add(brdf);
        delete brdf;

        Vector* temp = mult(&n_world, 2.0 * dot(e->dir, &n_world));
        Vector* r = sub(e->dir, temp);
        delete temp;

        ReflectRay rray = ReflectRay(&i_world, r);
        Color* rcolor = traceRay(&rray, --d);
        rcolor->mult(s->material->kr);
        c->add(rcolor);

        delete r; delete rcolor;

    }

    delete i_obj;
    return c;
    
}

void Scene::render() {
    for (int j = 0; j < view->getHeight(); j++) {
        for (int i = 0; i < view->getWidth(); i++) {
            Point* pixelLoc = view->getPixelPos(i, j);
            Vector* eye_dir = newVector(camera, pixelLoc);
            EyeRay e = EyeRay(camera, eye_dir);

            view->setPixelColor(i, j, traceRay(&e, depth));

            delete pixelLoc; delete eye_dir;
        }
    }
}

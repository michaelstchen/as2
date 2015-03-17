#include <vector>
#include <stdio.h>
#include "raytracer.h"

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

void World::clearMem() {
    vector<Light*>::iterator light_it = lightIter();
    for (light_it; light_it != lightIterEnd(); ++light_it) {
        delete (**light_it).pos;
        delete (**light_it).dir;
        delete (**light_it).color;
        delete *light_it;
    }

    vector<Shape*>::iterator shape_it = shapeIter();
    for (shape_it; shape_it != shapeIterEnd(); ++shape_it) {
        delete (**shape_it).transform; delete (**shape_it).normT;
        delete ((**shape_it).material)->ka; delete ((**shape_it).material)->kd; delete ((**shape_it).material)->ks; delete ((**shape_it).material)->kr;
        delete *shape_it;
    }
    
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

void ImgPlane::clearMem() {
    delete ll; delete lr; delete ul; delete ur;
    vector<Color*>::iterator it = pixels.begin();
    for (it; it != pixels.end(); ++it) {
        delete *it;
    }
}


/* Scene class implemetation. */
Scene::Scene(World* w, ImgPlane* v, Point* c) {
    world = w;
    view = v;
    camera = c;
}

Color* Scene::traceRay(Ray* e) {
    Color* c = new Color(0,0,0);
    Shape* s;
    float t = -1.0;

    vector<Shape*>::iterator shape_it = world->shapeIter();
    for (shape_it; shape_it != world->shapeIterEnd(); ++shape_it) {
        float currT = (**shape_it).intersect(e);
        if (currT < (unsigned) t && currT > 0) {
            t = currT;
            s = (*shape_it);
        }
    }

    Point* inter = e->findPoint(t);
    if (inter != NULL) {
        Color* brdf = s->calcBRDF(e, inter);
        c->add(brdf);
        delete brdf;
    }

    delete inter;
    return c;
    
}

void Scene::render() {
    for (int j = 0; j < view->getHeight(); j++) {
        for (int i = 0; i < view->getWidth(); i++) {
            Point* pixelLoc = view->getPixelPos(i, j);
            Vector* eye_dir = newVector(camera, pixelLoc);
            EyeRay* e = new EyeRay(camera, eye_dir);

            view->setPixelColor(i, j, traceRay(e));

            delete pixelLoc; delete eye_dir;
        }
    }
}

void Scene::clearMem() {
    delete world;
    delete view;
    delete camera;
}

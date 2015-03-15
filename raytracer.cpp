#include <vector>
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


/* ImgPlane class implementations */
ImgPlane::ImgPlane(Point* p0, Point* p1, Point* p2, Point* p3, int h, int w) {
    ll = p0; lr = p1; ul = p2; ur = p3;
    height = h; width = w;
    pixels.resize(height*width, new Color(0.0, 0.0, 0.0));
}

int ImgPlane::getHeight() {
    return height;
}

int ImgPlane::getWidth() {
    return width;
}

Point* ImgPlane::getPixelPos(int i, int j) {
    float u = (float) i / (float) width;
    float v = (float) j / (float) height;
    Point* vLL = mult(ll, v);
    Point* vUL = mult(ul, 1.0 - v);
    Point* left = add(vLL, vUL);
    delete vLL; delete vUL;

    Point* vLR = mult(lr, v);
    Point* vUR = mult(ur, 1.0 - v);
    Point* right = add(vLR, vUR);
    delete vLR; delete vUR;

    Point* uLeft = mult(left, u);
    Point* uRight = mult(right, 1.0 - u);
    Point* ret = add(uLeft, uRight);
    delete uLeft; delete uRight;
    
    return ret;
}

void ImgPlane::setPixelColor(int i, int j, Color* c) {
    pixels[i + j*width] = c;
}

Color* ImgPlane::getPixelColor(int i, int j) {
    return pixels[i + j*width];
}


/* Scene class implemetation. */
Scene::Scene(World* w, ImgPlane* v, Point* c) {
    world = w;
    view = v;
    camera = c;
}

Color* traceEye(EyeRay* e) {
    
}

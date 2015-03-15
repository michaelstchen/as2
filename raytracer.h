#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "light.h"
#include "geomobj.h"
#include "property.h"
#include "ray.h"

using namespace std;

class World {
 private:
    vector<Light*> lights;
    vector<Shape*> shapes;
 public:
    World() {}
    void addLight(Light l);
    int numOfLights();
    Light* getLight(int i);

    void addShape(Shape* s);
    int numOfShapes();
    Shape* getShape(int index);
};

class ImgPlane {
 private:
    Point ll; Point lr; Point ul; Point ur;
    int height; int width;
    vector< vector<Color*> > pixels;
 public:
    ImgPlane(Point* p0, Point* p1, Point* p2, Point p3, int height, int width);
    int getHeight();
    int getWidth();
    Point* getPixelPos(int u, int v);
    Color* getPixelColor(int u, int v);
    void setPixelColor(int u, int v, Color* c);
};

class Scene {
 public:
    Scene(World* w, ImgPlane* v, Point* c);
    World* world;
    ImgPlane* view;
    Point* camera;
    Color* traceEye(EyeRay* e);
    //Color* traceShadow(ShadowRay* s);
};

#endif

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "light.h"
#include "geomobj.h"
#include "property.h"

using namespace std;

class World {
 private:
    vector<Light*> lights;
    vector<Shape*> shapes;
 public:
    void addLight(Light l);
    int numOfLights();
    Light* getLight(int i);

    void addShape(Shape* s);
    int numOfShapes();
    Light* getShape(int index);
};

class ImgPlane {
 private:
    Point ll; Point lr; Point ul; Point ur;
    int height; int width;
    vector< vector<Color*> > pixels;
 public:
    int getHeight();
    int getWidth();
    Point* getPixelPos(int u, int v);
    Color* getPixelColor(int u, int v);
    void setPixelColor(int u, int v, Color* c);
};

#endif

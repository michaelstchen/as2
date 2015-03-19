#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "light.h"
#include "geomobj.h"
#include "property.h"
#include "ray.h"

//*************************************************
// Contains classes of higher level objects for the
// raytracer.
//
// The Classes:
//
//     --World: holds all of our lights and shapes
//     --ImgPlane: stores our image plane's dimensions
//                 and location with respect to the other
//                 World objects. stores pixel color data.
//     --Scene: includes our World, ImgPlane, and a Camera.
//              render() is used to render the scene and 
//              stores pixel color data in the ImgPlane.
//
//************************************************


using namespace std;

class World {
 public:
    vector<Light*> lights;
    vector<Shape*> shapes;
    World() {}
    void addLight(Light* l);
    int numOfLights();
    vector<Light*>::iterator lightIter();
    vector<Light*>::iterator lightIterEnd();

    void addShape(Shape* s);
    int numOfShapes();
    vector<Shape*>::iterator shapeIter();
    vector<Shape*>::iterator shapeIterEnd();

};

class ImgPlane {
 private:
    /* positions of image corners in world coordinates. */
    Point* ll; Point* lr; Point* ul; Point* ur;
    /* in number of pixels */
    int height; int width;
    /* used to store our pixel color data. */
    vector<Color*> pixels;
 public:
    ImgPlane(Point* LL, Point* LR, Point* UL, Point* UR, int w, int h);

    /* returns dimensions in number of pixels */
    int getHeight();
    int getWidth();

    /* converts pixel coordinates to world coordinates. (0,0) represents
       the upper left pixel. 0 <= i < w. 0 <= j < h. */
    Point* getPixelPos(int i, int j);

    Color* getPixelColor(int i, int j);
    void setPixelColor(int i, int j, Color* c);

};

class Scene {
 public:
    Scene(World* w, ImgPlane* v, Point* c);
    World* world;
    ImgPlane* view;
    Point* camera;
    int depth;

    Color* traceRay(Ray* e_world, int depth);

    /* renders our scene. stores pixel color data in the VIEW member obj 
     * (type ImgPlane). */
    void render();

};

#endif

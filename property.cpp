//***************************************************
// 
// Author: Michael Stephen Chen
//
// Description:
//    Definitions for an object's surface properties
//    (i.e. color, specular highlight, etc.).
// 
//***************************************************


#include "property.h"
#include <cmath>

Color::Color(float rIn, float gIn, float bIn) {
    r = rIn; g = gIn; b = bIn;
}

int Color::bit8R() {
    return fmin(r * 255, 255);
}

int Color::bit8G() {
    return fmin(g * 255, 255);
}

int Color::bit8B() {
    return fmin(b * 255, 255);
}

void Color::add(Color* color) {
    r = r + color->r;
    g = g + color->g;
    b = b + color->b;
}

void Color::mult(float scalar) {
    r = r * scalar;
    g = g * scalar;
    b = b * scalar;
}

void Color::mult(Color* c) {
    r = r * c->r;
    g = g * c->g;
    b = b * c->b;
}

Material::Material(Color* kaIN, Color* kdIN, Color* ksIN, Color* krIN, float kspIN) {
    ka = kaIN; kd = kdIN; ks = ksIN; kr = krIN;
    ksp = kspIN;
}

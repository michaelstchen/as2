#include "property.h"

Color::Color(float rIn, float gIn, float bIn) {
    r = rIn; g = gIn; b = bIn;
}

int Color::bit8R() {
    return r * 255;
}

int Color::bit8G() {
    return g * 255;
}

int Color::bit8B() {
    return b * 255;
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


Material::Material(Color kaIN, Color kdIN, Color ksIN, Color krIN, float kspIN) {
    ka = kaIN; kd = kdIN; ks = ksIN; kr = krIN;
    ksp = kspIN;
}

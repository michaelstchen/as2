#include "light.h"
#include <cstddef>
#include <cmath>

Point_Light::Point_Light(Color* c, Point* p, int f) {
    color = c;
    pos = p;
    dir = NULL;
    falloff = f;
}

Direct_Light::Direct_Light(Color* c, Vector* v) {
    color = c;
    pos = NULL;
    dir = v;
}

Ambient_Light::Ambient_Light(Color* c) {
    pos = NULL;
    dir = NULL;
    color = c;
}

#include "light.h"

Point_Light::Point_Light(Color* c, Point* p, int f) {
    color = c;
    pos = p;
    falloff = f;
}

Direct_Light::Direct_Light(Color* c, Vector* v) {
    color = c;
    dir = v;
}

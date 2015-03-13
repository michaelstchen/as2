#include "linalg.h"
#include <cmath>

Point::Point(float xIn, float yIn, float zIn) {
    x = xIn; y = yIn; z = zIn;
}

Vector::Vector(float xIn, float yIn, float zIn) {
    x = xIn; y = yIn; z = zIn;
}

void Vector::normalize() {
    float mag = sqrt(x*x + y*y + z*z);
    if (mag == 0) return;
    x = x / mag; y = y / mag; z = z / mag;
}

Vector* add(Vector* v1, Vector* v2) {
    Vector* ret = new Vector();
    ret->x = v1->x + v2->x;
    ret->y = v1->y + v2->y;
    ret->z = v1->z + v2->z;
    return ret;
}

Vector* mult(Vector* v1, float scalar) {
    Vector* ret = new Vector();
    ret->x = ret->x * scalar;
    ret->y = ret->y * scalar;
    ret->z = ret->z * scalar;
    return ret;
}

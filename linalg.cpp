#include "linalg.h"
#include <cmath>
#include <cstddef>

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

Point* add(Point* p, Vector* v) {
    Point* ret = new Point();
    ret->x = p->x + v->x;
    ret->y = p->y + v->y;
    ret->z = p->z + v->z;
    return ret;
}

Vector* mult(Vector* v1, float scalar) {
    Vector* ret = new Vector();
    ret->x = ret->x * scalar;
    ret->y = ret->y * scalar;
    ret->z = ret->z * scalar;
    return ret;
}

float dot(Vector* v1, Vector* v2) {
    return (v1->x)*(v2->x) + (v1->y)*(v2->y) + (v1->z)*(v2->z);
}

Vector* cross(Vector* v1, Vector* v2) {
    Vector* ret = new Vector();
    ret->x = (v1->y)*(v2->z) - (v1->z)*(v2->y);
    ret->y = (v1->x)*(v2->z) - (v1->z)*(v2->x);
    ret->z = (v1->x)*(v2->y) - (v1->y)*(v2->x);
    return ret;
}

Vector* newVector(Point* p1, Point* p2) {
    Vector* ret = new Vector();
    ret->x = p2->x - p1->x;
    ret->y = p2->y - p1->y;
    ret->z = p2->z - p1->z;
    return ret;
}


Matrix* Matrix::invert() {
    return NULL;
}

Matrix* Matrix::transpose() {
    return NULL;
}

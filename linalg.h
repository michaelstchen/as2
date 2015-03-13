#ifndef LINALG_H
#define LINALG_H

class Point {
 public:
    Point() {}
    Point(float xIn, float yIn, float zIn);
    float x; float y; float z;
};

class Vector {
 public:
    Vector() {}
    Vector(float xIn, float yIn, float zIn);
    float x; float y; float z;
    void normalize();
    bool isNormal() {return false;}
};

class Normal : public Vector {
 public:
    bool isNormal() {return true;}
};

Vector* add(Vector* v1, Vector* v2);
Vector* mult(Vector* v, float scalar);
Vector* dot(Vector* v1, Vector* v2);
/* returns a pointer to v1 x v2 */
Vector* cross(Vector* v1, Vector* v2);
/* returns a pointer to the vector p2 - p1 */
Vector* newVector(Point* p1, Point* p2);

class Matrix {
 protected:
    float mat[4][4] = 
        {
            {1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
        };
 public:
    float getVal(int m, int n);
    void setVal(int m, int n, float val);
    void print();
    Matrix* invert();
};

Matrix* makeTransl(float tx, float ty, float tz);
Matrix* makeRot(float rx, float ry, float rz);
Matrix* makeScale(float sx, float sy, float sz);
Matrix* compose(Matrix* a, Matrix* b);

Point* matrixmult(Matrix* a, Point* b);
Vector* matrixmult(Matrix* a, Vector* b);

#endif
